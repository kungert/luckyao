#include "loop/EventLoop.h"
#include "web/HttpServer.h"
#include <memory>
class HtmlServer
{
public:
    HtmlServer();

    void start(luckyao::EventLoop *loop);

    void setTplPath(const std::string &path);
    std::string getTplPath();

private:
    std::shared_ptr<luckyao::HttpServer> m_httpServer;
    std::string m_tplPath;
};