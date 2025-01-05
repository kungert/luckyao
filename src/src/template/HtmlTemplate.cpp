#include "template/HtmlTemplate.h"
#include "exception/FileException.h"
#include "template/TemplateParseException.h"
#include "template/TemplateNotFoundException.h"
#include <memory>

using std::cerr;
using std::pair;
using std::regex;
using std::smatch;
using std::stack;
using std::string;
using std::stringstream;

long get_file_size(FILE *f)
{
    long size = -1;
    long cur = ftell(f);
    if (f != nullptr)
    {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, cur, SEEK_SET);
    }
    return size;
}
string str_format(const char *format, ...)
{
    size_t len = strlen(format) + 200;
    char *szBuffer = new char[len];
    memset(szBuffer, 0x00, len);
    va_list ap;
    va_start(ap, format);
    vsnprintf(szBuffer, len, format, ap);
    string s(szBuffer);
    delete[] szBuffer;
    return s;
}

bool str_isnumber(const string &str)
{
    bool is_float = false;
    for (auto &ch : str)
    {
        if (!isdigit(ch))
        {
            if (ch == '.' && !is_float)
                is_float = true;
            else
                return false;
        }
    }
    return true;
}

pair<string, string> split2(const string &data, const string &delim)
{
    size_t pos = data.find(delim);
    if (pos != string::npos)
    {
        return {data.substr(0, pos), data.substr(pos + delim.size())};
    }
    return {data, ""};
}

