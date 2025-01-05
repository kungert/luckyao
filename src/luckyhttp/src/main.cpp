#include <iostream>
#include "HtmlServer.h"
#include "loop/EventLoop.h"

using namespace luckyao;

int main()
{
    EventLoop loop;
    HtmlServer hs(&loop);
    loop.loop();
    return 0;
}