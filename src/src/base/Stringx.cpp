/*
 * @Author: kunger
 * @Date: 2023-06-15 16:22:23
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-15 16:34:01
 */

#include "Stringx.h"
#include <algorithm>
#include <stdarg.h>
#include <iterator>
#include <string.h>
#include <fstream>

using namespace std;
using namespace luckyao;

bool Stringx::cutString(const std::string &src, std::string &sub1, std::string &sub2, const std::string &seq)
{
    auto pos = src.find(seq);
    if (pos != std::string::npos)
    {
        sub1 = src.substr(0, pos);
        sub2 = src.substr(pos + seq.size());
        return true;
    }
    else
        return false;
}

string Stringx::lower(const string &input)
{
    string str = input;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string Stringx::upper(const string &input)
{
    string str = input;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

char Stringx::toChar(const string &input)
{
    char c = 0;
    stringstream ss;
    ss << input;
    ss >> c;
    return c;
}

short Stringx::toShort(const string &input)
{
    short s = 0;
    stringstream ss;
    ss << input;
    ss >> s;
    return s;
}

int Stringx::toInt(const string &input)
{
    // return atoi(input.c_str());
    int i = 0;
    stringstream ss;
    ss << input;
    ss >> i;
    return i;
}

long Stringx::toLong(const string &input)
{
    // return atol(input.c_str());
    long l = 0;
    stringstream ss;
    ss << input;
    ss >> l;
    return l;
}

float Stringx::toFloat(const string &input)
{
    float f = 0.0;
    stringstream ss;
    ss << input;
    ss >> f;
    return f;
}

double Stringx::toDouble(const string &input)
{
    // return atof(input.c_str());
    double d = 0.0;
    stringstream ss;
    ss << input;
    ss >> d;
    return d;
}

string Stringx::toString(char c)
{
    ostringstream os;
    os << c;
    return os.str();
}

string Stringx::toString(short s)
{
    ostringstream os;
    os << s;
    return os.str();
}

string Stringx::toString(int i)
{
    ostringstream os;
    os << i;
    return os.str();
}

string Stringx::toString(long l)
{
    ostringstream os;
    os << l;
    return os.str();
}

string Stringx::toString(float f)
{
    ostringstream os;
    os << f;
    return os.str();
}

string Stringx::toString(double d)
{
    ostringstream os;
    os << d;
    return os.str();
}

std::vector<std::string> Stringx::toVector(const std::string str, char flag)
{
    vector<string> rs;
    std::istringstream iss(str);
    string temp;

    while (std::getline(iss, temp, flag))
    {
        rs.push_back(temp);
    }
    return rs;
}

string Stringx::trimStart(const string &input)
{
    return trimStart(input, " \r\n");
}

string Stringx::trimStart(const string &input, char trim)
{
    string str;
    str = trim;
    return trimStart(input, str.c_str());
}

string Stringx::trimStart(const string &input, const char *trims)
{
    string delimiter = trims;
    string str = input;
    size_t found;
    found = str.find_first_not_of(delimiter);
    if (found != string::npos)
        str.erase(0, found);
    else
        str.clear();
    return str;
}

string Stringx::trimEnd(const string &input)
{
    return trimEnd(input, " \r\n");
}

string Stringx::trimEnd(const string &input, char delim)
{
    string str;
    str = delim;
    return trimEnd(input, str.c_str());
}

string Stringx::trimEnd(const string &input, const char *delims)
{
    string delimiter = delims;
    string str = input;
    size_t found;
    found = str.find_last_not_of(delimiter);
    if (found != string::npos)
        str.erase(found + 1);
    else
        str.clear();
    return str;
}

string Stringx::trim(const string &input)
{
    return trim(input, " \r\n");
}

string Stringx::trim(const string &input, char trim)
{
    string str;
    str = trim;
    return Stringx::trim(input, str.c_str());
}

string Stringx::trim(const string &input, const char *trims)
{
    string str = trimStart(input, trims);
    return trimEnd(str, trims);
}

void Stringx::split(vector<string> &output, const string &input)
{
    output.clear();
    istringstream iss(input);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(output));
}

void Stringx::split(vector<string> &output, const string &input, char separator)
{
    output.clear();
    stringstream ss(input);
    string item;
    while (getline(ss, item, separator))
    {
        output.push_back(item);
    }
}

void Stringx::split(vector<string> &output, const string &input, const string &separators)
{
    output.clear();
    size_t last = 0;
    size_t index = input.find_first_of(separators, last);
    while (index != string::npos)
    {
        string str = input.substr(last, index - last);
        output.push_back(str);
        last = index + 1;
        index = input.find_first_of(separators, last);
    }
    if (index - last > 0)
    {
        output.push_back(input.substr(last, index - last));
    }
}

string Stringx::join(vector<string> &input)
{
    return join(input, ' ');
}

string Stringx::join(vector<string> &input, char separator)
{
    ostringstream os;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (i != 0)
            os << separator;
        os << input[i];
    }
    return os.str();
}

