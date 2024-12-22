#pragma once

#include <string>
#include <fstream>
namespace luckyao
{
    enum LogLevel
    {
        DEBUG, // 调试信息
        INFO,  // 普通信息
        ERROR, // 错误信息
        FATAL, // core信息
    };
    /**
     *  日志输出目的地
     */
    enum LOG_OUT_TYPE
    {
        LOG_OUT_STDOUT = 0x001,
#define LOG_OUT_STDOUT LOG_OUT_STDOUT
        LOG_OUT_FILE = 0x002,
#define LOG_OUT_FILE LOG_OUT_FILE
    };
    class Logger
    {
    public:
        // 获取日志唯一的实例对象
        static Logger &instance();
        // 设置日志级别
        void setLogLevel(int level);
        // 写日志
        void log(std::string msg);

        void open(const std::string &filename);
        void close();
        void enableOutFile() { m_outTypes |= LOG_OUT_FILE; };
        void rotate();

    private:
        Logger();
        ~Logger();
        void keepDirFileCount(std::size_t count = 4);

    private:
        int m_logLevel;

        // file
        std::string m_filename;
        std::ofstream m_fout;
        int m_max;
        int m_len;
        int m_outTypes;
    };
// LOG_INFO("%s %d", arg1, arg2)
#define LOG_INFO(logmsgFormat, ...)                            \
    do                                                         \
    {                                                          \
        luckyao::Logger &logger = luckyao::Logger::instance(); \
        logger.setLogLevel(luckyao::LogLevel::INFO);           \
        char buf[1024] = {0};                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);      \
        logger.log(buf);                                       \
    } while (0)

#define LOG_ERROR(logmsgFormat, ...)                           \
    do                                                         \
    {                                                          \
        luckyao::Logger &logger = luckyao::Logger::instance(); \
        logger.setLogLevel(luckyao::LogLevel::ERROR);          \
        char buf[1024] = {0};                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);      \
        logger.log(buf);                                       \
    } while (0)

#define LOG_FATAL(logmsgFormat, ...)                           \
    do                                                         \
    {                                                          \
        luckyao::Logger &logger = luckyao::Logger::instance(); \
        logger.setLogLevel(luckyao::LogLevel::FATAL);          \
        char buf[1024] = {0};                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);      \
        logger.log(buf);                                       \
        exit(-1);                                              \
    } while (0)

#define MUDEBUG
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                           \
    do                                                         \
    {                                                          \
        luckyao::Logger &logger = luckyao::Logger::instance(); \
        logger.setLogLevel(luckyao::LogLevel::DEBUG);          \
        char buf[1024] = {0};                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);      \
        logger.log(buf);                                       \
    } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif
#define LOG_SET_FILE(fileName)                                 \
    do                                                         \
    {                                                          \
        luckyao::Logger &logger = luckyao::Logger::instance(); \
        logger.enableOutFile();                                \
        logger.open(fileName);                                 \
    } while (0)
}
