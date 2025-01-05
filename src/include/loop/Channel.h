#pragma once

#include "base/Timestamp.h"
#include "base/Uncopyable.h"
#include <functional>
#include <memory>

namespace luckyao
{
    class EventLoop;
    class Channel : Uncopyable
    {
    public:
        using EventCallback = std::function<void()>;
        using ReadEventCallback = std::function<void(Timestamp)>;

        Channel(EventLoop *loop, int fd);
        ~Channel();
        void handleEvent(Timestamp receiveTime);

        // 设置回调函数对象
        void setReadCallback(ReadEventCallback cb) { m_readCallback = std::move(cb); }
        void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
        void setCloseCallback(EventCallback cb) { m_closeCallback = std::move(cb); }
        void setErrorCallback(EventCallback cb) { m_errorCallback = std::move(cb); }

        // 防止当channel被手动remove掉，channel还在执行回调操作
        void tie(const std::shared_ptr<void> &);

        int fd() const { return m_fd; }
        int events() const { return m_events; }
        void setRecvEvents(int revt) { m_recvEvents = revt; }

        // 设置fd相应的事件状态
        void enableReading()
        {
            m_events |= kReadEvent;
            update();
        }
        void disableReading()
        {
            m_events &= ~kReadEvent;
            update();
        }
        void enableWriting()
        {
            m_events |= kWriteEvent;
            update();
        }
        void disableWriting()
        {
            m_events &= ~kWriteEvent;
            update();
        }
        void disableAll()
        {
            m_events = kNoneEvent;
            update();
        }

        // 返回fd当前的事件状态
        bool isNoneEvent() const { return m_events == kNoneEvent; }
        bool isWriting() const { return m_events & kWriteEvent; }
        bool isReading() const { return m_events & kReadEvent; }

        int index() { return m_index; }
        void setIndex(int idx) { m_index = idx; }

        // one loop per thread
        EventLoop *ownerLoop() { return m_loop; }
        void remove();

    private:
        void update();
        void handleEventWithGuard(Timestamp receiveTime);

        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvent;

        EventLoop *m_loop; // 事件循环
        const int m_fd;    // fd, Poller监听的对象
        int m_events;      // 注册fd感兴趣的事件
        int m_recvEvents;  // poller返回的具体发生的事件
        int m_index;

        std::weak_ptr<void> m_tie;
        bool m_tied;

        // 因为channel通道里面能够获知fd最终发生的具体的事件revents，所以它负责调用具体事件的回调操作
        ReadEventCallback m_readCallback;
        EventCallback m_writeCallback;
        EventCallback m_closeCallback;
        EventCallback m_errorCallback;
    };
}