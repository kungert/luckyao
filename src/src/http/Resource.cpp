#include "http/Resource.h"
#include <fstream>
#include <cstring>

using namespace luckyao;

std::string Resource::getName(std::string &path)
{
    std::string name = "";
    std::size_t slash_pos = path.find_last_of("/");
    if (slash_pos != std::string::npos)
        name = path.substr(slash_pos + 1);
    return name;
}
std::string Resource::getExtension(std::string &path)
{
    std::string ext = "";
    std::size_t ext_pos = path.find_last_of(".");
    if (ext_pos != std::string::npos)
        ext = path.substr(ext_pos + 1);
    return ext;
}
Resource::Resource(std::string loc, bool dir)
{
    m_location = loc;
    m_bDirectory = dir;
    m_mimeType = "";
    m_size = 0;
    m_data = NULL;
}

Resource::~Resource()
{
    if (m_data != NULL)
    {
        delete[] m_data;
        m_data = NULL;
    }
}
bool Resource::readFile()
{
    std::ifstream file(m_location, std::ios::in);
    if (!file.is_open())
    {
        return false;
    }
    // 指针定位到文件末尾
    file.seekg(0, std::ios::end);
    m_size = file.tellg();
    // 指定定位到文件开始
    file.seekg(0, std::ios::beg);

    m_data = new char[m_size];
    memset(m_data, '\0', m_size);
    file.read((char *)m_data, m_size);
    return true;
}