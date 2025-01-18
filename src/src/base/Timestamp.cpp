#include "base/Timestamp.h"
#include <ctime>
#include <iomanip>
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
    char buffer[128] = {0};
    // 获取当前时间的time_t表示
    std::time_t currentTime = std::time(nullptr);

    // 利用localtime将time_t转换为tm结构体
    std::tm *localTime = std::localtime(&currentTime);

    // 设定所需的时间格式
    const char *timeFormat = "%a, %d %b %Y %H:%M:%S GMT";

    // 使用strftime根据指定格式将时间格式化
    std::strftime(buffer, sizeof(buffer), timeFormat, localTime);

    return buffer;
}