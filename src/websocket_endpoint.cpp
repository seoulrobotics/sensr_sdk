#include "websocket_endpoint.h"

namespace sensr
{   
  WebSocketEndPoint::WebSocketEndPoint() : 
    status_(Status::kConnecting), msg_receiver_(0), err_receiver_(0),
    certified_names_({"server"})
  {
    endpoint_.set_access_channels(websocketpp::log::alevel::all);
    endpoint_.clear_access_channels(websocketpp::log::alevel::frame_payload);
    endpoint_.set_error_channels(websocketpp::log::elevel::all);

    endpoint_.init_asio();
    endpoint_.start_perpetual();

    thread_ = std::thread(&websocketpp_client::run, &endpoint_);
  }

  WebSocketEndPoint::~WebSocketEndPoint() {
    endpoint_.stop_perpetual();
    Close(websocketpp::close::status::going_away);
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  bool WebSocketEndPoint::Connect(const std::string &uri, MsgReceiver func, ErrorReceiver err_func) {
    
    if (!connection_hdl_.expired()) {
      std::cout << uri << " is already connected." << std::endl;
      return true;
    }
    std::error_code ec;

    // Register our message handler
    endpoint_.set_tls_init_handler(std::bind(
      &WebSocketEndPoint::OnTSLInit,
      this,
      std::placeholders::_1));
    websocketpp_client::connection_ptr con = endpoint_.get_connection(uri, ec);
    if (ec) {
      std::cerr << "> Connect initialization error: " << ec.message() << std::endl;
      return false;
    }
    msg_receiver_ = func;
    err_receiver_ = err_func; 
    connection_hdl_ = con->get_handle();
    con->set_open_handler(std::bind(
      &WebSocketEndPoint::OnOpen,
      this,
      &endpoint_,
      std::placeholders::_1));
    con->set_fail_handler(std::bind(
      &WebSocketEndPoint::OnFail,
      this,
      &endpoint_,
      std::placeholders::_1));
    con->set_message_handler(std::bind(
      &WebSocketEndPoint::OnMessage,
      this,
      std::placeholders::_1,
      std::placeholders::_2));

    endpoint_.connect(con);
    endpoint_.get_alog().write(websocketpp::log::alevel::app, "Connecting to " + uri);
    return true;
  };

  void WebSocketEndPoint::Close(websocketpp::close::status::value code) {
    std::error_code ec;
    if (connection_hdl_.expired()) {
      //std::cout << "> No connection found" << std::endl;
    } else {
      if (status_ != Status::kClosed) {
        // Only close open connections
        endpoint_.close(connection_hdl_, code, "", ec);
        if (ec) {
          std::cerr << "> Error closing connection : " << ec.message() << std::endl;
        }      
      }
    }    
    connection_hdl_.reset();
    msg_receiver_ = 0;
    err_receiver_ = 0;
  }

  context_ptr WebSocketEndPoint::OnTSLInit(websocketpp::connection_hdl hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);


        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
        ctx->set_verify_callback(std::bind(&WebSocketEndPoint::OnVerifyCertificate, 
          this,
          std::placeholders::_1, 
          std::placeholders::_2));

        // Here we load the CA certificates of all CA's that this client trusts.
        ctx->load_verify_file("user.crt");
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return ctx;
  }

  void WebSocketEndPoint::OnOpen(websocketpp_client *c, websocketpp::connection_hdl hdl) {
    status_ = Status::kOpen;
    //websocketpp_client::connection_ptr con = c->get_con_from_hdl(hdl);
    //std::string server = con->get_response_header("Server");
  }

  void WebSocketEndPoint::OnFail(websocketpp_client *c, websocketpp::connection_hdl hdl) {
    status_ = Status::kFailed;
    if (err_receiver_ != 0) {
      websocketpp_client::connection_ptr con = c->get_con_from_hdl(hdl);
      err_receiver_(con->get_ec().message());
    }
  }

  void WebSocketEndPoint::OnClose(websocketpp_client *c, websocketpp::connection_hdl hdl) {
    status_ = Status::kClosed;
    // websocketpp_client::connection_ptr con = c->get_con_from_hdl(hdl);
    // std::stringstream s;
    // s << "close code: " << con->get_remote_close_code() << " ("
    //   << websocketpp::close::status::get_string(con->get_remote_close_code())
    //   << "), close reason: " << con->get_remote_close_reason();
    // m_error_reason = s.str();
  }

