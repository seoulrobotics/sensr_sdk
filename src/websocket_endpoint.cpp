#include "websocket_endpoint.h"

namespace sensr
{   
    template <typename T>
    void connection_metadata<T>::on_open(client *c, websocketpp::connection_hdl hdl)
    {
        m_status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
    }
    template <typename T>
    void connection_metadata<T>::on_fail(client *c, websocketpp::connection_hdl hdl)
    {
        m_status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
        m_error_reason = con->get_ec().message();
    }

    template <typename T>
    void connection_metadata<T>::on_close(client *c, websocketpp::connection_hdl hdl)
    {
        m_status = "Closed";
        client::connection_ptr con = c->get_con_from_hdl(hdl);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " ("
          << websocketpp::close::status::get_string(con->get_remote_close_code())
          << "), close reason: " << con->get_remote_close_reason();
        m_error_reason = s.str();
    }

    template <typename T>
    void connection_metadata<T>::on_message(websocketpp::connection_hdl hdl, client::message_ptr msg)
    {
        std::string message;
        if (msg->get_opcode() == websocketpp::frame::opcode::text)
        {
            message = msg->get_payload();
        }
        else
        {
            message = websocketpp::utility::to_hex(msg->get_payload());
        }
        if (msg_receiver_) 
        {
            T output;
            output.ParseFromString(message);        
            msg_receiver_(output);
        }
    }


    template <typename MsgType>
    websocket_endpoint<MsgType>::websocket_endpoint()
    {
        m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

        m_endpoint.init_asio();
        m_endpoint.start_perpetual();

        m_thread.reset(new websocketpp::lib::thread(&client::run, &m_endpoint));
    }

    template <typename MsgType>
    websocket_endpoint<MsgType>::~websocket_endpoint() {
        m_endpoint.stop_perpetual();
        close(websocketpp::close::status::going_away);
        m_thread->join();
    }

    template <typename MsgType>
    void websocket_endpoint<MsgType>::close(websocketpp::close::status::value code) {
        websocketpp::lib::error_code ec;
        if (m_connection == nullptr) {
            std::cout << "> No connection found" << std::endl;
            return;
        }

        if (m_connection->get_status() != "Open") {
            // Only close open connections
            m_endpoint.close(m_connection->get_hdl(), code, "", ec);
            if (ec) {
                std::cout << "> Error closing connection " << m_connection->get_id() << ": " << ec.message() << std::endl;
            }            
        }
        m_connection.reset();
    }
} // namespace sensr