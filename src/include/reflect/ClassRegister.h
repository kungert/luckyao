#pragma once
#include "ClassFactory.h"

namespace luckyao
{
    class ClassRegister
    {
    public:
        ClassRegister(const std::string &className, createObject method)
        {
            Singleton<ClassFactory>::instance()->registerClass(className, method);
        }
        ClassRegister(const std::string &className, const std::string &methodName, uintptr_t method)
        {
            Singleton<ClassFactory>::instance()->registerClassMethod(className, methodName, method);
        }
    };

#define REGISTER_CLASS(className)      \
    Object *createObject##className()  \
    {                                  \
        Object *obj = new className(); \
        obj->setClassName(#className); \
        return obj;                    \
    }                                  \
    ClassRegister classRegister##className(#className, createObject##className)

#define REGISTER_CLASS_FIELD(className, fieldName, fieldType) \
    className className##fieldName;                           \
    ClassRegister classRegister##className##fieldName(#className, #fieldName, #fieldType, (size_t)(&(className##fieldName.fieldName)) - (size_t)(&className##fieldName))

#define REGISTER_CLASS_METHOD(className, methodName, returnType, ...)                                             \
    std::function<returnType(className *, ##__VA_ARGS__)> className##methodName##method = &className::methodName; \
    ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t) & (className##methodName##method))

} // namespace name
