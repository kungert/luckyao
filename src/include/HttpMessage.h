/*
 * @Author: kunger
 * @Date: 2023-06-09 01:06:46
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-09 01:09:49
 */
#pragma once

#include <map>
#include <string>
#include <sstream>

#include "ByteBuffer.h"
// Constants
#define HTTP_VERSION_10 "HTTP/1.0"
#define HTTP_VERSION_11 "HTTP/1.1"
#define DEFAULT_HTTP_VERSION HTTP_VERSION_11
#define NUM_METHODS 9

namespace luckyao
{
    // HTTP Methods (Requests)

    enum Method
    {
        HEAD = 0,
        GET = 1,
        POST = 2,
        PUT = 3,
        DEL = 4, // DELETE is taken, use DEL instead
        TRACE = 5,
        OPTIONS = 6,
        CONNECT = 7,
        PATCH = 8
    };

    const static char *const requestMethodStr[NUM_METHODS] = {
        "HEAD",    // 0
        "GET",     // 1
        "POST",    // 2
        "PUT",     // 3
        "DELETE",  // 4
        "TRACE",   // 5
        "OPTIONS", // 6
        "CONNECT", // 7
        "PATCH"    // 8
    };

    // HTTP Response Status codes
    enum Status
    {
        // 1xx Informational
        CONTINUE = 100,

        // 2xx Success
        OK = 200,

        // 3xx Redirection

        // 4xx Client Error
        BAD_REQUEST = 400,
        NOT_FOUND = 404,

        // 5xx Server Error
        SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501
    };

    class HttpMessage : public ByteBuffer
    {
    public:
        HttpMessage();

        HttpMessage(std::string sData);

        HttpMessage(char *pData, unsigned int len);

        HttpMessage(ByteBuffer *byteBuffer);

        virtual ~HttpMessage();

        virtual void prepare() = 0;

        virtual bool parse() = 0;

        // Create helpers
        void putLine(std::string str = "", bool crlf_end = true);

        void putHeaders();

        // Parse helpers
        std::string getLine();

        std::string getStrElement(char delim = 0x20); // 0x20 = "space"

        void parseHeaders();

        bool parseBody();

        // Header Map manipulation
        void addHeader(std::string line);

        void addHeader(std::string key, std::string value);

        void addHeader(std::string key, int value);

        std::string getHeaderValue(std::string key);

        std::string getHeaderStr(int index);

        int getNumHeaders();

        void clearHeaders();

        // Getters & Setters

        std::string getParseError()
        {
            return m_parseErrorStr;
        }

        void setVersion(std::string v)

        {
            m_version = v;
        }

        std::string getVersion()
        {
            return m_version;
        }

        void setData(char *d, unsigned int len)
        {
            m_data = d;
            m_dataLen = len;
        }

        char *getData()
        {
            return m_data;
        }

        unsigned int getDataLength()
        {
            return m_dataLen;
        }

    protected:
        virtual void init();

    private:
        std::map<std::string, std::string> *m_headers;

    protected:
        std::string m_parseErrorStr;

        std::string m_version;

        // Message Body Data (Resource in the case of a response, extra parameters in the case of a request)
        char *m_data;
        unsigned int m_dataLen;
    };
} // namespace http
