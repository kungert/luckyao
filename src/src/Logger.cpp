#include "Logger.h"
#include "Timestamp.h"

#include <iostream>
#include <sstream>

using namespace luckyao;
using namespace std;

// 获取日志唯一的实例对象
Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}
Logger::Logger()
    : m_outTypes(LOG_OUT_STDOUT), m_max(1024 * 1024 * 5) // 5M
{
}

Logger::~Logger()
{
    close();
}
// 设置日志级别
void Logger::setLogLevel(int level)
{
    m_logLevel = level;
}

// 写日志  [级别信息] time : msg
void Logger::log(std::string msg)
{
    ostringstream os;
    switch (m_logLevel)
    {
    case INFO:
        os << "[INFO ] ";
        // cout << "\033[33m";
        break;
    case ERROR:
        os << "[ERROR] ";
        // cout << "\033[31m";
        break;
    case FATAL:
        os << "[FATAL] ";
        // cout << "\033[31m";
        break;
    case DEBUG:
        os << "[DEBUG] ";
        // cout << "\033[32m\033[1m";
        break;
    default:
        break;
    }
    // 打印时间和msg
    os << Timestamp::now().toString() << " : " << msg << std::endl;

    // 打印时间和msg
    if ((m_outTypes & LOG_OUT_STDOUT) == LOG_OUT_STDOUT)
        cout << os.str();

    cout << os.str();
    // cout << "\033[0m";

    if ((m_outTypes & LOG_OUT_FILE) == LOG_OUT_FILE)
    {
        if (m_fout.fail())
        {
            throw std::logic_error("open log file failed: " + m_filename);
        }
        m_fout << os.str();
        m_fout.flush();
        m_len += os.str().length();

        if (m_max > 0 && m_len >= m_max)
        {
            rotate();
        }
    }
}
void Logger::open(const string &filename)
{
    m_filename = filename;
    enableOutFile();
    m_fout.open(filename.c_str(), ios::app);
    if (m_fout.fail())
    {
        throw std::logic_error("open log file failed: " + filename);
    }
    m_fout.seekp(0, ios::end);
    m_len = m_fout.tellp();
}

void Logger::close()
{
    m_fout.close();
}
void Logger::rotate()
{
    close();
    string filename = m_filename + Timestamp::now().toString(".%4d-%02d-%02d_%02d-%02d-%02d.%03d");
    if (rename(m_filename.c_str(), filename.c_str()) != 0)
    {
        throw std::logic_error("rename log file failed: ");
    }
    open(m_filename);
}