  void WebSocketEndPoint::OnMessage(websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg) {
    if (status_ == Status::kOpen && msg->get_opcode() == websocketpp::frame::opcode::BINARY) {
      if (msg_receiver_ != 0) {   
        msg_receiver_(msg->get_payload());
      }
    }     
  }

  /// Verify that one of the subject alternative names matches the given hostname
  bool WebSocketEndPoint::VerifySubjectAlternativeName(const char * hostname, X509 * cert) {
    STACK_OF(GENERAL_NAME) * san_names = NULL;    
    san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
    if (san_names == NULL) {
        return false;
    }    
    int san_names_count = sk_GENERAL_NAME_num(san_names);    
    bool result = false;    
    for (int i = 0; i < san_names_count; i++) {
        const GENERAL_NAME * current_name = sk_GENERAL_NAME_value(san_names, i);        
        if (current_name->type != GEN_DNS) {
            continue;
        }        
        char const * dns_name = (char const *) ASN1_STRING_get0_data(current_name->d.dNSName);        
        // Make sure there isn't an embedded NUL character in the DNS name
        if (ASN1_STRING_length(current_name->d.dNSName) != static_cast<int>(strlen(dns_name))) {
            break;
        }
        // Compare expected hostname with the CN
        result = (strcasecmp(hostname, dns_name) == 0);
    }
    sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);    
    return result;
  }

  /// Verify that the certificate common name matches the given hostname
  bool WebSocketEndPoint::VerifyCommonName(char const * hostname, X509 * cert) {
    // Find the position of the CN field in the Subject field of the certificate
    int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
    if (common_name_loc < 0) {
        return false;
    }    
    // Extract the CN field
    X509_NAME_ENTRY * common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
    if (common_name_entry == NULL) {
        return false;
    }    
    // Convert the CN field to a C string
    ASN1_STRING * common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
    if (common_name_asn1 == NULL) {
        return false;
    }    
    char const * common_name_str = (char const *) ASN1_STRING_get0_data(common_name_asn1);    
    // Make sure there isn't an embedded NUL character in the CN
    if (ASN1_STRING_length(common_name_asn1) != static_cast<int>(strlen(common_name_str))) {
        return false;
    }
    // Compare expected hostname with the CN
    return (strcasecmp(hostname, common_name_str) == 0);
  }

  /**
   * This code is derived from examples and documentation found ato00po
   * http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/ssl/client.cpp
   * and
   * https://github.com/iSECPartners/ssl-conservatory
   */
  bool WebSocketEndPoint::OnVerifyCertificate(bool preverified, 
                                              boost::asio::ssl::verify_context& ctx) {
    // The verify callback can be used to check whether the certificate that is
    // being presented is valid for the peer. For example, RFC 2818 describes
    // the steps involved in doing this for HTTPS. Consult the OpenSSL
    // documentation for more details. Note that the callback is called once
    // for each certificate in the certificate chain, starting from the root
    // certificate authority.

    // Retrieve the depth of the current cert in the chain. 0 indicates the
    // actual server cert, upon which we will perform extra validation
    // (specifically, ensuring that the hostname matches. For other certs we
    // will use the 'preverified' flag from Asio, which incorporates a number of
    // non-implementation specific OpenSSL checking, such as the formatting of
    // certs and the trusted status based on the CA certs we imported earlier.
    int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());
    // if we are on the final cert and everything else checks out, ensure that
    // the hostname is present on the list of SANs or the common name (CN).
    if (depth == 0 && preverified) {
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        for (const auto& name : certified_names_) {
          if (VerifySubjectAlternativeName(name.c_str(), cert)) {
            return true;
          } else if (VerifyCommonName(name.c_str(), cert)) {
            return true;
          } else {
            std::string errstr(
              X509_verify_cert_error_string(
              X509_STORE_CTX_get_error(ctx.native_handle())));
            std::cerr << errstr << std::endl;
            return false;
          }
        }
        
    } else {
      if (X509_STORE_CTX_get_error(ctx.native_handle()) == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT) {
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        for (const auto& name : certified_names_) {
          if (VerifyCommonName(name.c_str(), cert)) {         
            return true;
          }
        }
      }
    }
    return preverified;
  }
} // namespace sensr