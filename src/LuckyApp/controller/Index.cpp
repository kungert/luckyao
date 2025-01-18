#include "Index.h"
#include "base/Logger.h"

using namespace luckyao;
using namespace LuckyApp;

REGISTER_CLASS(Index);
REGISTER_CLASS_METHOD(Index, show, void, const Request &, Response &);
REGISTER_CLASS_METHOD(Index, index, void, const Request &, Response &);

void Index::index(const Request &req, Response &resp)
{
    // resp.html("<h1 style=\"text-align: center;\">yazi-web</h1><p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>");
    // std::string html = "<p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>";

    std::string body = "<h1 style=\"text-align: center;\">yazi-web</h1><p style=\"text-align: center;\">a lightweight c++ web mvc framework</p>";
    // resp.html(body);
    std::string json = R"({"name":"Alice","age":25,"isStudent":false})";
    resp.json(json);
    LOG_DEBUG("Index::index :%d", body.length());
}

void Index::show(const Request &req, Response &resp)
{
    // resp.html("index-show");
    LOG_DEBUG("Index::show");
}