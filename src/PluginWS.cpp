#include "PluginWS.h"
#include "ServerManager.h"
#include "entrypoint.h"
#include "utils.h"

#include <swiftly-ext/event.h>
#include <swiftly-ext/callstack.h>
#include <embedder/src/Embedder.h>


PluginWS::PluginWS(std::string m_plugin_name)
{
    REGISTER_CALLSTACK(this->plugin_name, string_format("PluginWS::PluginWS(m_plugin_name=\"%s\")", m_plugin_name.c_str()));

    this->plugin_name = m_plugin_name;
}

PluginWS::~PluginWS()
{
    for (auto val : toDelete) {
        std::string ip_addr = std::any_cast<std::string>(val[0]);
        uint16_t port = std::any_cast<uint16_t>(val[1]);
        if(val[2].type() == typeid(void*)) {
            g_websocketServerManager->UnregisterServerListener(ip_addr, port, std::any_cast<void*>(val[2]));
        }
    }
}

void PluginWS::Listen(std::string ip_addr, uint16_t port, EValue callback)
{
    REGISTER_CALLSTACK(this->plugin_name, string_format("PluginWS::Listen(ip_addr=\"%s\", port=%d, callback=%p)", ip_addr.c_str(), port, callback));

    auto revcb = new EValue(callback);

    g_websocketServerManager->RegisterServerListener(ip_addr, port, (void*)revcb, { (void*)revcb });

    toDelete.push_back({ ip_addr, port, (void*)revcb });
}