#include <iostream>
#include "tcp/TcpServer.h"
#include "base/ByteBuffer.h"
#include "base/Timestamp.h"
#include "base/Logger.h"
#include "base/Thread.h"

#include <string>
#include <functional>

using namespace luckyao;
class EchoServer
{
public:
    EchoServer(EventLoop *loop,
               const InetAddress &addr,
               const std::string &name)
        : server_(loop, addr, name), loop_(loop)
    {
        // 注册回调函数
        server_.setConnectionCallback(
            std::bind(&EchoServer::onConnection, this, std::placeholders::_1));

        server_.setMessageCallback(
            std::bind(&EchoServer::onMessage, this,
                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 设置合适的loop线程数量 loopthread
        server_.setThreadNum(3);
    }
    void start()
    {
        server_.start();
    }

private:
    // 连接建立或者断开的回调
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
        }
    }

    // 可读写事件回调
    void onMessage(const TcpConnectionPtr &conn,
                   ByteBuffer &buf,
                   Timestamp time)
    {
        std::string msg = buf.retrieveAllAsString();
        conn->send(msg);
        conn->shutdown(); // 写端   EPOLLHUP =》 closeCallback_
    }

    EventLoop *loop_;
    TcpServer server_;
};
int main()
{

    LOG_SET_FILE("test.log");
    LOG_INFO("%s", "Hello Debug");
    // luckyao::Thread t([&]()
    //                   { std::cout << "Hello World:" << luckyao::Timestamp::now().toString() << std::endl; });
    // t.start();
    // t.join();

    EventLoop loop;
    InetAddress addr(8000);
    EchoServer server(&loop, addr, "EchoServer-01"); // Acceptor non-blocking listenfd  create bind
    server.start();                                  // listen  loopthread  listenfd => acceptChannel => mainLoop =>
    loop.loop();                                     // 启动mainLoop的底层Poller

    return 0;
}