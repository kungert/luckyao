#pragma once

#include "HttpMessage.h"

namespace luckyao
{
    // Http响应类
    class Response : public HttpMessage
    {
    public:
        Response();

        Response(std::string sData);

        Response(char *pData, unsigned int len);

        virtual ~Response();

        virtual void prepare();

        virtual bool parse();

        // Accessors & Mutators

        void json(std::string data);

        void html(std::string data);

        void setStatus(int scode)
        {
            m_status = scode;
            determineReasonStr();
        }

        std::string getReason()
        {
            return m_reason;
        }

    protected:
        virtual void init();

    private:
        void determineReasonStr();
        void determineStatusCode();

    private:
        // Response variables
        int m_status;
        std::string m_reason;
    };
}