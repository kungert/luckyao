#include <iostream>
#include "Timestamp.h"
#include "Logger.h"
#include "Thread.h"
int main()
{

    LOG_SET_FILE("test.log");
    LOG_INFO("%s", "Hello Debug");
    luckyao::Thread t([&]()
                      { std::cout << "Hello World:" << luckyao::Timestamp::now().toString() << std::endl; });
    t.start();
    // t.join();
    return 0;
}