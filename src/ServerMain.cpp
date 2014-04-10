#include "ServerMain.hpp"

using namespace std;

HTTPServer* server;

void signalHandler(int signo)
{
    switch(signo)
    {
        case SIGINT:cout << "SIGINT Recieved." << endl; server->Stop(); break;
        case SIGSTOP:cout << "SIGSTOP" << endl; break;
        case SIGKILL: cout << "SIGKILL" << endl; break;
    }
}
void mapSignalHandlers()
{
    if(signal(SIGINT, signalHandler) == SIG_ERR)
    {
        cerr << "Failed to map SIGINT handler." << endl;
    }
    if(signal(SIGKILL, signalHandler) == SIG_ERR)
    {
        cerr << "Failed to map SIGKILL handler." << endl;
    }
    if(signal(SIGSTOP, signalHandler) == SIG_ERR)
    {
        cerr << "Failed to map SIGSTOP handler." << endl;
    }
}

int main(int argc, char** argv)
{
    ServerConfig serverConfig;
    serverConfig.Load("res/DB/Server.config");
    serverConfig.Init();

    HTTPServer os(serverConfig);
    server = &os;
    mapSignalHandlers();
    return os.Start();
}
