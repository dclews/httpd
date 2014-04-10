#ifndef CONNECTION_HANDLER_HPP
#define CONNECTION_HANDLER_HPP

#include <clews/core/CoreObject.hpp>
#include <clews/sock/SocketStream.hpp>
#include "ServerConfig.hpp"


class ConnectionHandler : public CoreObject
{
private:
    ServerConfig& mServerConfig;
    SocketStream& mSocketStream;
public:
    ConnectionHandler(SocketStream& ss, ServerConfig& config);
    bool AcceptConnection();
};

#endif
