#include "ServerConfig.hpp"
#include <clews/utility/StringUtil.hpp>

using namespace std;

ServerConfig::ServerConfig() : KvpDB("ServerConfig", "=") {}

bool ServerConfig::ValidatePath(const std::string& filePath)
{
	SetPrintPrefix(__func__, FUNC_PRINT);
	std::string webRoot = getFullPath(Get("WebRoot"));

	StandardOut() << "FilePath: " << filePath << endl;
	StandardOut() << "WebRoot : " << webRoot << endl;

	ClearPrintPrefix();
	return startsWith(filePath, webRoot);
}
//@Override
bool ServerConfig::Reload()
{
	bool success = KvpDB::Reload();
	success &= Init();
	return success;
}

bool ServerConfig::Init()
{
	string mimeDBPath = Get("MimeDB");
	bool success = mMimeDB.LoadFile(mimeDBPath);
	string statusDBPath = Get("StatusDB");
	success &= mStatusDB.LoadFile(statusDBPath);
	return success;
}
MimeDB& ServerConfig::Mimes()
{
	return mMimeDB;
}
HttpResponseDB& ServerConfig::StatusDB()
{
	return mStatusDB;
}
