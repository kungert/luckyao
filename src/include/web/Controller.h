#pragma once

#include "Request.h"
#include "Response.h"
#include "reflect/Object.h"

namespace luckyao
{

    class Controller : public Object
    {
    public:
        Controller() {}
        virtual ~Controller() {}
    };

}