#pragma once
#include "exception/BaseException.h"
namespace luckyao
{

    class TemplateParseException : public BaseException
    {
    public:
        explicit TemplateParseException(const std::string &msg) : BaseException(msg) {}
        TemplateParseException() { setMsg("模板解析错误"); }
    };
}