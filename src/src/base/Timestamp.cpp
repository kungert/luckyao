#include "base/Timestamp.h"
#include <time.h>
using namespace luckyao;

Timestamp::Timestamp() : m_microsecondsSinceEpoch(0) {}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
    : m_microsecondsSinceEpoch(microSecondsSinceEpoch)
{
}
Timestamp Timestamp::now()
{
    return Timestamp(time(NULL));
}
std::string Timestamp::toString() const
{
    char buf[128] = {0};
    tm *tm_time = localtime(&m_microsecondsSinceEpoch);
    // 获取秒数和毫秒数
    int milliseconds = m_microsecondsSinceEpoch % 1000;
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d.%03d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday,
             tm_time->tm_hour,
             tm_time->tm_min,
             tm_time->tm_sec,
             milliseconds);
    return buf;
}
std::string Timestamp::toString(std::string format) const
{
    char buf[128] = {0};
    tm *tm_time = localtime(&m_microsecondsSinceEpoch);

    // 获取秒数和毫秒数
    int milliseconds = m_microsecondsSinceEpoch % 1000;
    // snprintf(buf, 128, format.c_str(),
    //          tm_time->tm_year + 1900,
    //          tm_time->tm_mon + 1,
    //          tm_time->tm_mday,
    //          tm_time->tm_hour,
    //          tm_time->tm_min,
    //          tm_time->tm_sec,
    //          milliseconds);

    strftime(buf, 128, "%a, %d %b %Y %H:%M:%S GMT", tm_time);
    return buf;
}