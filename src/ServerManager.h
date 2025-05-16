#ifndef _websocket_servermanager_h
#define _websocket_servermanager_h

#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXWebSocketMessageType.h>
#include <thread>
#include <map>
#include <vector>
#include <any>
#include <functional>
#include <mutex>

class  WebSocketServerManager
{
private:
    void SetupWebSocketServer(std::string ip_addr, uint16_t port);


public:
    std::map<std::pair<std::string, uint16_t>, ix::WebSocketServer*> wsServers;
    std::map<std::pair<std::string, uint16_t>, std::vector<std::pair<void*, std::vector<std::any>>>> wsListeners;
    ~WebSocketServerManager();

    void RegisterServerListener(std::string ip_addr, uint16_t port, void* callback, std::vector<std::any> additional);
    void UnregisterServerListener(std::string ip_addr, uint16_t port, void* callback);

};


extern WebSocketServerManager* g_websocketServerManager;

#endif