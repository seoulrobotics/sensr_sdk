#pragma once

#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"
#include <functional>

namespace sensr
{
    typedef websocketpp::client<websocketpp::config::asio_client> client;
    class websocket_endpoint
    {
    public:
        using MsgReceiver = std::function<void(const std::string& msg)>;
        websocket_endpoint();
        ~websocket_endpoint();

        int connect(const std::string &uri, MsgReceiver func);
        void close(websocketpp::close::status::value code);

    private:
        void on_open(client *c, websocketpp::connection_hdl hdl);
        void on_fail(client *c, websocketpp::connection_hdl hdl);
        void on_close(client *c, websocketpp::connection_hdl hdl);
        void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);
        client m_endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
        websocketpp::connection_hdl m_hdl;
        std::string m_status;
        std::string m_uri;
        std::string m_server;
        std::string m_error_reason;
        MsgReceiver msg_receiver_;
    };
} // namespace sensr