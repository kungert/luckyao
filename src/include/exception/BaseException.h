#pragma once

#include <exception>
#include <string>
namespace luckyao
{

#define throwException(Exception, ...)                                                                                                                                                               \
    {                                                                                                                                                                                                \
        std::cerr << "[" << #Exception << "] : FILE: " << string(__FILE__).substr(string(__FILE__).find_last_of('/') + 1) << "   LINE: " << __LINE__ << "  FUNCTION: " << __FUNCTION__ << std::endl; \
        throw Exception(__VA_ARGS__);                                                                                                                                                                \
    }
    class BaseException : public std::exception
    {
    public:
        explicit BaseException(std::string msg) : m_message(std::move(msg)) {}

        BaseException() = default;

        virtual ~BaseException() noexcept = default;

        [[nodiscard]] const char *what() const noexcept override { return m_message.c_str(); }

        void setMsg(const std::string &msg)
        {
            this->m_message = msg;
        }

    private:
        std::string m_message;
    };

} // namespace name
