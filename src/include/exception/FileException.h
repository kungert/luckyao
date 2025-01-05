#pragma once
#include "BaseException.h"
namespace luckyao
{

    class FileException : public BaseException
    {
    public:
        explicit FileException(const std::string &msg) : BaseException(msg) {}
        FileException() { setMsg("文件读写错误"); }
    };
} // namespace name
