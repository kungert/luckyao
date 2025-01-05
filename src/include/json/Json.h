/*
 * @Author: kunger
 * @Date: 2023-06-09 23:07:42
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-15 19:42:20
 */
#pragma once

// #include "base/defs.h"
#include <string>
#include <ostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace luckyao
{
    typedef enum _Type
    {
        Error = 0,
        Null,   // null value
        Bool,   // bool value
        Int,    // integer value
        Double, // double value
        String, // string value
        Array,  // array value
        Object  // object value
    } JsonType;
    class Json
    {
    public:
        Json(JsonType type = JsonType::Object);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char *value);
        Json(const std::string &value);
        Json(const Json &other);
        Json(std::initializer_list<std::pair<const std::string, Json>> values);
        // Json(std::initializer_list<Json> values);
        ~Json();

        static Json array(std::initializer_list<Json> values);
        JsonType type() const;

        bool isError() const;
        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isDouble() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        bool asBool() const;
        int asInt() const;
        double asDouble() const;
        std::string asString() const;

        // number of values in array or object
        int size() const;

        // return true if empty array, empty object, or null, otherwise, false.
        bool empty() const;

        void clear();

        bool has(int index) const;
        bool has(const char *key) const;
        bool has(const std::string &key) const;

        Json get(int index) const;
        Json get(const char *key) const;
        Json get(const std::string &key) const;
        std::vector<std::string> getAllKeys();

        void remove(int index);
        void remove(const char *key);
        void remove(const std::string &key);

        Json getAndRemove(const std::string &key);
        // append value to array at the end.
        // void append(const Json &value);
        // append value to Json at the end.
        // void append(const std::string &key, const Json &value);
        // add value to Object at the end.
        // json object
        bool extend(const Json &json);
        // json array
        bool add(const Json &json);
        bool add(const std::string &val);
        bool add(const char *val);
        bool add(const std::string key, const Json &value);
        bool add(const std::string name, std::vector<Json> items);

        Json &operator=(const Json &other);
        bool operator==(const Json &other);
        bool operator!=(const Json &other);

        Json operator[](int index);
        Json operator[](const char *key);
        Json operator[](const std::string &key);

        friend std::ostream &operator<<(std::ostream &os, const Json &json)
        {
            os << json.str();
            return os;
        }

        operator bool();
        operator int();
        operator double();

        Json &parse(const std::string &str);
        // static Json parse(const char *key);
        std::string str() const;

        typedef std::list<Json>::iterator iterator;
        iterator begin()
        {
            return (m_value.m_array)->begin();
        }
        iterator end()
        {
            return (m_value.m_array)->end();
        }

    private:
        void copy(const Json &other);

    private:
        union Value
        {
            bool m_bool;
            int m_int;
            double m_double;
            std::string *m_string;
            std::list<Json> *m_array;
            std::unordered_map<std::string, Json> *m_object;
        };

        JsonType m_type;
        Value m_value;
    };
} // namespace luckyao
