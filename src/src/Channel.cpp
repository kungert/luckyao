#include "Channel.h"
#include "EventLoop.h"
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
    m_tie = obj;
    m_tied = true;
}
void Channel::update()
{
    // 通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    m_loop->updateChannel(this);
}

// 在channel所属的EventLoop中， 把当前的channel删除掉
void Channel::remove()
{
    m_loop->removeChannel(this);
}

// fd得到poller通知以后，处理事件的
void Channel::handleEvent(Timestamp receiveTime)
{
    if (m_tied)
    {
        std::shared_ptr<void> guard = m_tie.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

// 根据poller通知的channel发生的具体事件， 由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n", m_recvEvents);

    if ((m_recvEvents & EPOLLHUP) && !(m_recvEvents & EPOLLIN))
    {
        if (m_closeCallback)
        {
            m_closeCallback();
        }
    }

    if (m_recvEvents & EPOLLERR)
    {
        if (m_errorCallback)
        {
            m_errorCallback();
        }
    }

    if (m_recvEvents & (EPOLLIN | EPOLLPRI))
    {
        if (m_readCallback)
        {
            m_readCallback(receiveTime);
        }
    }

    if (m_recvEvents & EPOLLOUT)
    {
        if (m_writeCallback)
        {
            m_writeCallback();
        }
    }
}