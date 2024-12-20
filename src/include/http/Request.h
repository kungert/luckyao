#pragma once
#include <string>

#include "HttpMessage.h"

namespace luckyao
{
    class Request : public HttpMessage
    {
    public:
        Request();

        Request(std::string sData);

        Request(char *pData, unsigned int len);

        Request(ByteBuffer *byteBuffer);

        virtual ~Request();

        virtual void prepare();

        virtual bool parse();

        // Helper functions

        int methodStrToInt(std::string name);

        std::string methodIntToStr(unsigned int mid);

        // Info getters  & setters

        void setMethod(int m)
        {
            m_method = m;
        }

        int getMethod()
        {
            return m_method;
        }

        void setRequestUri(std::string u)
        {
            m_requestUri = u;
        }

        std::string getRequestUri() const
        {
            return m_requestUri;
        }

    protected:
        virtual void init();

    private:
        int m_method;
        std::string m_requestUri;
    };
}