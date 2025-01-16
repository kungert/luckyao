#pragma once
#include "base/Callbacks.h"
#include <functional>
#include <string>
#include <memory>
#include <map>

namespace luckyao
{
    class Timestamp;
    class TcpServer;
    class ResourceHost;
    class Response;
    class Request;
    class EventLoop;
    typedef std::function<void(const Request &, Response &)> ServerHandlerType;
    class HttpServer
    {
    public:
        HttpServer(EventLoop *loop, int threadNum, const std::string &nameArg, int nPort = 80);
        // HttpServer(int nPort = 80);

        virtual ~HttpServer();

        void setStaticServer(const std::string &path);

        void start();

        void bind(const std::string &path, ServerHandlerType handler);

        void sendResponse(const TcpConnectionPtr &conn, Response &resp, bool disconnect = false);

        void sendStatusResponse(const TcpConnectionPtr &conn, int status, std::string msg = "");

    private:
        bool handleControl(const TcpConnectionPtr &conn, const Request &req);
        void handleStaticHtml(const TcpConnectionPtr &conn, Request &req);
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn,
                       ByteBuffer &buf,
                       Timestamp time);

    private:
        std::shared_ptr<TcpServer> m_tcpserver;

        EventLoop *m_loop;

        std::string m_htdocsPath; // 静态路径地址

        bool m_bStaticServer; // 是否是静态服务器

        ResourceHost *m_resourceHost;

        std::map<std::string, ServerHandlerType> m_handlers;
    };
}