#ifndef CONNECTION_HANDLER_HPP
#define CONNECTION_HANDLER_HPP

#include <clews/core/CoreObject.hpp>
#include <clews/sock/Socket.hpp>
#include "ServerConfig.hpp"


class ConnectionHandler : public CoreObject
{
private:
	ServerConfig& mServerConfig;
	Socket& mSocketStream;
public:
	ConnectionHandler(Socket& ss, ServerConfig& config);
	bool AcceptConnection();
};

#endif
