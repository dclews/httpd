#include "ConnectionHandler.hpp"
#include <clews/http/HttpRequest.hpp>
#include <clews/http/HttpResponse.hpp>
#include "RequestHandler.hpp"
#include <string>
#include <ctime>

using namespace std;

ConnectionHandler::ConnectionHandler(Socket& ss, ServerConfig& config) : CoreObject("ConnectionHandler"), mServerConfig(config), mSocketStream(ss) {}

bool ConnectionHandler::AcceptConnection()
{
	SetPrintPrefix(__func__, FUNC_PRINT);
	bool success = false;

	SocketConnection* con = mSocketStream.Accept();
	if(con != NULL)
	{
		con->DebugOut().Enable(true);

		string requestStr = con->Read();
		HttpRequest request(requestStr);
		bool keepAlive = request["Connection"] != "Close";

		RequestHandler reqHandler(request, mServerConfig);
		HttpResponse resp = reqHandler.GenerateResponse();

		StandardOut() << "Sending Response:" << endl;
		resp.PrintHeader();
		*con << resp.FullResponse();
		con->Close();

		success = true;
		delete con;
	}
	else
	{
		ErrorOut() << "Failed to accept connection." << endl;
	}
	ClearPrintPrefix();
	return success;
}
