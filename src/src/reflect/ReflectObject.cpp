/*
 * @Author: kunger
 * @Date: 2023-06-09 22:00:53
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:01:30
 */

#include "reflect/ReflectObject.h"
#include "reflect/ClassField.h"
#include "reflect/ClassFactory.h"

using namespace luckyao;
using namespace std;

ReflectObject::ReflectObject()
    : m_className("")
{
}

ReflectObject::~ReflectObject()
{
}

void ReflectObject::setClassName(const string &className)
{
    m_className = className;
}

const string &ReflectObject::getClassName() const
{
    return m_className;
}

int ReflectObject::getFieldCount()
{
    return Singleton<ClassFactory>::instance()->getClassFieldCount(m_className);
}

ClassField *ReflectObject::getField(int pos)
{
    return Singleton<ClassFactory>::instance()->getClassField(m_className, pos);
}

ClassField *ReflectObject::getField(const string &fieldName)
{
    return Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
}

void ReflectObject::set(const string &fieldName, const char *value)
{
    ClassField *field = Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
    size_t offset = field->offset();
    *((string *)((unsigned char *)(this) + offset)) = string(value);
}
