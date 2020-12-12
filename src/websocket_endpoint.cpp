#include "websocket_endpoint.h"

namespace sensr
{   
    websocket_endpoint::websocket_endpoint() : 
        m_status("Connecting"), m_uri("N/A"), m_server("N/A")
    {
        m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

        m_endpoint.init_asio();
        m_endpoint.start_perpetual();

        m_thread.reset(new websocketpp::lib::thread(&client::run, &m_endpoint));
    }

    websocket_endpoint::~websocket_endpoint() {
        m_endpoint.stop_perpetual();
        close(websocketpp::close::status::going_away);
        if (m_thread->joinable()) {
            m_thread->join();
        }
    }

    int websocket_endpoint::connect(const std::string &uri, MsgReceiver func) {
        websocketpp::lib::error_code ec;
        client::connection_ptr con = m_endpoint.get_connection(uri, ec);
        if (ec) {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }
        m_uri = uri;
        msg_receiver_ = func;
        m_hdl = con->get_handle();

        con->set_open_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_open,
            this,
            &m_endpoint,
            websocketpp::lib::placeholders::_1));
        con->set_fail_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_fail,
            this,
            &m_endpoint,
            websocketpp::lib::placeholders::_1));
        con->set_message_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_message,
            this,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2));

        m_endpoint.connect(con);
        return 0;
    };

    void websocket_endpoint::close(websocketpp::close::status::value code) {
        websocketpp::lib::error_code ec;
        if (m_hdl.expired()) {
            std::cout << "> No connection found" << std::endl;
            return;
        }

        if (m_status != "Open") {
            // Only close open connections
            m_endpoint.close(m_hdl, code, "", ec);
            if (ec) {
                std::cout << "> Error closing connection : " << ec.message() << std::endl;
            }            
        }
        m_uri = "N/A";
        m_hdl.reset();
    }

    void websocket_endpoint::on_open(client *c, websocketpp::connection_hdl hdl) {
        m_status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
    }

    void websocket_endpoint::on_fail(client *c, websocketpp::connection_hdl hdl) {
        m_status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
        m_error_reason = con->get_ec().message();
    }

    void websocket_endpoint::on_close(client *c, websocketpp::connection_hdl hdl) {
        m_status = "Closed";
        client::connection_ptr con = c->get_con_from_hdl(hdl);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " ("
          << websocketpp::close::status::get_string(con->get_remote_close_code())
          << "), close reason: " << con->get_remote_close_reason();
        m_error_reason = s.str();
    }

    void websocket_endpoint::on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
        std::string message;
        if (msg->get_opcode() == websocketpp::frame::opcode::text) {
            message = msg->get_payload();
        } else {
            message = websocketpp::utility::to_hex(msg->get_payload());
        }
        if (msg_receiver_) {
            //T output;
            //output.ParseFromString(message);        
            msg_receiver_(message);
        }
    }


} // namespace sensr