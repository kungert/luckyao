/*
 * @Author: kunger
 * @Date: 2023-06-09 21:54:41
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:06:43
 */
#pragma once
#include <string>

namespace luckyao
{
    class ClassField
    {
    public:
        ClassField() : m_name(""), m_type(""), m_offset(0) {}
        ClassField(const std::string &name, const std::string &type, size_t offset) : m_name(name), m_type(type), m_offset(offset) {}
        ~ClassField() {}

        const std::string &name()
        {
            return m_name;
        }

        const std::string &type()
        {
            return m_type;
        }

        size_t offset()
        {
            return m_offset;
        }

    private:
        std::string m_name;
        std::string m_type;
        std::size_t m_offset;
    };

}