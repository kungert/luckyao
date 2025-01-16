#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <fstream>

namespace luckyao
{
    class Resource;

    // Valid files to serve as an index of a directory
    const static char *const validIndexes[] = {
        "index.html",
        "index.htm"};

    class ResourceHost
    {
    public:
        ResourceHost(std::string base);
        ResourceHost();
        ~ResourceHost();

        // Write a resource to the file system
        void putResource(Resource *res, bool writeToDisk);

        // Returns a Resource based on URI
        Resource *getResource(std::string uri);

    private:
        // Local file system base path
        std::string m_baseDiskPath;

        // Dictionary that relates file extensions to their MIME type
        std::unordered_map<std::string, std::string> m_mimeMap = {
#define STR_PAIR(K, V) std::pair<std::string, std::string>(K, V)
#include "MimeTypes.inc"
        };
        std::unordered_map<std::string, Resource *> m_resources;

    private:
        // Returns a MIME type string given an extension
        std::string lookupMimeType(std::string ext);

        // Read a file from the FS and into a Resource object
        Resource *readFile(std::string path);

        // Provide a string rep of the directory listing based on URI
        std::string generateDirList(std::string dirPath);
    };
}