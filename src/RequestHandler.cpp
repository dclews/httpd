#include "RequestHandler.hpp"
#include <clews/utility/FileUtil.hpp>
#include <clews/http/HttpCommon.hpp>
#include "ServerUtil.hpp"

using namespace std;

RequestHandler::RequestHandler(HttpRequest& req, ServerConfig& serverConfig) : CoreObject("RequestHandler"), mServerConfig(serverConfig), mRequest(req) {}

HttpResponse RequestHandler::GenerateResponse()
{
    HttpResponse resp(mServerConfig["HTTPVersion"], mServerConfig.StatusDB());

    uint32_t status = 0;
    if(mRequest.RequestType() == "GET")
    {
        status = LoadContent(resp, mRequest.RequestedFile());
    }
    if(status != HTTP_OK)
    {
        status = HandleError(resp, status);
    }
    resp.SetStatusCode(status);
    time_t now = time(0);
    resp.SetDate(localtime(&now));

    return resp;
}
uint32_t RequestHandler::HandleError(HttpResponse& resp, uint32_t errorCode)
{
    uint32_t status = LoadFileUnsafe(resp, statusPagePath(mServerConfig, errorCode));
    if(status != HTTP_OK)
    {
        LoadFileUnsafe(resp, statusPagePath(mServerConfig, HTTP_SERVER_ERROR));
    }
    return status;
}
uint32_t RequestHandler::LoadContent(HttpResponse& resp, string url)
{
    uint32_t status = 0;

    SetPrintPrefix(__func__, FUNC_PRINT);

    StandardOut() << "Request for: " << url << endl;

    string webRoot = mServerConfig["WebRoot"];
    url = webRoot + url;

    StandardOut() << "Rel path: " << url << endl;
    if(url.at(url.length()-1) == '/')
    {
        url+=mServerConfig["IndexPage"];
    }
    else
    {
        url = getFullPath(url);
        if(!mServerConfig.ValidatePath(url))
        {
            status = HTTP_FORBIDDEN;
        }
    }

    if(status != HTTP_FORBIDDEN)
    {
        StandardOut() << "Retrieving file: " + url << endl;
        status = LoadFileUnsafe(resp, url);
    }
    else
    {
        ErrorOut() << "Not retrieving file, access forbidden: " << url << endl;
    }

    ClearPrintPrefix();

    return status;
}
uint32_t RequestHandler::LoadFileUnsafe(HttpResponse& resp, string url)
{
    uint32_t status = 0;

    SetPrintPrefix(__func__, FUNC_PRINT);
    StandardOut() << "Loading file: " + url << endl;

    HttpResource res(url, mServerConfig.Mimes());
    status = res.Load() ? HTTP_OK : HTTP_NOT_FOUND;

    MimeType mimeType = res.Mime();
    resp.SetContentType(mimeType.RawType());
    resp.SetContent(res.Content());
    ClearPrintPrefix();
    return status;
}
