#include "ResourceHost.h"
#include "Resource.h"

using namespace luckyao;

ResourceHost::ResourceHost(std::string base)
    : m_baseDiskPath(base)
{
}

ResourceHost::ResourceHost()
{
}

ResourceHost::~ResourceHost()
{
    for (const std::pair<std::string, Resource *> &kv : m_resources)
    {
        delete kv.second;
    }
    m_resources.clear();
}

std::string ResourceHost::lookupMimeType(std::string ext)
{
    std::unordered_map<std::string, std::string>::const_iterator it = m_mimeMap.find(ext);
    if (it == m_mimeMap.end())
        return "";

    return it->second;
}

Resource *ResourceHost::readFile(std::string path)
{
    Resource *res = NULL;
    do
    {
        std::string name = Resource::getName(path);
        if (name.length() == 0)
        {
            break;
        }
        std::string mimetype = lookupMimeType(Resource::getExtension(name));
        if (mimetype.length() == 0)
        {
            // res->setMimeType("application/octet-stream");  // default to binary
            break;
        }
        // Always disallow hidden files
        if (name.c_str()[0] == '.')
        {
            break;
        }

        res = new Resource(path);
        res->setMimeType(mimetype);

        if (!res->readFile())
        {
            delete res;
            res = nullptr;
            break;
        }

    } while (false);

    return res;
}

Resource *ResourceHost::getResource(std::string uri)
{
    // 可能存在同时调用
    std::unordered_map<std::string, Resource *>::const_iterator it = m_resources.find(uri);
    if (it != m_resources.end())
        return it->second;

    if (uri.length() > 255 || uri.empty())
        return NULL;

    // Do not allow directory traversal
    if (uri.find("../") != std::string::npos || uri.find("/..") != std::string::npos)
        return NULL;

    std::string path = m_baseDiskPath + uri;
    // Resource* res = NULL;

    //// Gather info about the resource with stat: determine if it's a directory or file, check if its owned by group/user, modify times
    // struct stat sb;
    // if (stat(path.c_str(), &sb) == -1)
    //	return NULL; // File not found

    //// Determine file type
    // if (sb.st_mode & S_IFDIR) { // Directory
    //	// Read a directory list or index into memory from FS
    //	res = readDirectory(path, sb);
    // }
    // else if (sb.st_mode & S_IFREG) { // Regular file
    //	// Attempt to load the file into memory from the FS
    //	res = readFile(path, sb);
    // }
    // else { // Something else..device, socket, symlink
    //	return NULL;
    // }
    Resource *res = readFile(path);
    if (res == NULL)
        return res;

    m_resources.insert(std::pair<std::string, Resource *>(uri, res));
    return res;
}