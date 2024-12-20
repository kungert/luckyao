#include "Response.h"

using namespace luckyao;

Response::Response()
    : HttpMessage()
{
    this->init();
}

Response::Response(std::string sData)
    : HttpMessage(sData)
{
    this->init();
}

Response::Response(char *pData, unsigned int len)
    : HttpMessage(pData, len)
{
    this->init();
}

Response::~Response()
{
}

void Response::init()
{
    m_status = 0;
    m_reason = "";
}

void Response::determineStatusCode()
{
    if (m_reason.find("Continue") != std::string::npos)
    {
        m_status = Status(CONTINUE);
    }
    else if (m_reason.find("OK") != std::string::npos)
    {
        m_status = Status(OK);
    }
    else if (m_reason.find("Bad Request") != std::string::npos)
    {
        m_status = Status(BAD_REQUEST);
    }
    else if (m_reason.find("Not Found") != std::string::npos)
    {
        m_status = Status(NOT_FOUND);
    }
    else if (m_reason.find("Server Error") != std::string::npos)
    {
        m_status = Status(SERVER_ERROR);
    }
    else if (m_reason.find("Not Implemented") != std::string::npos)
    {
        m_status = Status(NOT_IMPLEMENTED);
    }
    else
    {
    }
}

void Response::determineReasonStr()
{
    switch (m_status)
    {
    case Status(CONTINUE):
        m_reason = "Continue";
        break;
    case Status(OK):
        m_reason = "OK";
        break;
    case Status(BAD_REQUEST):
        m_reason = "Bad Request";
        break;
    case Status(NOT_FOUND):
        m_reason = "Not Found";
        break;
    case Status(SERVER_ERROR):
        m_reason = "Internal Server Error";
        break;
    case Status(NOT_IMPLEMENTED):
        m_reason = "Not Implemented";
        break;
    default:
        break;
    }
}

void Response::prepare()
{
    // Clear the bytebuffer in the event this isn't the first call of create()
    clear();

    // Insert the m_status line: <version> <m_status code> <m_reason>\r\n
    std::stringstream sline;
    sline << m_version << " " << m_status << " " << m_reason;
    putLine(sline.str());

    // Put all headers
    putHeaders();

    // If theres body data, add it now
    if ((m_data != NULL) && m_dataLen > 0)
    {
        putBytes(m_data, m_dataLen);
    }

    // Allocate space for the returned byte array and return it
    // byte* createRetData = new byte[size()];
    setReadPos(0);
    // getBytes(createRetData, size());

    // return createRetData;
}

bool Response::parse()
{
    std::string statusstr;

    // Get elements from the m_status line: <version> <m_status code> <m_reason>\r\n
    m_version = getStrElement();
    statusstr = getStrElement();
    determineStatusCode();
    m_reason = getLine(); // Pull till \r\n termination

    // Parse and populate the headers map using the parseHeaders helper
    parseHeaders();

    // If the body of the message
    if (!parseBody())
        return false;

    return true;
}