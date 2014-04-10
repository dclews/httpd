#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <clews/http/HttpRequest.hpp>
#include <clews/http/HttpResponse.hpp>
#include <clews/http/HttpResource.hpp>
#include <clews/utility/KvpDB.hpp>
#include <clews/core/CoreObject.hpp>
#include "ServerConfig.hpp"

class RequestHandler : CoreObject
{
private:
    ServerConfig& mServerConfig;
    HttpRequest& mRequest;

    uint32_t LoadContent(HttpResponse& resp, std::string url);
    uint32_t HandleError(HttpResponse& resp, uint32_t errorCode);
    uint32_t LoadFileUnsafe(HttpResponse& resp, std::string url);
public:
    RequestHandler(HttpRequest& req, ServerConfig& serverConfig);
    HttpResponse GenerateResponse();
};

#endif
