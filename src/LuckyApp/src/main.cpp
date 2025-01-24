#include <iostream>
#include "HtmlServer.h"
#include "loop/EventLoop.h"
#include "base/Singleton.h"
using namespace luckyao;

int main()
{
    int ret = system("tar -zxvf 11.tar.gz"); // 执行ls命令

    if (ret == -1)
    {
        printf("Failed to execute command\n");
    }
    else
    {
        if (WIFEXITED(ret))
        { // 检查命令是否成功结束
            if (WEXITSTATUS(ret) == 0)
            {
                printf("Command executed successfully\n");
            }
            else
            {
                printf("Command exited with status %d\n", WEXITSTATUS(ret));
            }
        }
        else
        {
            printf("Command terminated by signal %d\n", WTERMSIG(ret));
        }
    }
    return 0;

    EventLoop loop;
    Singleton<HtmlServer>::instance()->start(&loop);
    loop.loop();
    return 0;
}