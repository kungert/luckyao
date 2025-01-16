#pragma once

#include <string>

namespace luckyao
{
    class Resource
    {
    public:
        Resource(std::string loc, bool dir = false);
        ~Resource();

        bool readFile();
        // Setters

        void setData(char *d, unsigned int s)
        {
            if (m_data)
            {
                delete[] m_data;
            }
            m_data = d;
            m_size = s;
        }

        void setMimeType(std::string mt)
        {
            m_mimeType = mt;
        }

        // Getters

        std::string getMimeType()
        {
            return m_mimeType;
        }

        std::string getLocation()
        {
            return m_location;
        }

        bool isDirectory()
        {
            return m_bDirectory;
        }

        char *getData()
        {
            return m_data;
        }

        unsigned int getSize()
        {
            return m_size;
        }

        // Get the file name
        static std::string getName(std::string &path);

        // Get the file extension
        static std::string getExtension(std::string &path);

    private:
        char *m_data; // File m_data
        unsigned int m_size;
        std::string m_mimeType;
        std::string m_location; // Disk path m_location within the server
        bool m_bDirectory;
    };

}