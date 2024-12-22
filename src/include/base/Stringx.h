/*
 * @Author: kunger
 * @Date: 2023-06-15 16:19:24
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-26 10:50:23
 */
#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <limits.h>

namespace luckyao
{
    class Stringx
    {
    public:
        static bool cutString(const std::string &src, std::string &sub1, std::string &sub2, const std::string &seq);

        static std::string lower(const std::string &input);
        static std::string upper(const std::string &input);

        static char toChar(const std::string &input);
        static short toShort(const std::string &input);
        static int toInt(const std::string &input);
        static long toLong(const std::string &input);
        static float toFloat(const std::string &input);
        static double toDouble(const std::string &input);

        static std::string toString(char c);
        static std::string toString(short s);
        static std::string toString(int i);
        static std::string toString(long l);
        static std::string toString(float f);
        static std::string toString(double d);

        static std::vector<std::string> toVector(const std::string str, char flag = ',');

        static std::string trimStart(const std::string &input);
        static std::string trimStart(const std::string &input, char trim);
        static std::string trimStart(const std::string &input, const char *trims);

        static std::string trimEnd(const std::string &input);
        static std::string trimEnd(const std::string &input, char trim);
        static std::string trimEnd(const std::string &input, const char *trims);

        static std::string trim(const std::string &input);
        static std::string trim(const std::string &input, char trim);
        static std::string trim(const std::string &input, const char *trims);

        static void split(std::vector<std::string> &output, const std::string &input);
        static void split(std::vector<std::string> &output, const std::string &input, char separator);
        static void split(std::vector<std::string> &output, const std::string &input, const std::string &separators);

        static std::string join(std::vector<std::string> &input);
        static std::string join(std::vector<std::string> &input, char separator);
        static std::string join(std::vector<std::string> &input, const char *separators);

        static std::string capitalize(const std::string &input);

        static int compare(const std::string &strA, const std::string &strB, bool ignoreCase = false);
        static std::string format(const char *format, ...);

        static bool isNumeric(const std::string &input);

        static bool endswith(const std::string &str, const std::string &suffix, int start = 0, int end = INT_MAX);
        static bool startswith(const std::string &str, const std::string &suffix, int start = 0, int end = INT_MAX);

        static void replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);

        static bool isInVector(std::vector<std::string> strs, const std::string value);
        // vector中是否有value的子字符串
        static bool isSubInVector(std::vector<std::string> strs, const std::string value);

        static bool readFile2OStr(const std::string &path, std::ostringstream &os);
    };
}