#include "Server.hpp"
#include <unistd.h>
#include <ctime>
#include <clews/http/UserAgent.hpp>
#include "ConnectionHandler.hpp"

using namespace std;

HTTPServer::HTTPServer(ServerConfig& serverConfig) : CoreObject("HTTPServer"), mServerConfig(serverConfig),
    mSocketStream(serverConfig.GetInt("ServerPort"), serverConfig.GetInt("ServerBufferSize"))
{
}
bool HTTPServer::ReloadConfig()
{
    bool success = mServerConfig.Reload();
    success &= mServerConfig.Init();
    return success;
}

ExitCodes HTTPServer::Start()
{
    run = true;
    //ss.DebugOut().Enable(true);
    if(!mSocketStream.Create())
    {
        ErrorOut() << "Failed to create server. Giving up." << endl;
        return CREATE_FAILED;
    }

    if(mSocketStream.Listen() != 0)
    {
        ErrorOut() << "Failed to listen to socket. Giving up." << endl;
        return LISTEN_FAILED;
    }
    int conPid = fork();
    if(conPid < 0)
    {
        ErrorOut() << "Failed to start connection handler." << endl;
    }
    else if(conPid == 0)
    {
        StandardOut() << "Starting connection handler." << endl;
        SpawnConnections();
        StandardOut() << "DONE CON." << endl;
        exit(0);
    }

    int status = 0;
    int pidCom = fork();
    if(pidCom < 0)
    {
        ErrorOut() << "[PARENT] Failed to start command handler" << endl;
    }
    else if(pidCom==0)
    {
        //SetPrintPrefix("[CHILD-COM] ");
        StandardOut() << "Starting command handler." << endl;
        CommandHandler();
        StandardOut() << "DONE COM." << endl;
        exit(0);
    }
    waitpid(pidCom, &status, 0);
    StandardOut() << "[PARENT] Exit called. Cleaning up." << endl;

    if(!mSocketStream.Destroy())
    {
        cerr << "Socket did not close cleanly!" << endl;
        return DESTROY_DIRTY;
    }
    return SUCCESS;
}
void HTTPServer::SpawnConnections()
{
    SetPrintPrefix(__func__, FUNC_PRINT);
    while(run)
    {
        ConnectionHandler connection(mSocketStream, mServerConfig);
        connection.AcceptConnection();
    }
}
void HTTPServer::CommandHandler()
{
    StandardOut() << "CommandHandler()" << endl;
    string command;
    do
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
    while (command != "exit" && run == true);
}
void HTTPServer::Stop()
{
    run = false;
}
