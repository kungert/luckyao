/*
 * @Author: kunger
 * @Date: 2023-06-09 22:00:53
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 22:01:30
 */

#include "reflect/Object.h"
#include "reflect/ClassField.h"
#include "reflect/ClassFactory.h"

using namespace luckyao;
using namespace std;

Object::Object()
    : m_className("")
{
}

Object::~Object()
{
}

void Object::setClassName(const string &className)
{
    m_className = className;
}

const string &Object::getClassName() const
{
    return m_className;
}

int Object::getFieldCount()
{
    return Singleton<ClassFactory>::instance()->getClassFieldCount(m_className);
}

ClassField *Object::getField(int pos)
{
    return Singleton<ClassFactory>::instance()->getClassField(m_className, pos);
}

ClassField *Object::getField(const string &fieldName)
{
    return Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
}

void Object::set(const string &fieldName, const char *value)
{
    ClassField *field = Singleton<ClassFactory>::instance()->getClassField(m_className, fieldName);
    size_t offset = field->offset();
    *((string *)((unsigned char *)(this) + offset)) = string(value);
}
