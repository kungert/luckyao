/*
 * @Author: kunger
 * @Date: 2023-06-09 23:10:43
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-15 19:46:29
 */
#include <sstream>
#include <stdexcept>
#include "base/Stringx.h"

#include "json/Json.h"
#include "json/Parser.h"
// using namespace luckyao;
using namespace std;

namespace luckyao
{
    Json::Json(JsonType type)
        : m_type(type)
    {
        switch (m_type)
        {
        case Null:
            break;
        case Bool:
            m_value.m_bool = false;
            break;
        case Int:
            m_value.m_int = 0;
            break;
        case Double:
            m_value.m_double = 0.0;
            break;
        case String:
            m_value.m_string = new string("");
            break;
        case Array:
            m_value.m_array = new std::list<Json>();
            break;
        case Object:
            m_value.m_object = new std::unordered_map<string, Json>();
        default:
            break;
        }
    }

    Json::Json(bool value) : m_type(Bool)
    {
        m_value.m_bool = value;
    }

    Json::Json(int value) : m_type(Int)
    {
        m_value.m_int = value;
    }

    Json::Json(double value) : m_type(Double)
    {
        m_value.m_double = value;
    }

    Json::Json(const char *value) : m_type(String)
    {
        m_value.m_string = new string(value);
    }

    Json::Json(const string &value) : m_type(String)
    {
        m_value.m_string = new string(value);
    }

    Json::Json(const Json &other) : m_type(Null)
    {
        copy(other);
    }

    Json::Json(initializer_list<pair<const string, Json>> values)
    {
        m_type = Object;
        m_value.m_object = new std::unordered_map<std::string, Json>();
        for (auto al : values)
        {
            add(al.first, al.second);
        }
    }
    // Json::Json(std::initializer_list<Json> values)
    // {
    //     m_type = Array;
    //     m_value.m_array = new std::list<Json>();
    //     for (auto al : values)
    //     {
    //         add(al);
    //     }
    // }
    Json::~Json()
    {
        clear();
    }

    Json Json::array(std::initializer_list<Json> values)
    {
        Json json(JsonType::Array);
        for (auto al : values)
        {
            json.add(al);
        }
        return json;
    }
    JsonType Json::type() const
    {
        return m_type;
    }

    bool Json::isError() const
    {
        return m_type == Error;
    }
    bool Json::isNull() const
    {
        return m_type == Null;
    }

    bool Json::isBool() const
    {
        return m_type == Bool;
    }

    bool Json::isInt() const
    {
        return m_type == Int;
    }

    bool Json::isDouble() const
    {
        return m_type == Double;
    }

    bool Json::isString() const
    {
        return m_type == String;
    }

    bool Json::isArray() const
    {
        return m_type == Array;
    }

    bool Json::isObject() const
    {
        return m_type == Object;
    }

    bool Json::asBool() const
    {
        if (m_type == Bool)
        {
            return m_value.m_bool;
        }
        throw std::logic_error("function asBool value type error");
    }

    int Json::asInt() const
    {
        if (m_type == Int)
        {
            return m_value.m_int;
        }
        throw std::logic_error("function asInt value type error");
    }

    double Json::asDouble() const
    {
        if (m_type == Double)
        {
            return m_value.m_double;
        }
        throw std::logic_error("function asDouble value type error");
    }

    string Json::asString() const
    {
        switch (m_type)
        {
        case Null:
            break;
        case Bool:
            return Stringx::toString(m_value.m_bool);
        case Int:
            return Stringx::toString(m_value.m_int);
            break;
        case Double:
            return Stringx::toString(m_value.m_double);
            break;
        case String:
            return *(m_value.m_string);
            break;
        case Array:
            break;
            break;
        case Object:
            break;
        default:
            break;
        }
        return "";
    }

    // 深拷贝
    void Json::copy(const Json &other)
    {
        clear();
        m_type = other.m_type;

        switch (m_type)
        {
        case Error:
        case Null:
        case Bool:
        case Int:
        case Double:
            m_value = other.m_value;
            break;
        case String:
        {
            m_value.m_string = new string(*other.m_value.m_string);
        }
        break;
        case Array:
        {
            m_value.m_array = new std::list<Json>((other.m_value.m_array)->begin(), (other.m_value.m_array)->end());
        }
        break;
        case Object:
        {
            m_value.m_object = new unordered_map<std::string, Json>((other.m_value.m_object)->begin(), (other.m_value.m_object)->end());
        }
        break;
        default:
            break;
        }
    }
    int Json::size() const
    {
        switch (m_type)
        {
        case Array:
            return (m_value.m_array)->size();
        case Object:
            return (m_value.m_object)->size();
        default:
            break;
        }
        throw std::logic_error("function size value type error");
    }

