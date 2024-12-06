#include <iostream>
#include "Timestamp.h"
int main()
{
    std::cout << "Hello World:" << luckyao::Timestamp::now().toString() << std::endl;
    return 0;
}