#include "entrypoint.h"
#include "ServerManager.h"
#include "PluginWS.h"

//////////////////////////////////////////////////////////////
/////////////////        Core Variables        //////////////
////////////////////////////////////////////////////////////
CREATE_GLOBALVARS();
WebSocketExtension g_Ext;

WebSocketServerManager* g_websocketServerManager = nullptr;


//////////////////////////////////////////////////////////////
/////////////////          Core Class          //////////////
////////////////////////////////////////////////////////////

EXT_EXPOSE(g_Ext);
bool WebSocketExtension::Load(std::string& error, SourceHook::ISourceHook* SHPtr, ISmmAPI* ismm, bool late)
{
    SAVE_GLOBALVARS();

    g_websocketServerManager = new WebSocketServerManager();
    return true;
}

bool WebSocketExtension::Unload(std::string& error)
{
    delete g_websocketServerManager;
    return true;
}

void WebSocketExtension::AllExtensionsLoaded()
{

}

void WebSocketExtension::AllPluginsLoaded()
{

}

bool WebSocketExtension::OnPluginLoad(std::string pluginName, void* pluginState, PluginKind_t kind, std::string& error)
{
    EContext* ctx = (EContext*)pluginState;
    
    ADD_CLASS("WebSocket");

    ADD_CLASS_FUNCTION("WebSocket", "~WebSocket", [](FunctionContext* context, ClassData* data) -> void {
        delete data->GetData<PluginWS*>("pwebsocket");
    });

    ADD_CLASS_FUNCTION("WebSocket", "Listen", [](FunctionContext* context, ClassData* data) -> void {
        std::string ip = context->GetArgumentOr<std::string>(0, "0.0.0.0");
        int port = context->GetArgumentOr<int>(1, 3000);
        EValue callback = context->GetArgument<EValue>(2);
        data->GetData<PluginWS*>("pwebsocket")->Listen(ip, port, callback);
    });

    ADD_VARIABLE("_G", "websocket", MAKE_CLASS_INSTANCE_CTX(ctx, "WebSocket", { { "pwebsocket", new PluginWS(pluginName) } }));

    return true;
}

bool WebSocketExtension::OnPluginUnload(std::string pluginName, void* pluginState, PluginKind_t kind, std::string& error)
{
    return true;
}

const char* WebSocketExtension::GetAuthor()
{
    return "Energykill & Swiftly Development Team";
}

const char* WebSocketExtension::GetName()
{
    return "WebSocket Extension";
}

const char* WebSocketExtension::GetVersion()
{
#ifndef VERSION
    return "Local";
#else
    return VERSION;
#endif
}

const char* WebSocketExtension::GetWebsite()
{
    return "https://energykill.net/";
}
