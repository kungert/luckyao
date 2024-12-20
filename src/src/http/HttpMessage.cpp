#include "HttpMessage.h"
#include "Logger.h"
#define DEFAULT_BUFFER_SIZE 1024

using namespace luckyao;

HttpMessage::HttpMessage()
    : ByteBuffer(DEFAULT_BUFFER_SIZE)
{
    this->init();
}

HttpMessage::HttpMessage(std::string sData)
    : ByteBuffer(sData.size() + 1)
{
    putBytes((char *)sData.c_str(), sData.size() + 1);
    this->init();
}

HttpMessage::HttpMessage(char *pData, unsigned int len)
    : ByteBuffer(pData, len)
{
    this->init();
}

HttpMessage::HttpMessage(ByteBuffer *byteBuffer)
    : ByteBuffer(byteBuffer)
{
    this->init();
}
HttpMessage::~HttpMessage()
{
    m_headers->clear();
    delete m_headers;
}

void HttpMessage::init()
{
    m_parseErrorStr = "";

    m_data = NULL;
    m_dataLen = 0;

    m_version = DEFAULT_HTTP_VERSION;

    m_headers = new std::map<std::string, std::string>();
}

void HttpMessage::putLine(std::string str, bool crlf_end)
{
    // Terminate with crlf if flag set
    if (crlf_end)
        str += "\r\n";

    // Put the entire contents of str into the buffer
    putBytes((char *)str.c_str(), str.size());
}

void HttpMessage::putHeaders()
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = m_headers->begin(); it != m_headers->end(); it++)
    {
        std::string final = it->first + ": " + it->second;
        putLine(final, true);
    }

    // End with a blank line
    putLine();
}

std::string HttpMessage::getLine()
{
    std::string ret = "";
    int startPos = getReadPos();
    bool newLineReached = false;
    char c = 0;

    // Append characters to the return std::string until we hit the end of the buffer, a CR (13) or LF (10)
    for (unsigned int i = startPos; i < size(); i++)
    {
        // If the next char is a \r or \n, we've reached the end of the line and should break out of the loop
        c = *peek();
        if ((c == 13) || (c == 10))
        {
            newLineReached = true;
            break;
        }

        // Otherwise, append the next character to the std::string
        ret += getChar();
    }

    // If a line termination was never reached, discard the result and conclude there are no more lines to parse
    if (!newLineReached)
    {
        setReadPos(startPos); // Reset the position to before the last line read that we are now discarding
        ret = "";
        return ret;
    }

    // Increment the read position until the end of a CR or LF chain, so the read position will then point to the next line
    // Also, only read a maximum of 2 characters so as to not skip a blank line that is only \r\n
    unsigned int k = 0;
    for (unsigned int i = getReadPos(); i < size(); i++)
    {
        if (k++ >= 2)
            break;
        c = getChar();
        if ((c != 13) && (c != 10))
        {
            // Set the Read position back one because the retrived character wasn't a LF or CR
            setReadPos(getReadPos() - 1);
            break;
        }
    }

    return ret;
}

std::string HttpMessage::getStrElement(char delim)
{
    std::string ret = "";
    int startPos = getReadPos();
    unsigned int size = 0;
    int endPos = find(delim, startPos);

    // Calculate the size based on the found ending position
    size = (endPos + 1) - startPos;

    if ((endPos == -1) || (size <= 0))
        return "";

    // Grab the std::string from the char buffer up to the delimiter
    char *str = new char[size];
    memset(str, '\0', size);
    getBytes((char *)str, size);
    str[size - 1] = 0x00; // NULL termination
    ret.assign(str);
    delete[] str;

    // Increment the read position PAST the delimiter
    setReadPos(endPos + 1);

    return ret;
}