template <typename T>
T str_parse(const string &str)
{
    T ans;
    stringstream ss(str);
    ss >> ans;
    return ans;
}
luckyao::HtmlTemplate::HtmlTemplate(const std::string &str, int flag)
{
    if (flag == 0)
    {
        file = fopen(str.c_str(), "rb");
        if (file == nullptr)
            throwException(TemplateNotFoundException, "模板文件 " + str + " 不存在");
    }
    else
        tmpl = std::move(const_cast<string &>(str));
}
double luckyao::HtmlTemplate::calculator(vector<Json> &number, vector<char> &op)
{
    // 例如下表达式会成为
    // 1 - 2 - 3 + 2 *3 * 4 - 4*5
    // vector<char> op = { '-', '-', '+', '*', '*', '-', '*' };
    // vector<any> number = { 1, 2, 3, 2, 3, 4, 4, 5 };
    if (number.size() != op.size() + 1)
        throwException(TemplateParseException, "运算符号数和操作数不匹配");

    /* 定义计算器的内部函数 */
    auto calc = [](Json &var1, double var2, char op) -> double
    {
    // var2 + var1
    // var2 * var1
    // var2 - var1
    // var2 / var1
    // 注意顺序
#define CALC(op2)                                                 \
    if (#op2[0] == op)                                            \
    {                                                             \
        if (var1.type() == JsonType::Int)                         \
            return var2 op2 static_cast<double>(var1.asInt());    \
        else if (var1.type() == JsonType::Double)                 \
            return var2 op2 static_cast<double>(var1.asDouble()); \
    }
        CALC(+);
        CALC(-);
        CALC(*);
        CALC(/);
        throwException(TemplateParseException, "不允许对空指针进行运算");
#undef CALC
    };

    vector<double> num_stack;                     // 计算中间结果存储栈
    num_stack.push_back(calc(number[0], 0, '+')); // 获取值  number[i+1] + 0 (加法运算零元为0，乘法运算零元为1)
    /* 计算 * / 法*/
    for (size_t i = 0; i < op.size(); i++)
    {
        if (op[i] == '+' || op[i] == '-')
        {
            num_stack.push_back(calc(number[i + 1], 0, '+')); // number[i+1] + 0
        }
        else if (op[i] == '*' || op[i] == '/')
        {
            double var1 = num_stack.back();
            num_stack.pop_back();
            num_stack.push_back(calc(number[i + 1], var1, op[i])); // var1/number[i+1] 或者是  var1/number[i+1]
        }
        else
            throwException(TemplateParseException, str_format("非法操作符 %d", op[i]));
    }
    /* 计算 + - 法*/
    double result = num_stack[0];
    size_t i = 1;
    for (auto &ch : op)
    {
        if (ch == '+')
        {
            result += num_stack[i++];
        }
        else if (ch == '-')
        {
            result -= num_stack[i++];
        }
    }
    return result;
}

string luckyao::HtmlTemplate::render()
{
    long size;
    if (tmpl.empty())
    {
        size = get_file_size(file);
        if (size <= 0)
            return "";
        if (size > MAX_FILE_SIZE)
            throwException(FileException, "文件太大，无法处理");
        auto *buf = new char[size + 1];
        fread(buf, static_cast<size_t>(size), 1, file);
        buf[size] = 0;
        tmpl = buf;
        delete[] buf;
    }

    size = tmpl.size();
    /* 渲染开始 */
    ss.str(""); // 清空
    parse(0, static_cast<size_t>(size));
    return ss.str();
}

void luckyao::HtmlTemplate::parse(size_t start_pos, size_t len2)
{
    if (len2 == 0)
        return;
    smatch result;
    size_t last_pos = start_pos;
    for (size_t i = start_pos; i < start_pos + len2; i++)
    {
        if (tmpl[i] == Token::TMPL_START)
        {
            ss << tmpl.substr(last_pos, i - last_pos);

            // 开始处理
            if (tmpl[i + 1] == Token::STATEMENT_START)
            {
                // 处理语句
                size_t end_pos = tmpl.find(Token::STATEMENT_END, i + 2);
                if (tmpl[end_pos + 1] != Token::TMPL_END)
                    throwException(TemplateParseException, errorView(i) + "变量结束位置有错误");
                size_t len = end_pos + 2 - i;
                string tmp_str = tmpl.substr(i, len);
                if (regex_match(tmp_str, result, Token::IF_REGEX))
                {
                    string statement = result.str(1);
                    string::const_iterator start = tmpl.begin() + end_pos + 1;
                    string::const_iterator end = tmpl.begin() + start_pos + len2;
                    result = findNext(start, end, Token::ENDIF_REGEX, Token::IF_REGEX);
                    size_t pos1 = result[0].first - tmpl.begin();
                    size_t pos2 = result[0].second - tmpl.begin() - 1;
                    parse_if(end_pos + 2, pos1 - end_pos - 2, statement);
                    i = pos2;
                }
                else if (regex_match(tmp_str, result, Token::FOR_REGEX))
                {
                    string var1 = result.str(1), var2 = result.str(2);
                    string::const_iterator start = tmpl.begin() + end_pos + 2;
                    string::const_iterator end = tmpl.begin() + start_pos + len2;
                    result = findNext(start, end, Token::ENDFOR_REGEX, Token::FOR_REGEX);
                    size_t pos1 = result[0].first - tmpl.begin();
                    size_t pos2 = result[0].second - tmpl.begin() - 1;
                    parse_for(end_pos + 2, pos1 - end_pos - 2, var1, var2);
                    i = pos2;
                }
                else if (regex_match(tmp_str, result, Token::INCLUDE_REGEX))
                {
                    // 简单实现方法如下，不过这种方法需要复制var_tables，会浪费时间
                    // ss << HtmlTemplate(result.str(1)).setValue(JSONObject(var_tables)).render();

                    string var1 = result.str(1);   // 文件位置
                    string save = std::move(tmpl); // 保存临时值
                    FILE *new_file = fopen(var1.c_str(), "rb");
                    i += result[0].length();
                    i--;
                    if (new_file == nullptr)
                        throwException(FileException, "文件不存在");
                    long size = get_file_size(new_file);
                    if (size <= 0)
                    {
                        fclose(new_file);
                        tmpl = std::move(save); // 退回
                    }
                    else if (size > MAX_FILE_SIZE)
                    {
                        throwException(FileException, "文件太大，无法处理");
                    }
                    else
                    {
                        auto *buf = new char[size + 1];
                        fread(buf, static_cast<size_t>(size), 1, new_file);
                        buf[size] = 0;
                        tmpl = buf;
                        delete[] buf;
                        parse(0, tmpl.size());
                        fclose(new_file);
                        tmpl = std::move(save); // 退回
                    }
                }
                else
                    throwException(TemplateParseException, errorView(i) + "模板解析错误，未知语句");
            }
            else if (tmpl[i + 1] == Token::VAR_START)
            {
                // 处理变量
                //  TODO 处理嵌套情况 {{ key.{{ x }} }} , 这种情况一般用在for迭代object
                size_t end_pos = tmpl.find(Token::VAR_END, i);
                if (tmpl[end_pos + 1] != Token::TMPL_END)
                    throwException(TemplateParseException, errorView(i) + "模板解析错误");
                size_t len = end_pos + 2 - i;
                string tmp_str = tmpl.substr(i, len);
                auto &&var_result = parse_var(tmp_str);
                ss << var_result;
                i = end_pos + 1;
            }
            else
            {
                ss << '{';
            }
            last_pos = i + 1;
        }
    }
    ss << tmpl.substr(last_pos, start_pos + len2 - last_pos);
}

void luckyao::HtmlTemplate::parse_if(size_t start, size_t len, const string &statement)
{
    // TODO statement处理
    bool true_or_false = false;
    string::const_iterator start_it = tmpl.begin() + start;
    string::const_iterator end_it = tmpl.begin() + start + len;
    smatch result;
    enum TYPE
    {
        VAR,    // 变量
        NUMBER, // 数字
        STRING, // 字符串
    };
    auto check_type = [](const string &var) -> TYPE
    {
        // 检查表达式类型
        if (isdigit(var[0]) && str_isnumber(var))
            return NUMBER;
        else if (var[0] == '"' && var[var.size() - 1] == '"')
            return STRING;
        return VAR;
    };

    auto get_var = [this](const string &var) -> Json
    {
        // 检查表达式类型
        return parse_var(str_format("{{%s}}", var.c_str()));
    };

    auto get_number = [](Json &var) -> double
    {
        const auto &var_type = var.type();
        if (var_type == JsonType::Int)
            return static_cast<double>(var.asInt());
        // else if (var_type == typeid(float))
        //     return static_cast<double>(any_cast<int>(var));
        else if (var_type == JsonType::Double)
            return var.asDouble();
        throwException(TemplateParseException, "错误的类型转换");
    };

    if (regex_match(statement, result, Token::EQUAL_REGEX))
    { // a<b a==b a>b a!=b 类型表达式
        TYPE type1 = check_type(result.str(1));
        TYPE type2 = check_type(result.str(3));
        Json &&expr1 = get_var(result.str(1));
        Json &&expr2 = get_var(result.str(3));
        string &&op = result.str(2);                           // 操作符号
        bool number_comp = type1 == NUMBER || type2 == NUMBER; // 比较数字,否则比较字符串
        if (number_comp)
        {
            double number1 = get_number(expr1);
            double number2 = get_number(expr2);
            if (op == "==")
                true_or_false = (fabs(number1 - number2) < 1e-4); // 两个数相等
            else if (op == ">")
                true_or_false = ((number1 - number2) > 1e-4);
            else if (op == "<")
                true_or_false = ((number1 - number2) < 1e-4);
            else if (op == "!=")
                true_or_false = !(fabs(number1 - number2) < 1e-4);
        }
        else
        {
            if (expr1.type() != JsonType::String || expr2.type() != JsonType::String)
                throwException(TemplateParseException, "错误的类型转换");
            string str1 = expr1.asString();
            string str2 = expr2.asString();
            if (op == "==")
                true_or_false = (str1 == str2);
            else if (op == ">")
                true_or_false = (str1 > str2);
            else if (op == "<")
                true_or_false = (str1 < str2);
            else if (op == "!=")
                true_or_false = (str1 != str2);
        }
    }
    else if (regex_match(statement, result, Token::NOT_REGEX))
    { // !a 类型表达式
        TYPE type = check_type(result.str(1));
        Json &&expr = get_var(result.str(1));
        if (type == NUMBER)
        {
            double number = get_number(expr);
            true_or_false = (fabs(number) < 1e-4); // number == 0
        }
        else if (type == STRING)
        {
            true_or_false = expr.asString().empty();
        }
        else
        {
            true_or_false = expr.empty();
        }
    }
    else
    { // 看成一个变量
        TYPE type = check_type(statement);
        Json &&expr = get_var(statement);
        if (type == NUMBER)
        {
            double number = get_number(expr);
            true_or_false = fabs(number) >= 1e-4; // number == 0
        }
        else if (type == STRING)
        {
            true_or_false = !expr.asString().empty();
        }
        else
        {
            true_or_false = !expr.empty();
        }
    }

    if (regex_search(start_it, end_it, result, Token::ELSE_REGEX))
    { // if-else-endif 结构
        auto it1 = result[0].first;
        auto it2 = result[0].second;
        if (true_or_false)
        { // if 成功
            size_t len2 = it1 - tmpl.begin() - start;
            parse(start, len2);
        }
        else
        {                                                     // 处理else
            size_t start2 = it2 - tmpl.begin();               // 新的起始位置
            size_t len2 = start + len - (it2 - tmpl.begin()); // 新的长度
            parse(start2, len2);
        }
    }
    else
    { // if-endif 结构
        if (!true_or_false)
            return;
        parse(start, len);
    }
}

void luckyao::HtmlTemplate::parse_for(size_t start, size_t len, const string &var1, const string &var2)
{
    Json &&s = __parse_var(const_cast<string &>(var2));
    auto type = s.type();
    if (type == JsonType::Object)
    {
        // TOOD 对字典迭代，两种情况
        // {% for key in obj %}
        // {% for key,value in obj %}
        pair<string, string> keyvalue = split2(var1, ",");
        auto keys = s.getAllKeys();
        Json save_key = m_tables[keyvalue.first]; // 存储之前的值
        Json save_value = m_tables[keyvalue.second];
        for (auto &key : keys)
        {
            if (!keyvalue.first.empty())
                m_tables[keyvalue.first] = key;
            if (!keyvalue.second.empty())
                m_tables[keyvalue.second] = m_tables[key];
            parse(static_cast<int>(start), static_cast<int>(len));
        }
        m_tables[keyvalue.first] = save_key; // 全局变量退回
        m_tables[keyvalue.second] = save_value;
    }
    else if (type == JsonType::Array)
    {
        const Json save = m_tables[var1]; // 存储之前全局变量

        for (auto &item : s)
        {
            m_tables[var1] = item;
            parse(static_cast<int>(start), static_cast<int>(len));
        }
        m_tables[var1] = save; // 全局变量退回
    }
    else
        throwException(TemplateParseException, errorView(start) + "for迭代失败");
}

luckyao::Json luckyao::HtmlTemplate::parse_var(const string &tmp_str)
{
    smatch result;
    if (regex_match(tmp_str, result, Token::VAR_REGEX))
    {
        auto var = result.str(1);
        vector<string> res; // 变量集合
        vector<char> op;    // 操作符集合
        size_t j = 0;
        for (size_t i = 0; i < var.size(); i++)
        {
            char ch = var[i];
            if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
            {
                res.push_back(var.substr(j, i - j));
                op.push_back(ch);
                j = i + 1;
            }
        }
        if (res.empty())
        {
            if (var[0] == '"' && var[var.size() - 1] == '"') // 处理 {{ "abcdef" }} 类型
                return var.substr(1, var.size() - 2);
            else if (str_isnumber(var)) // 处理 {{ 1 }}
                return str_parse<double>(var);
            return __parse_var(var);
        }
        res.push_back(var.substr(j));
        vector<Json> vars; // 变量集合
        vars.reserve(res.size());
        enum
        {
            UNKNOWN,
            NUMBER,
            STRING,
        } TYPE = UNKNOWN; // 结果类型
        for (auto &item : res)
        {
            if (item[0] == '"' && *(item.end() - 1) == '"' && item.size() > 2)
            { // 字符串
                vars.emplace_back(item.substr(1, item.size() - 2));
                if (TYPE == NUMBER)
                    throwException(TemplateParseException, tmp_str + "解析变量失败");
                TYPE = STRING;
            }
            else if (isdigit(item[0]) && str_isnumber(item))
            { // 立即数
                vars.emplace_back(str_parse<double>(item));
                if (TYPE == STRING)
                    throwException(TemplateParseException, tmp_str + "解析变量失败");
                TYPE = NUMBER;
            }
            else
                vars.emplace_back(__parse_var(item));
        }
        auto &first = vars[0];
        string str;

        /* 初始化第一个 */
        // if (first.type() == typeid(int) || first.type() == typeid(float) || first.type() == typeid(double))
        if (first.type() == JsonType::Int || first.type() == JsonType::Double)
        {
            if (TYPE == STRING)
                throwException(TemplateParseException, tmp_str + "解析变量失败");
            TYPE = NUMBER;
        }
        else
        {
            if (TYPE == NUMBER)
                throwException(TemplateParseException, tmp_str + "解析变量失败");
            TYPE = STRING;
            str = first.asString();
        }

        if (TYPE == NUMBER)
            return calculator(vars, op);

        /* 字符串拼接 */
        for (size_t i = 1; i < vars.size(); i++)
        {
            auto &item = vars[i];
            if (item.type() == JsonType::String)
            {
                if (op[i - 1] != '+')
                    throwException(TemplateParseException, errorView(i) + "对字符串操作符不合法");
                str.append(item.asString());
            }
            else
                throwException(TemplateParseException, errorView(i) + "类型不匹配");
        }
        return str;
    }
    else
        throwException(TemplateParseException, tmp_str + " 解析变量失败");
}

std::smatch luckyao::HtmlTemplate::findNext(string::const_iterator start, const string::const_iterator end, const regex &reg, const regex &reg2)
{

    smatch result, result2;
    if (regex_search(start, end, result, reg) == 0)
        throwException(TemplateParseException, errorView(start - tmpl.begin()) + "模板for if嵌套使用结束位置不对");
    const auto &end_ptr = result[0].first;
    int cnt = 0;
    while (regex_search(start, end_ptr, result2, reg2) == 1)
    {
        start = result2[0].second;
        cnt++;
    }
    while (cnt--)
    {
        start = result[0].second;
        if (regex_search(start, end, result, reg) == 0)
            throwException(TemplateParseException, errorView(start - tmpl.begin()) + "模板for缺少endfor");
    }
    return result;
}

luckyao::Json luckyao::HtmlTemplate::__parse_var(string &var)
{
    size_t j;
    Json var_result; // 得到访问的最终结果
    size_t var_len = var.size();
    for (j = 0; j < var_len; j++)
    {
        if (var[j] == '.' || var[j] == '[')
        {

            var_result =
                m_tables[var.substr(0, j)];
            break;
        }
        else if (var[j] == ']')
        {
            throwException(TemplateParseException, "模板语法错误: " + var)
        }
    }
    if (j == var_len)
        var_result = m_tables[var];
    enum
    {
        OBJ,
        ARR,
        NULL_PTR,
        UNKNOWN
    } type = OBJ;
    for (; j < var_len;)
    {
        if (type == NULL_PTR)
            throwException(TemplateParseException, "无法对空对象进行索引 " + var);
        if (var_result.type() == JsonType::Object)
        {
            type = OBJ;
        }
        else if (var_result.type() == JsonType::Array)
        {
            type = ARR;
        }
        else
            type = UNKNOWN;
        if (var[j] == '.')
        {
            if (type != OBJ)
                throwException(TemplateParseException, "无法使用.访问" + var);
            size_t k = j + 1;
            for (; k < var_len; k++)
            {
                if (var[k] == '[' || var[k] == '.')
                {
                    if (k == j + 1)
                        throwException(TemplateParseException, "不能取空的下标" + var);
                    break;
                }
                else if (var[k] == ']')
                    throwException(TemplateParseException, "不能访问到]" + var);
            }
            string to_vis = var.substr(j + 1, k - j - 1);
            auto temp_obj = var_result;
            var_result = temp_obj[to_vis];
            j = k;
            if (var_result.empty())
                type = NULL_PTR;
        }
        else if (var[j] == '[')
        {
            if (type != ARR)
                throwException(TemplateParseException, "无法使用[]访问" + var);
            int index = 0;
            size_t k = j + 1;
            for (; k < var.size(); k++)
            {
                if (isdigit(var[k]))
                    index = index * 10 + var[k] - '0';
                else if (var[k] == ']')
                {
                    auto temp_arr = var_result;
                    if (temp_arr.size() <= index)
                        throwException(TemplateParseException, "数组访问越界 " + var);
                    var_result = temp_arr[index];
                    break;
                }
                else
                    throwException(TemplateParseException, "不允许非数字下标");
            }
            j = k + 1;
        }
        else
            throwException(TemplateParseException, "解析错误");
    }
    return var_result;
}

string luckyao::HtmlTemplate::errorView(size_t pos)
{
    size_t start = pos < 10 ? 0 : pos - 10;
    string &&str = tmpl.substr(start, 30);
    /* 清除\n \r */
    char buf[50], space[11];
    size_t i;
    for (i = 0; i < (10 > pos ? pos : 10); i++)
        space[i] = ' ';
    space[i] = 0;
    i = 0;
    for (auto &ch : str)
        if (ch != '\r' && ch != '\n')
            buf[i++] = ch;
    buf[i] = 0;
    return str_format("%s\n%s↑\n错误信息: ", buf, space);
}
