#include "ServerManager.h"

WebSocketServerManager::~WebSocketServerManager()
{
    for (auto it = wsServers.begin(); it != wsServers.end(); ++it) {
        it->second->stop();
        delete it->second;
    }
}

typedef void (*WSCallback)(std::shared_ptr<ix::WebSocket>, const ix::WebSocketMessagePtr&, std::shared_ptr<ix::ConnectionState>, std::vector<std::any>);

void SetupServer(WebSocketServerManager* _this, std::string ip_addr, uint16_t port)
{
    std::pair<std::string, uint16_t> key{ ip_addr, port };

    auto server = _this->wsServers[key];
    server->setOnConnectionCallback([_this, key](std::weak_ptr<ix::WebSocket> weakSocket,
                                                 std::shared_ptr<ix::ConnectionState> state) {
        if (auto socket = weakSocket.lock()) {
            socket->setOnMessageCallback([_this, key, socket, state](const ix::WebSocketMessagePtr& msg) {
                auto listeners = _this->wsListeners[key];
                for (auto& listener : listeners) {
                    reinterpret_cast<WSCallback>(listener.first)(socket, msg, state, listener.second);
                }
            });
        }
    });

    server->listen();
    server->start();

}

void WebSocketServerManager::SetupWebSocketServer(std::string ip_addr, uint16_t port)
{
    std::pair<std::string, uint16_t> key{ ip_addr, port };
    if(wsServers.find(key) != wsServers.end()) {
        wsServers[key]->stop();
        delete wsServers[key];
    }

    this->wsServers[key] = new ix::WebSocketServer(port, ip_addr);
    std::thread(SetupServer, this, ip_addr, port).detach();
}

void WebSocketServerManager::RegisterServerListener(std::string ip_addr, uint16_t port, void* callback, std::vector<std::any> additional)
{
    std::pair<std::string, uint16_t> key{ ip_addr, port };
    if (wsServers.find(key) == wsServers.end()) SetupWebSocketServer(ip_addr, port);

    wsListeners[key].push_back({ callback, additional });    
}

void WebSocketServerManager::UnregisterServerListener(std::string ip_addr, uint16_t port, void* callback)
{
    std::pair<std::string, uint16_t> key{ ip_addr, port };
    if (wsServers.find(key) == wsServers.end()) SetupWebSocketServer(ip_addr, port);

    for (auto it = wsListeners[key].begin(); it != wsListeners[key].end(); ++it) {
        if (std::any_cast<void*>(it->second[0]) == callback) {
            wsListeners[key].erase(it);
            break;
        }
    }

    if (wsListeners[key].size() == 0) {
        wsServers[key]->stop();
        delete wsServers[key];
        wsServers.erase(key);
    }
}