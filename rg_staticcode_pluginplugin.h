#ifndef RG_STATICCODE_PLUGIN_H
#define RG_STATICCODE_PLUGIN_H

#include "rg_staticcode_plugin_global.h"

#include <extensionsystem/iplugin.h>

namespace RG_StaticCode_Plugin {
namespace Internal {

class RG_StaticCode_PluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "RG_StaticCode_Plugin.json")
    
public:
    RG_StaticCode_PluginPlugin();
    ~RG_StaticCode_PluginPlugin();
    
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
    
private slots:
    void triggerAction();
};

} // namespace Internal
} // namespace RG_StaticCode_Plugin

#endif // RG_STATICCODE_PLUGIN_H

