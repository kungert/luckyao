#include <iostream>
#include "HtmlServer.h"
#include "loop/EventLoop.h"
#include "base/Singleton.h"
using namespace luckyao;

int main()
{
    EventLoop loop;
    Singleton<HtmlServer>::instance()->start(&loop);
    loop.loop();
    return 0;
}