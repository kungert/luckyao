#include "loop/Poller.h"
#include "loop/EPollPoller.h"

#include <stdlib.h>
using namespace luckyao;

Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("MUDUO_USE_POLL"))
    {
        return nullptr; // 生成poll的实例
    }
    else
    {
        return new EPollPoller(loop); // 生成epoll的实例
    }
}