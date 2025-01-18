#include "HtmlServer.h"

HtmlServer::HtmlServer()
{
}
void HtmlServer::start(luckyao::EventLoop *loop)
{
    m_httpServer = std::make_shared<luckyao::HttpServer>(loop, 2, "EditorServer_Static", 1234);
    // m_httpServer->setStaticServer("/home/kunger/my/luckyao/www/pear-admin-layui");
    m_httpServer->setStaticServer("/home/kunger/my/luckyao/www/my");
    setTplPath("/home/kunger/my/luckyao/www/tpl");
    m_httpServer->start();
}

void HtmlServer::setTplPath(const std::string &path)
{
    m_tplPath = path;
}

std::string HtmlServer::getTplPath()
{
    return m_tplPath;
}