void HttpMessage::parseHeaders()
{
    std::string hline = "", app = "";

    // Get the first header
    hline = getLine();

    // Keep pulling m_headers until a blank line has been reached (signaling the end of m_headers)
    while (hline.size() > 0)
    {
        // Case where values are on multiple lines ending with a comma
        app = hline;
        while (app[app.size() - 1] == ',')
        {
            app = getLine();
            hline += app;
        }

        addHeader(hline);
        hline = getLine();
    }
}

bool HttpMessage::parseBody()
{
    // Content-Length should exist (size of the Body m_data) if there is body m_data
    std::string hlenstr = "";
    unsigned int contentLen = 0;
    hlenstr = getHeaderValue("Content-Length");

    // No body m_data to read:
    if (hlenstr.empty())
        return true;

    contentLen = atoi(hlenstr.c_str());

    // contentLen should NOT exceed the remaining number of bytes in the buffer
    // Add 1 to bytesRemaining so it includes the char at the current read position
    if (contentLen > bytesRemaining() + 1)
    {
        /*
        // If it exceeds, read only up to the number of bytes remaining
        m_dataLen = bytesRemaining();
        */
        // If it exceeds, there's a potential security issue and we can't reliably parse
        std::stringstream pes;
        pes << "Content-Length (" << hlenstr << ") is greater than remaining bytes (" << bytesRemaining() << ")";
        m_parseErrorStr = pes.str();
        return false;
    }
    else
    {
        // Otherwise, we ca probably trust Content-Length is valid and read the specificed number of bytes
        m_dataLen = contentLen;
    }

    // Create a big enough buffer to store the m_data
    unsigned int dIdx = 0, s = size();
    m_data = new char[m_dataLen];

    // Grab all the bytes from the current position to the end
    for (unsigned int i = getReadPos(); i < s; i++)
    {
        m_data[dIdx] = get(i);
        dIdx++;
    }

    // TODO: Handle chuncked Request/Response parsing (with footers) here

    return true;
}

void HttpMessage::addHeader(std::string line)
{
    std::string key = "", value = "";
    size_t kpos;
    std::size_t i = 0;
    kpos = line.find(':');
    if (kpos == std::string::npos)
    {
        // std::cout << "Could not addHeader: " << line.c_str() << std::endl;
        LOG_FATAL("Could not addHeader:%s", line.c_str());

        return;
    }
    key = line.substr(0, kpos);
    value = line.substr(kpos + 1, line.size() - kpos - 1);

    // Skip all leading spaces in the value
    while (i < value.size() && value.at(i) == 0x20)
    {
        i++;
    }
    value = value.substr(i, value.size());

    // Add header to the map
    addHeader(key, value);
}

void HttpMessage::addHeader(std::string key, std::string value)
{
    m_headers->insert(std::pair<std::string, std::string>(key, value));
}

void HttpMessage::addHeader(std::string key, int value)
{
    std::stringstream sz;
    sz << value;
    m_headers->insert(std::pair<std::string, std::string>(key, sz.str()));
}

std::string HttpMessage::getHeaderValue(std::string key)
{
    char c;
    std::string key_lower = "";

    // Lookup in map
    auto it = m_headers->find(key);

    // Key wasn't found, try an all lowercase variant as some clients won't always use proper capitalization
    if (it == m_headers->end())
    {

        for (std::size_t i = 0; i < key.length(); i++)
        {
            c = key.at(i);
            key_lower += tolower(c);
        }

        // Still not found, return empty string to indicate the Header value doesnt exist
        it = m_headers->find(key_lower);
        if (it == m_headers->end())
            return "";
    }

    // Otherwise, return the value
    return it->second;
}

std::string HttpMessage::getHeaderStr(int index)
{
    int i = 0;
    std::string ret = "";
    std::map<std::string, std::string>::const_iterator it;
    for (it = m_headers->begin(); it != m_headers->end(); it++)
    {
        if (i == index)
        {
            ret = it->first + ": " + it->second;
            break;
        }

        i++;
    }
    return ret;
}

int HttpMessage::getNumHeaders()
{
    return m_headers->size();
}

void HttpMessage::clearHeaders()
{
    m_headers->clear();
}