
#include "base/SysApi.h"
#include "base/Stringx.h"
#include <unistd.h>
using namespace luckyao;
using namespace std;

string SysApi::getCurrentPath()
{
    std::string path;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr)
    {
        path = cwd;
    }
    else
    {
        perror("getcwd() 错误");
    }
    return path;
}