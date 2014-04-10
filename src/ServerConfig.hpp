#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <clews/utility/KvpDB.hpp>
#include <clews/mime/MimeDB.hpp>
#include <clews/http/HttpResponseDB.hpp>

class ServerConfig : public KvpDB
{
private:
    MimeDB mMimeDB;
    HttpResponseDB mStatusDB;
public:
    ServerConfig();
    bool Init();

    bool ValidatePath(const std::string& filePath);
    //@Override.
    bool Reload();
    MimeDB& Mimes();
    HttpResponseDB& StatusDB();
};

#endif
