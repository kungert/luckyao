#include "http/Request.h"
#include "base/Logger.h"

using namespace luckyao;

Request::Request()
    : HttpMessage()
{
    this->init();
}

Request::Request(std::string sData)
    : HttpMessage(sData)
{
    this->init();
}

Request::Request(char *pData, unsigned int len)
    : HttpMessage(pData, len)
{
    this->init();
}
Request::Request(ByteBuffer *byteBuffer)
    : HttpMessage(byteBuffer)
{
    this->init();
}

Request::~Request()
{
}

void Request::init()
{
    m_method = 0;
    m_requestUri = "";
}

int Request::methodStrToInt(std::string name)
{
    // Method name cannot must be between 1 and 10 characters. Anything outside those bounds shouldn't be compared at all
    if (name.empty() || (name.size() >= 10))
        return -1;

    // Loop through requestMethodStr array and attempt to match the 'name' with a known m_method in the array
    int ret = -1;
    for (unsigned int i = 0; i < NUM_METHODS; i++)
    {
        if (strcmp(requestMethodStr[i], name.c_str()) == 0)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

std::string Request::methodIntToStr(unsigned int mid)
{
    // ID is out of bounds of the possible requestMethodStr indexes
    if (mid >= NUM_METHODS)
        return "";

    // Return the std::string matching the id
    return requestMethodStr[mid];
}

void Request::prepare()
{
    // Clear the bytebuffer in the event this isn't the first call of create()
    clear();

    // Insert the initial line: <m_method> <path> <version>\r\n
    std::string mstr = "";
    mstr = methodIntToStr(m_method);
    if (mstr.empty())
    {
        // std::cout << "Could not create Request, unknown m_method id: " << m_method << std::endl;
        LOG_ERROR("Could not create Request, unknown m_method id: %d", m_method);

        return;
    }
    putLine(mstr + " " + m_requestUri + " " + m_version);

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

bool Request::parse()
{
    std::string initial = "", methodName = "";

    // Get elements from the initial line: <m_method> <path> <version>\r\n
    methodName = getStrElement();
    m_requestUri = getStrElement();
    m_version = getLine(); // End of the line, pull till \r\n

    // Convert the name to the internal enumeration number
    m_method = methodStrToInt(methodName);
    if (m_method == -1)
    {
        m_parseErrorStr = "Invalid Method: " + methodName;
        return false;
    }

    // Parse and populate the headers map using the parseHeaders helper
    parseHeaders();

    // Only POST and PUT can have Content (data after headers)
    if ((m_method != POST) && (m_method != PUT))
        return true;

    // Parse the body of the message
    if (!parseBody())
        return false;

    return true;
}