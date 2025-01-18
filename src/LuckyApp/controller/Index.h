#pragma once
#include "web/Controller.h"
#include "web/HtmlTemplate.h"
using namespace luckyao;

namespace LuckyApp
{
    class Index : public Controller
    {
    public:
        Index();
        virtual ~Index();

        void index(const Request &request, Response &response);
        void show(const Request &req, Response &resp);

    private:
        HtmlTemplate *m_htmlTpl;
    };
} // namespace name
