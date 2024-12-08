#include "Channel.h"
// #include "EventLoop.h"
#include "Logger.h"

#include <sys/epoll.h>

using namespace luckyao;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;
Channel::Channel(EventLoop *loop, int fd)
    : m_loop(loop), m_fd(fd), m_events(0), m_recvEvents(0), m_index(-1), m_tied(false)
{
}

Channel::~Channel()
{
}
void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}