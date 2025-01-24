#include "Index.h"
#include "base/Logger.h"
#include "HtmlServer.h"

using namespace LuckyApp;

REGISTER_CLASS(Index);
REGISTER_CLASS_METHOD(Index, show, void, const Request &, Response &);
REGISTER_CLASS_METHOD(Index, index, void, const Request &, Response &);

Index::Index()
{
    m_htmlTpl = HtmlTemplate::fromPath(Singleton<HtmlServer>::instance()->getTplPath() + "/index.tpl");
}
Index::~Index()
{
}
void Index::index(const Request &req, Response &resp)
{
    // resp.html("<h1 style=\"text-align: center;\">yazi-web</h1><p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>");
    // std::string html = "<p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>";

    // std::string body = "<h1 style=\"text-align: center;\">yazi-web</h1><p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>";
    // resp.html(body);
    // std::string json = R"({"name":"Alice","age":25,"isStudent":false})";
    // resp.json(json);
    // LOG_DEBUG("Index::index :%d", body.length());

    Json obj = {
        {"username", "你好啊111"},
    };
    m_htmlTpl->setValue(obj);
    std::string body = m_htmlTpl->render();
    LOG_DEBUG("Index::index :%s", body.c_str());
    resp.html(body);
}

void Index::show(const Request &req, Response &resp)
{
    // resp.html("index-show");
    LOG_DEBUG("Index::show");
}