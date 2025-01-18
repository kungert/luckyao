#pragma once

#include "Request.h"
#include "Response.h"
#include "reflect/ReflectObject.h"
#include "reflect/ClassRegister.h"

namespace luckyao
{

    class Controller : public ReflectObject
    {
    public:
        Controller() {}
        virtual ~Controller() {}
    };

}