#include "websocket_endpoint.h"

namespace sensr
{   
    WebSocketEndPoint::WebSocketEndPoint() : 
        status_(Status::kConnecting), msg_receiver_(0), err_receiver_(0)
    {
        endpoint_.clear_access_channels(websocketpp::log::alevel::all);
        endpoint_.clear_error_channels(websocketpp::log::elevel::all);

        endpoint_.init_asio();
        endpoint_.start_perpetual();

        thread_.reset(new std::thread(&websocketpp_client::run, &endpoint_));
    }

    WebSocketEndPoint::~WebSocketEndPoint() {
        endpoint_.stop_perpetual();
        Close(websocketpp::close::status::going_away);
        if (thread_->joinable()) {
            thread_->join();
        }
    }

    bool WebSocketEndPoint::Connect(const std::string &uri, MsgReceiver func, ErrorReceiver err_func) {
        
        if (!connection_hdl_.expired()) {
            std::cout << uri << " is already connected." << std::endl;
            return true;
        }
        std::error_code ec;
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


} // namespace sensr