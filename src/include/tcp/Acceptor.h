#pragma once
#include "base/Uncopyable.h"
#include "base/Socket.h"
#include "loop/Channel.h"

#include <functional>
namespace luckyao
{

    class EventLoop;
    class InetAddress;

    class Acceptor : Uncopyable
    {
    public:
        using NewConnectionCallback = std::function<void(int sockfd, const InetAddress &)>;
        Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
        ~Acceptor();

        void setNewConnectionCallback(const NewConnectionCallback &cb)
        {
            m_newConnectionCallback = cb;
        }

        bool listenning() const { return m_listenning; }
        void listen();

    private:
        void handleRead();

        EventLoop *m_loop; // Acceptor用的就是用户定义的那个baseLoop，也称作mainLoop
        Socket m_acceptSocket;
        Channel m_acceptChannel;
        NewConnectionCallback m_newConnectionCallback;
        bool m_listenning;
    };
} // namespace luckyao