string Stringx::join(vector<string> &input, const char *separators)
{
    ostringstream os;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (i != 0)
            os << separators;
        os << input[i];
    }
    return os.str();
}

string Stringx::capitalize(const string &input)
{
    ostringstream os;
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        if (i == 0)
        {
            if (input[i] >= 97 && input[i] <= 122)
            {
                os << (char)(input[i] - 32);
            }
            else
            {
                os << input[i];
            }
        }
        else
        {
            os << input[i];
        }
    }
    return os.str();
}

int Stringx::compare(const string &strA, const string &strB, bool ignoreCase)
{
    if (ignoreCase)
    {
#ifdef WIN32
        return _stricmp(strA.c_str(), strB.c_str());
#else
        return strcasecmp(strA.c_str(), strB.c_str());
#endif
    }
    else
    {
        return strA.compare(strB);
    }
}

string Stringx::format(const char *format, ...)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(buf, sizeof(buf), format, arg_ptr);
    va_end(arg_ptr);
    return string(buf);
}

bool Stringx::isNumeric(const string &input)
{
    if (input.find_first_not_of("0123456789.") != string::npos)
        return false;
    if (count(input.begin(), input.end(), '.') > 1)
        return false;
    return true;
}

#define ADJUST_INDICES(start, end, len) \
    if (end > len)                      \
        end = len;                      \
    else if (end < 0)                   \
    {                                   \
        end += len;                     \
        if (end < 0)                    \
            end = 0;                    \
    }                                   \
    if (start < 0)                      \
    {                                   \
        start += len;                   \
        if (start < 0)                  \
            start = 0;                  \
    }

// 匹配函数：endswith与startwith的内部调用函数
int _string_tailmatch(const std::string &self, const std::string &substr, int start, int end, int direction)
{
    int selflen = (int)self.size();
    int slen = (int)substr.size();

    const char *str = self.c_str();
    const char *sub = substr.c_str();

    // 对输入的范围进行校准
    ADJUST_INDICES(start, end, selflen);

    // 字符串头部匹配（即startswith）
    if (direction < 0)
    {
        if (start + slen > selflen)
            return 0;
    }
    // 字符串尾部匹配（即endswith）
    else
    {
        if (end - start < slen || start > selflen)
            return 0;
        if (end - slen > start)
            start = end - slen;
    }
    if (end - start >= slen)
        // mcmcmp函数用于比较buf1与buf2的前n个字节
        return memcmp(str + start, sub, slen) == 0;
    return 0;
}
bool Stringx::endswith(const std::string &str, const std::string &suffix, int start, int end)
{
    // 调用＿string＿tailmatch函数，参数+1表示字符串尾部匹配
    int result = _string_tailmatch(str, suffix, start, end, +1);
    return static_cast<bool>(result);
}
bool Stringx::startswith(const std::string &str, const std::string &suffix, int start, int end)
{
    // 调用＿string＿tailmatch函数，参数-1表示字符串头部匹配
    int result = _string_tailmatch(str, suffix, start, end, -1);
    return static_cast<bool>(result);
}

void Stringx::replace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();

    while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}
bool Stringx::isInVector(vector<string> strs, const string value)
{
    auto iter = std::find(strs.begin(), strs.end(), value);
    if (iter == strs.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool Stringx::isSubInVector(std::vector<std::string> strs, const std::string value)
{
    bool rs = false;
    size_t len = strs.size();
    for (size_t i = 0; i < len; i++)
    {
        size_t vlen = value.size();
        string key = strs[i];
        size_t klen = key.size();
        if (vlen <= klen)
            continue;
        else
        {
            if (value.substr(0, klen).compare(key) == 0)
            {
                rs = true;
                break;
            }
        }
    }
    return rs;
}
bool Stringx::readFile2OStr(const string &path, std::ostringstream &os)
{
    ifstream input_file(path);
    if (!input_file.is_open())
    {
        return false;
    }
    os << input_file.rdbuf();
    return true;
}