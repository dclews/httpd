#include "Server.hpp"
#include <unistd.h>
#include <ctime>
#include <clews/http/UserAgent.hpp>
#include "ConnectionHandler.hpp"

using namespace std;

HTTPServer::HTTPServer(ServerConfig& serverConfig) : CoreObject("HTTPServer"), mServerConfig(serverConfig),
    mSocket(serverConfig.GetInt("ServerPort"), serverConfig.GetInt("ServerBufferSize"))
{
    mSocket.DebugOut().Enable(true);
}
bool HTTPServer::ReloadConfig()
{
    bool success = mServerConfig.Reload();
    success &= mServerConfig.Init();
    return success;
}

ExitCodes HTTPServer::Start()
{
    mRun = true;
    //ss.DebugOut().Enable(true);

    if(!mSocket.Create())
    {
        ErrorOut() << "Failed to create server. Giving up." << endl;
        return CREATE_FAILED;
    }
    if(mSocket.Listen() != 0)
    {
        ErrorOut() << "Failed to listen to socket. Giving up." << endl;
        return LISTEN_FAILED;
    }

    mConnectionThread = thread(&HTTPServer::SpawnConnections, this);
    mCommandThread = thread(&HTTPServer::CommandHandler, this);

    mCommandThread.join();
    mConnectionThread.join();

    return SUCCESS;
}
void HTTPServer::SpawnConnections()
{
    SetPrintPrefix(__func__, FUNC_PRINT);
    StandardOut() << endl;
    while(mRun)
    {
        ConnectionHandler connection(mSocket, mServerConfig);
        connection.AcceptConnection();
    }
    StandardOut() << "Done.";
    ClearPrintPrefix();
}
void HTTPServer::CommandHandler()
{
    SetPrintPrefix(__func__, FUNC_PRINT);
    StandardOut() << endl;
    string command;
    while(command != "exit" && mRun)
    {
        getline(cin, command);
        vector<string> commandParts = split(command, ' ');

        if(commandParts.size() > 2 && commandParts.at(0) == "db")
        {
            KvpDB* db = NULL;
            if(commandParts.at(1) == "mime")
            {
                db = &mServerConfig.Mimes();
            }
            else if(commandParts.at(1) == "conf")
            {
                db = &mServerConfig;
            }
            else if(commandParts.at(1) == "resp")
            {
                db = &mServerConfig.StatusDB();
            }
            if(!db)
            {
                ErrorOut() << "Unknown Database '" << commandParts.at(1) << "'" << endl;
            }
            else if(commandParts.at(2) == "reload")
            {
                db->Reload();
            }
            else if(commandParts.at(2) == "print")
            {
                db->Print();
            }
            else if(commandParts.at(2) == "load" && commandParts.size() == 4)
            {
                db->Load(commandParts.at(3));
            }
        }
    }
    StandardOut() << "Done." << endl;
    Stop();
    ClearPrintPrefix();
}
void HTTPServer::Stop()
{
    StandardOut() << "Stopping server." << endl;
    mRun = false;
}
