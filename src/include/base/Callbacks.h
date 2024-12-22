#pragma once

#include <memory>
#include <functional>

namespace luckyao
{

    class ByteBuffer;
    class TcpConnection;
    class Timestamp;

    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
    using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
    using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
    using MessageCallback = std::function<void(const TcpConnectionPtr &,
                                               ByteBuffer &,
                                               Timestamp)>;
    using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr &, size_t)>;
} // namespace name