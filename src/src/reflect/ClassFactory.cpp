/*
 * @Author: kunger
 * @Date: 2023-06-09 20:12:36
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:04:45
 */
#include "ClassFactory.h"
#include "ClassField.h"
#include "ClassMethod.h"

using namespace luckyao;
using namespace std;

void ClassFactory::registerClass(const string &className, createObject method)
{
    m_classMap[className] = method;
}
Object *ClassFactory::createClass(const string &className)
{
    auto it = m_classMap.find(className);
    if (it == m_classMap.end())
    {
        return nullptr;
    }
    return it->second();
}

void ClassFactory::registerClassField(const string &className, const string &fieldName, const string &fieldType, size_t offset)
{
    m_classFields[className].push_back(new ClassField(fieldName, fieldType, offset));
}

int ClassFactory::getClassFieldCount(const string &className)
{
    return m_classFields[className].size();
}

ClassField *ClassFactory::getClassField(const string &className, int pos)
{
    int size = m_classFields[className].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_classFields[className][pos];
}

ClassField *ClassFactory::getClassField(const string &className, const string &fieldName)
{
    auto fields = m_classFields[className];
    for (auto it = fields.begin(); it != fields.end(); it++)
    {
        if ((*it)->name() == fieldName)
        {
            return *it;
        }
    }
    return nullptr;
}

void ClassFactory::registerClassMethod(const string &className, const string &methodName, uintptr_t method)
{
    m_classMethods[className].push_back(new ClassMethod(methodName, method));
}

int ClassFactory::getClassMethodCount(const string &className)
{
    return m_classMethods[className].size();
}

ClassMethod *ClassFactory::getClassMethod(const string &className, int pos)
{
    int size = m_classMethods[className].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_classMethods[className][pos];
}

ClassMethod *ClassFactory::getClassMethod(const string &className, const string &methodName)
{
    auto methods = m_classMethods[className];
    for (auto it = methods.begin(); it != methods.end(); it++)
    {
        if ((*it)->name() == methodName)
        {
            return *it;
        }
    }
    return nullptr;
}
