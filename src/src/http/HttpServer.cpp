#include "HttpServer.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include "ByteBuffer.h"
#include "Logger.h"
#include "Request.h"
#include "Response.h"
#include "Resource.h"
#include "ResourceHost.h"
#include "ClassFactory.h"
#include "Object.h"
#include "Stringx.h"

#include <string>

using namespace luckyao;
using namespace std;

HttpServer::HttpServer(EventLoop *loop, int threadNum, const std::string &nameArg, int nPort)
    : m_htdocsPath("E:\\1\\ts\\wuday\\htdocs"),
      m_bStaticServer(false),
      m_loop(loop)
{
    InetAddress addr(nPort);
    // string name = "HttpServer";

    m_tcpserver = make_shared<TcpServer>(m_loop, addr, nameArg);

    m_tcpserver->setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, std::placeholders::_1));

    m_tcpserver->setMessageCallback(
        std::bind(&HttpServer::onMessage, this,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_tcpserver->setThreadNum(threadNum);
}
// HttpServer::HttpServer(int nPort)
//     //: m_htdocsPath("/home/yozo1093/wuday/htdocs")
//     : m_htdocsPath("E:\\1\\ts\\wuday\\htdocs")
// {
//     SockAddress addr(nPort);
//     string name = "HttpServer";
//     m_loop = new EventLoop();

//     m_resourceHost = new ResourceHost(m_htdocsPath);

//     m_tcpserver = make_shared<TcpServer>(m_loop, addr, name);

//     m_tcpserver->setConnectionCallback(
//         std::bind(&HttpServer::onConnection, this, std::placeholders::_1));

//     m_tcpserver->setMessageCallback(
//         std::bind(&HttpServer::onMessage, this,
//                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

//     m_tcpserver->setThreadNum(3);
// }
HttpServer::~HttpServer()
{
    // delete m_loop;
    // m_loop = nullptr;
    if (m_resourceHost)
    {
        delete m_resourceHost;
        m_resourceHost = nullptr;
    }
}
void HttpServer::setStaticServer(const std::string &path)
{
    m_htdocsPath = path;
    m_bStaticServer = true;
    m_resourceHost = new ResourceHost(m_htdocsPath);
}
void HttpServer::start()
{
    m_tcpserver->start();
    // m_loop->loop();
}
void HttpServer::bind(const string &path, ServerHandlerType handler)
{
    m_handlers[path] = handler;
}
void HttpServer::onConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
    }
    else
    {
        LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
    }
}
void HttpServer::handleControl(const TcpConnectionPtr &conn, const Request &req)
{
    Response resp;
    Object *ctrl = nullptr;
    string path = req.getRequestUri();
    try
    {
        std::map<string, ServerHandlerType>::iterator it = m_handlers.find(path);
        if (it != m_handlers.end())
        {
            it->second(req, resp);
        }
        else
        {
            string classname;
            string methodname;
            std::vector<string> arr;
            Stringx::split(arr, Stringx::trim(path, " /"), '/');
            if (arr.size() == 0)
            {
                classname = "Index";
                methodname = "index";
            }
            else if (arr.size() == 1)
            {
                classname = Stringx::capitalize(arr[0]);
                methodname = "index";
            }
            else if (arr.size() == 2)
            {
                classname = Stringx::capitalize(arr[0]);
                methodname = arr[1];
            }
            ClassFactory *factory = Singleton<ClassFactory>::instance();

            ctrl = factory->createClass(classname);
            if (ctrl == nullptr)
            {
                sendStatusResponse(conn, Status(NOT_FOUND));
            }
            else
            {
                ctrl->call(methodname, req, resp);
                delete ctrl;
            }
        }
    }
    catch (std::exception &e)
    {
        sendStatusResponse(conn, Status(NOT_FOUND));
        if (ctrl != nullptr)
            delete ctrl;
    }
    sendResponse(conn, resp);
}
void HttpServer::onMessage(const TcpConnectionPtr &conn,
                           ByteBuffer &buf,
                           Timestamp time)
{
    LOG_INFO("HttpServer::onMessage");
    // std::string msg = buf->retrieveAllAsString();

    // conn->send(msg);
    // conn->shutdown();

    Request *req = nullptr;
    Response *resp = nullptr;
    do
    {
        req = new Request(&buf);
        if (req->parse() == false)
            break;
        std::string uri = req->getRequestUri();

        if (m_bStaticServer)
        {
            Resource *r = m_resourceHost->getResource(uri);
            if (r == nullptr)
            {
                uri = "/index.html";
                r = m_resourceHost->getResource(uri);
            }
            if (r != nullptr)
            {
                resp = new Response();
                resp->setStatus(Status(OK));
                resp->addHeader("Content-Type", r->getMimeType());
                resp->addHeader("Content-Length", r->getSize());
                // Only send a message body if it's a GET request. Never send a body for HEAD
                if (req->getMethod() == Method(GET))
                    resp->setData(r->getData(), r->getSize());

                bool dc = false;

                // HTTP/1.0 should close the connection by default
                if (req->getVersion().compare(HTTP_VERSION_10) == 0)
                    dc = true;

                // If Connection: close is specified, the connection should be terminated after the request is serviced
                string connection_val = req->getHeaderValue("Connection");
                if (connection_val.compare("close") == 0)
                    dc = true;

                sendResponse(conn, *resp, dc);
            }
            else
            {
                sendStatusResponse(conn, Status(NOT_FOUND));
            }
        }
        else
        {
            handleControl(conn, *req);
        }

    } while (false);
    delete req;
    delete resp;
    buf.clear();
}
void HttpServer::sendResponse(const TcpConnectionPtr &conn, Response &resp, bool disconnect)
{
    // Server Header
    resp.addHeader("Server", "wms/1.0");

    // Time stamp the response with the Date header
    // std::string tstr;
    // char tbuf[36] = {0};
    // struct tm bufTm;
    // time_t t = time(NULL);
    // gmtime_s(&bufTm, &t);
    // // Ex: Fri, 31 Dec 1999 23:59:59 GMT
    // strftime(tbuf, 36, "%a, %d %b %Y %H:%M:%S GMT", &bufTm);
    // resp.addHeader("Date", tbuf);
    Timestamp now;
    resp.addHeader("Date", now.toString("%a, %d %b %Y %H:%M:%S GMT"));

    // Include a Connection: close header if this is the final response sent by the server
    if (disconnect)
        resp.addHeader("Connection", "close");
    else
        resp.addHeader("Connection", "keep-alive");

    // Get raw data by creating the response (we are responsible for cleaning it up in process())
    // byte* pData = resp->create();

    // m_IOCPServer->send(ctx,(const char*) pData, resp->size());
    resp.prepare();
    conn->send(resp.retrieveAllAsString());
    if (disconnect)
    {
        conn->shutdown();
    }
}
void HttpServer::sendStatusResponse(const TcpConnectionPtr &conn, int status, std::string msg)
{
    Response *resp = new Response();
    resp->setStatus(Status(status));

    // Body message: Reason string + additional msg
    string body = resp->getReason();
    if (msg.length() > 0)
        body += ": " + msg;

    unsigned int slen = body.length();
    /*char* sdata = new char[slen];
    memset(sdata, '\0', slen);
    strncpy_s(sdata, slen, body.c_str(), slen);*/

    resp->addHeader("Content-Type", "text/plain");
    resp->addHeader("Content-Length", slen);
    resp->addHeader("Connection", "close");
    // resp->setData((byte*)sdata, slen);
    resp->setData((char *)body.c_str(), slen);

    sendResponse(conn, *resp, true);

    delete resp;
}