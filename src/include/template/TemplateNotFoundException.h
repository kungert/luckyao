#pragma once
#include "exception/BaseException.h"

namespace luckyao
{
    class TemplateNotFoundException : public BaseException
    {
    public:
        explicit TemplateNotFoundException(const std::string &msg) : BaseException(msg) {}
        TemplateNotFoundException() { setMsg("template not found"); }
    };
}