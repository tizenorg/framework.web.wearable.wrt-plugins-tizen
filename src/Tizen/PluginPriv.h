//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __TIZEN_TIZEN_PLUGIN_PRIV_H__
#define __TIZEN_TIZEN_PLUGIN_PRIV_H__

#include <map>
#include <set>
#include <memory>
#include <JavaScriptCore/JavaScript.h>
#include <PluginManager/IPluginManager.h>

namespace DeviceAPI {
namespace Tizen {

/**
 * This class is used to store WrtDeviceApis::PluginManager in tizen object
 * and to attach to it other plugins which are it's children.
 */
class PluginPriv {
public:
    PluginPriv(
            const WrtDeviceApis::PluginManager::Api::IPluginManagerPtr &plugin_manager);
    virtual ~PluginPriv();

    WrtDeviceApis::PluginManager::Api::IPluginManagerPtr getPluginManager() const;
    bool propertyInitialized(std::string &property_name);

private:
    WrtDeviceApis::PluginManager::Api::IPluginManagerPtr m_plugin_manager;
    std::set<std::string> m_initialized_properties;
};

typedef std::shared_ptr<PluginPriv> PluginPrivPtr;

} // Tizen
} // DeviceAPI

#endif // __TIZEN_TIZEN_PLUGIN_PRIV_H__
