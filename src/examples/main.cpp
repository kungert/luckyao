#include <iostream>
#include "Timestamp.h"
#include "Logger.h"
int main()
{
    std::cout << "Hello World:" << luckyao::Timestamp::now().toString() << std::endl;
    LOG_SET_FILE("test.log");
    LOG_INFO("%s", "Hello Debug");
    return 0;
}