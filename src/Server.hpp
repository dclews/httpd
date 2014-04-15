#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <clews/core/CoreObject.hpp>
#include <clews/sock/Socket.hpp>
#include <clews/utility/StringUtil.hpp>
#include <clews/utility/FileUtil.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <clews/mime/MimeDB.hpp>
#include <clews/http/HttpResponseDB.hpp>
#include <clews/http/HttpResponse.hpp>
#include <clews/http/HttpRequest.hpp>
#include <clews/http/HttpResource.hpp>
#include <thread>
#include "ServerConfig.hpp"
#include "RequestHandler.hpp"

#ifndef PATH_MAX
#define PATH_MAX 250
#endif

enum ExitCodes
{
    SUCCESS         = 0,
    CREATE_FAILED   = 1,
    LISTEN_FAILED   = 2,
    DESTROY_DIRTY   = 3,
    COMMAND_FAILED  = 4
};

class HTTPServer : CoreObject
{
private:
    bool mRun;
    Socket mSocket;
    ServerConfig& mServerConfig;

    std::thread mConnectionThread;
    std::thread mCommandThread;

    void CommandHandler();
    void SpawnConnections();
public:
    HTTPServer(ServerConfig& serverConfig);
    ExitCodes Start();
    void Stop();
    bool LoadConfig();
    bool ReloadConfig();
};

#endif
