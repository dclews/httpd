#include "ConnectionHandler.hpp"
#include <clews/http/HttpRequest.hpp>
#include <clews/http/HttpResponse.hpp>
#include "RequestHandler.hpp"
#include <string>
#include <ctime>

using namespace std;

ConnectionHandler::ConnectionHandler(SocketStream& ss, ServerConfig& config) : CoreObject("ConnectionHandler"), mServerConfig(config), mSocketStream(ss) {}

bool ConnectionHandler::AcceptConnection()
{
    SetPrintPrefix(__func__, FUNC_PRINT);
    bool success = false;
    if(mSocketStream.Accept() >= 0)
    {
        string requestStr = mSocketStream.Read();
        HttpRequest request(requestStr);
        bool keepAlive = request["Connection"] != "Close";

        RequestHandler reqHandler(request, mServerConfig);
        HttpResponse resp = reqHandler.GenerateResponse();

        StandardOut() << "Sending Response:" << endl;
        resp.PrintHeader();
        mSocketStream << resp.FullResponse();
        mSocketStream.Close();

        success = true;
    }
    else
    {
        ErrorOut() << "Faield to accept connection." << endl;
    }
    ClearPrintPrefix();
    return success;
}
