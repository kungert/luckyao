#pragma once
#include "web/Controller.h"

using namespace luckyao;

namespace LuckyApp
{
    class Index : public Controller
    {
    public:
        Index() = default;
        ~Index() = default;

        void index(const Request &request, Response &response);
        void show(const Request &req, Response &resp);
    };
} // namespace name
