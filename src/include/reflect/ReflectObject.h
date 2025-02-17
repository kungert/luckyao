/*
 * @Author: kunger
 * @Date: 2023-06-09 20:27:49
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:06:20
 */
#pragma once
#include <string>
#include <ostream>

#include "base/Singleton.h"
#include "reflect/ClassFactory.h"
#include "reflect/ClassField.h"

namespace luckyao
{
    class ReflectObject
    {
    public:
        ReflectObject();

        virtual ~ReflectObject();

        void setClassName(const std::string &className);
        const std::string &getClassName() const;

        int getFieldCount();
        ClassField *getField(int pos);
        ClassField *getField(const std::string &fieldName);

        template <typename T>
        void get(const std::string &fieldName, T &value);

        template <typename T>
        void set(const std::string &fieldName, const T &value);
        void set(const std::string &fieldName, const char *value);

        template <typename R = void, typename... Args>
        R call(const std::string &methodName, Args &...args);

    private:
        std::string m_className;
    };

    template <typename T>
    void ReflectObject::get(const std::string &fieldName, T &value)
    {
        ClassField *field = Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
        if (field == nullptr)
        {
            return;
        }
        std::size_t offset = field->offset();
        value = *((T *)((unsigned char *)(this) + offset));
    }

    template <typename T>
    void ReflectObject::set(const std::string &fieldName, const T &value)
    {
        ClassField *field = Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
        if (field != nullptr)
        {
            return;
        }
        size_t offset = field->offset();
        *((T *)((unsigned char *)(this) + offset)) = value;
    }

    template <typename R, typename... Args>
    R ReflectObject::call(const std::string &methodName, Args &...args)
    {
        ClassFactory *factory = Singleton<ClassFactory>::instance();
        ClassMethod *method = factory->getClassMethod(m_className, methodName);
        if (method == nullptr)
        {
            std::ostringstream os;
            os << "reflect method " << methodName << " not exists";
            throw std::logic_error(os.str());
        }
        auto func = method->method();
        typedef std::function<R(decltype(this), Args & ...)> class_method;
        return (*((class_method *)func))(this, args...);
    }
}