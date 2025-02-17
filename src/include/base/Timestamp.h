#pragma once

#include <iostream>
#include <string>

namespace luckyao
{ // 时间类
    class Timestamp
    {
    public:
        Timestamp();
        explicit Timestamp(int64_t microSecondsSinceEpoch);
        static Timestamp now();
        std::string toString() const;
        std::string toString(std::string format) const;

    private:
        int64_t m_microsecondsSinceEpoch;
    };
}