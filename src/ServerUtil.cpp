#include "ServerUtil.hpp"

using namespace std;

string statusPagePath(ServerConfig& serverConfig, int statusCode)
{
	string pageDir = serverConfig["StatusPages"];
	char buffer[20]; snprintf(buffer, 20, "/%d.html", statusCode);
	return string(pageDir+buffer);
}
