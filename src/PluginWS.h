
#ifndef _pluginws_h
#define _pluginws_h

#include <string>
#include <vector>
#include <any>
#include <Embedder.h>


class PluginWS
{
private:
    std::string plugin_name;
    std::vector<std::vector<std::any>> toDelete;

public:
    PluginWS(std::string m_plugin_name);
    ~PluginWS();

    void Listen(std::string ip_addr, uint16_t port, EValue callback);;
};

#endif