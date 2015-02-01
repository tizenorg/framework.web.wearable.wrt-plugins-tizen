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

#include "PluginPriv.h"

namespace DeviceAPI {
namespace Tizen {

using namespace WrtDeviceApis::PluginManager::Api;

PluginPriv::PluginPriv(const IPluginManagerPtr &plugin_manager) :
        m_plugin_manager(plugin_manager)
{
}

PluginPriv::~PluginPriv()
{
}

IPluginManagerPtr PluginPriv::getPluginManager() const
{
    return m_plugin_manager;
}

bool PluginPriv::propertyInitialized(std::string &property_name)
{
    if (m_initialized_properties.find(property_name) ==
            m_initialized_properties.end()) {
        m_initialized_properties.insert(property_name);
        return false;
    }

    return true;
}

} // Tizen
} // DeviceAPI
