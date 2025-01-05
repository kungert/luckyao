#include "loop/EventLoop.h"
#include "http/HttpServer.h"
#include <memory>
class HtmlServer
{
public:
    HtmlServer(luckyao::EventLoop *loop);

private:
    std::shared_ptr<luckyao::HttpServer> m_httpServer;
};