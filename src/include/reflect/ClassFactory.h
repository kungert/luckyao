/*
 * @Author: kunger
 * @Date: 2023-06-09 20:12:41
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:07:01
 */
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>

#include "Singleton.h"
#include "ClassField.h"
#include "ClassMethod.h"

namespace luckyao
{
    class Object;

    typedef Object *(*createObject)(void);

    class ClassFactory
    {
        friend class Singleton<ClassFactory>;

    public:
        // reflect class
        void registerClass(const std::string &className, createObject method);
        Object *createClass(const std::string &className);

        // reflect class field
        void registerClassField(const std::string &className, const std::string &fieldName, const std::string &fieldType, std::size_t offset);
        int getClassFieldCount(const std::string &className);
        ClassField *getClassField(const std::string &className, int pos);
        ClassField *getClassField(const std::string &className, const std::string &fieldName);

        // reflect class method
        void registerClassMethod(const std::string &className, const std::string &methodName, uintptr_t method);
        int getClassMethodCount(const std::string &className);
        ClassMethod *getClassMethod(const std::string &className, int pos);
        ClassMethod *getClassMethod(const std::string &className, const std::string &methodName);

    private:
        ClassFactory() {}
        ~ClassFactory() {}

    private:
        std::map<std::string, createObject> m_classMap;
        std::map<std::string, std::vector<ClassField *>> m_classFields;
        std::map<std::string, std::vector<ClassMethod *>> m_classMethods;
    };
}