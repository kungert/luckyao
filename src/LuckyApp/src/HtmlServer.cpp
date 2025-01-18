#include "HtmlServer.h"
HtmlServer::HtmlServer(luckyao::EventLoop *loop)
{
    m_httpServer = std::make_shared<luckyao::HttpServer>(loop, 2, "EditorServer_Static", 1234);
    // m_httpServer->setStaticServer("/home/kunger/my/luckyao/www/pear-admin-layui");
    m_httpServer->setStaticServer("/home/kunger/my/luckyao/www/my");
    m_httpServer->start();
}