#include "Timestamp.h"
Timestamp::Timestamp() : m_microseconds_since_epoch(0) {}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
    : m_microseconds_since_epoch(microSecondsSinceEpoch)
{
}