    bool Json::empty() const
    {
        switch (m_type)
        {
        case Null:
            return true;
        case Array:
            return (m_value.m_array)->empty();
        case Object:
            return (m_value.m_object)->empty();
        default:
            break;
        }
        return false;
    }

    void Json::clear()
    {
        switch (m_type)
        {
        case Error:
        case Null:
        case Bool:
        case Int:
        case Double:
            break;
        case String:
        {
            delete m_value.m_string;
        }
        break;
        case Array:
        {
            delete m_value.m_array;
        }
        break;
        case Object:
        {
            delete m_value.m_object;
        }
        break;
        default:
            break;
        }
        m_type = Null;
    }

    bool Json::has(int index) const
    {
        if (m_type != Array)
        {
            return false;
        }
        int size = (m_value.m_array)->size();
        return (index >= 0) && (index < size);
    }

    bool Json::has(const char *key) const
    {
        string name(key);
        return has(name);
    }

    bool Json::has(const string &key) const
    {
        if (m_type != Object)
        {
            return false;
        }
        return (m_value.m_object)->find(key) != (m_value.m_object)->end();
    }

    Json Json::get(int index) const
    {
        if (!has(index))
        {
            return Json();
        }
        auto it = (m_value.m_array)->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }
        return *it;
    }

    Json Json::get(const char *key) const
    {
        if (!has(key))
        {
            return Json();
        }
        return (*(m_value.m_object))[key];
    }

    Json Json::get(const string &key) const
    {
        if (!has(key))
        {
            return Json();
        }
        return (*(m_value.m_object))[key];
    }

    vector<string> Json::getAllKeys()
    {
        vector<string> rs;
        if (isObject())
        {
            std::unordered_map<string, Json>::iterator ite;
            for (ite = m_value.m_object->begin(); ite != m_value.m_object->end(); ite++)
            {
                rs.push_back(ite->first);
            }
        }
        return rs;
    }
    void Json::remove(int index)
    {
        if (m_type != Array)
        {
            return;
        }
        int size = (m_value.m_array)->size();
        if (index < 0 || index >= size)
        {
            return;
        }
        auto it = (m_value.m_array)->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }
        it->clear();
        (m_value.m_array)->erase(it);
    }

    void Json::remove(const char *key)
    {
        string name = key;
        remove(name);
    }

    void Json::remove(const string &key)
    {
        if (m_type != Object)
        {
            return;
        }
        auto it = (m_value.m_object)->find(key);
        if (it != (m_value.m_object)->end())
        {
            it->second.clear();
            (m_value.m_object)->erase(key);
        }
    }
    Json Json::getAndRemove(const string &key)
    {
        Json rs = (*this)[key];
        remove(key);
        return rs;
    }

    // void append(const Json &value)
    // {
    //     if (m_type != Array)
    //     {
    //         clear();
    //         m_type = Array;
    //         m_value.m_array = new std::list<Json>();
    //     }
    //     (m_value.m_array)->push_back(value);
    // }

    // void append(const std::string &key, const Json &value)
    // {
    //     if (m_type != Object)
    //     {
    //         clear();
    //         m_type = Array;
    //         m_value.m_object = new std::map<std::string, Json>();
    //     }
    //     m_value.m_object->insert(pair<string, Json>(key, value));
    // }
    bool Json::extend(const Json &json)
    {
        if (m_type != Object || !json.isObject())
        {
            return false;
        }
        for (auto it = (json.m_value.m_object)->begin(); it != (json.m_value.m_object)->end(); it++)
        {
            if (has(it->first))
            {
                (*(m_value.m_object))[it->first] = it->second;
            }
            else
            {
                add(it->first, it->second);
            }
        }
        return true;
    }
    bool Json::add(const Json &json)
    {
        if (m_type != Array)
        {
            return false;
        }
        (m_value.m_array)->push_back(json);
        return true;
    }
    bool Json::add(const std::string &val)
    {
        Json json(val);
        return add(json);
    }
    bool Json::add(const char *val)
    {
        Json json(val);
        return add(json);
    }
    bool Json::add(const std::string key, const Json &value)
    {
        if (m_type == Error || m_type == Null)
        {
            m_type = Object;
            m_value.m_object = new std::unordered_map<std::string, Json>();
        }
        if (m_type != Object)
        {
            return false;
        }
        m_value.m_object->insert(pair<string, Json>(key, value));
        return true;
    }
    bool Json::add(const string name, std::vector<Json> items)
    {
        if (items.empty())
            return true;
        if (isObject())
        {
            Json arr(JsonType::Array);
            for (Json item : items)
            {
                arr.add(item);
            }
            return this->add(name, arr);
        }
        else
        {
            return false;
        }
    }
    Json &Json::operator=(const Json &other)
    {
        copy(other);
        return *this;
    }

    bool Json::operator==(const Json &other)
    {
        if (m_type != other.type())
        {
            return false;
        }
        switch (m_type)
        {
        case Null:
            return true;
        case Bool:
            return (m_value.m_bool == other.m_value.m_bool);
        case Int:
            return (m_value.m_int == other.m_value.m_int);
        case Double:
            return (m_value.m_double == other.m_value.m_double);
        case String:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case Array:
            return m_value.m_array == other.m_value.m_array;
        case Object:
            return m_value.m_object == other.m_value.m_object;
        default:
            break;
        }
        return false;
    }

    bool Json::operator!=(const Json &other)
    {
        return !(*this == other);
    }

    Json Json::operator[](int index)
    {
        if (index < 0)
        {
            throw std::logic_error("function [] index less than 0");
        }
        if (m_type != Array)
        {
            throw std::logic_error("function [] not an array");
        }
        int size = (m_value.m_array)->size();
        if (index >= size)
        {
            throw std::logic_error("function [] out of range");
        }
        auto it = (m_value.m_array)->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }
        return *it;
    }

    Json Json::operator[](const char *key)
    {
        string name = key;
        return (*this)[name];
    }

    Json Json::operator[](const string &key)
    {
        Json rs(Error);
        if (isObject())
        {

            if (key.empty() || !has(key))
            {
                return rs;
            }
            else
            {
                return (*(m_value.m_object))[key];
            }
        }
        return rs;
    }

    Json::operator bool()
    {
        if (m_type != Bool)
        {
            throw std::logic_error("function operator (bool) requires bool value");
        }
        return m_value.m_bool;
    }

    Json::operator int()
    {
        if (m_type != Int)
        {
            throw std::logic_error("function operator (int) requires int value");
        }
        return m_value.m_int;
    }

    Json::operator double()
    {
        if (m_type != Double)
        {
            throw std::logic_error("function operator (double) requires double value");
        }
        return m_value.m_double;
    }

    Json &Json::parse(const string &str)
    {
        Parser parser;
        parser.load(str);
        *this = parser.parse();
        return *this;
    }
    // Json parse(const char *key)
    // {
    //     Parser parser;
    //     parser.load(key);
    //     return parser.parse();
    // }

    string Json::str() const
    {
        stringstream ss;
        switch (m_type)
        {
        case Error:
            ss << "Error";
            break;
        case Null:
            ss << "null";
            break;
        case Bool:
            if (m_value.m_bool)
            {
                ss << "true";
            }
            else
            {
                ss << "false";
            }
            break;
        case Int:
            ss << m_value.m_int;
            break;
        case Double:
            ss << m_value.m_double;
            break;
        case String:
            ss << "\"" << *(m_value.m_string) << "\"";
            break;
        case Array:
        {
            ss << "[";
            for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
            {
                if (it != (m_value.m_array)->begin())
                {
                    ss << ",";
                }
                ss << (*it).str();
            }
            ss << "]";
        }
        break;
        case Object:
        {
            ss << "{";
            for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
            {
                if (it != (m_value.m_object)->begin())
                {
                    ss << ",";
                }
                ss << "\"" << it->first << "\":" << it->second.str();
            }
            ss << "}";
        }
        break;
        default:
            break;
        }
        return ss.str();
    }
}