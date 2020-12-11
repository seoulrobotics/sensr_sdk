#pragma once

#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"
#include <functional>

namespace sensr
{
    typedef websocketpp::client<websocketpp::config::asio_client> client;
    template <typename T>
    class connection_metadata
    {
    public:
        using MsgReceiver = std::function<void(const T& msg)>;

        connection_metadata(websocketpp::connection_hdl hdl, std::string uri, MsgReceiver receiver)
        : m_hdl(hdl), m_status("Connecting"), m_uri(uri), m_server("N/A"), msg_receiver_(receiver){}

        void on_open(client *c, websocketpp::connection_hdl hdl);
        void on_fail(client *c, websocketpp::connection_hdl hdl);
        void on_close(client *c, websocketpp::connection_hdl hdl);
        void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);

    private:
        websocketpp::connection_hdl m_hdl;
        std::string m_status;
        std::string m_uri;
        std::string m_server;
        std::string m_error_reason;
        MsgReceiver msg_receiver_;
    };

    template <typename MsgType>
    class websocket_endpoint
    {
    public:
        using MsgReceiver = std::function<void(const MsgType& msg)>;
        websocket_endpoint();
        ~websocket_endpoint();

        int connect(std::string const &uri, MsgReceiver func) {
            websocketpp::lib::error_code ec;
            client::connection_ptr con = m_endpoint.get_connection(uri, ec);
            if (ec) {
                std::cout << "> Connect initialization error: " << ec.message() << std::endl;
                return -1;
            }
            m_connection.reset(new connection_metadata<MsgType>(con->get_handle(), uri, func));

            con->set_open_handler(websocketpp::lib::bind(
                &connection_metadata<MsgType>::on_open,
                m_connection.get(),
                &m_endpoint,
                websocketpp::lib::placeholders::_1));
            con->set_fail_handler(websocketpp::lib::bind(
                &connection_metadata<MsgType>::on_fail,
                m_connection.get(),
                &m_endpoint,
                websocketpp::lib::placeholders::_1));
            con->set_message_handler(websocketpp::lib::bind(
                &connection_metadata<MsgType>::on_message,
                m_connection.get(),
                websocketpp::lib::placeholders::_1,
                websocketpp::lib::placeholders::_2));

            m_endpoint.connect(con);
            return 0;
        };
        void close(websocketpp::close::status::value code);

    private:
        client m_endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
        websocketpp::lib::unique_ptr<connection_metadata<MsgType>> m_connection;
    };
} // namespace sensr