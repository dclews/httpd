#ifndef SERVER_UTIL_HPP
#define SERVER_UTIL_HPP

#include <string>
#include "ServerConfig.hpp"

std::string statusPagePath(ServerConfig& serverConfig, int statusCode);

#endif
