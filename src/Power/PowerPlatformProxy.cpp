//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include "PowerPlatformProxy.h"

#include <algorithm>
#include <Logger.h>

namespace DeviceAPI {
namespace Power {

PowerPlatformProxy::PowerPlatformProxy() :
    m_dbusOp("org.tizen.system.deviced", "/Org/Tizen/System/DeviceD/Display", "org.tizen.system.deviced.display")
{
}

PowerPlatformProxy::~PowerPlatformProxy()
{
}

int PowerPlatformProxy::deviceLockState()
{
    DeviceAPI::Common::DBusOperationArguments args;
    args.addArgumentString("lcddim");
    args.addArgumentString("staycurstate");
    args.addArgumentString("NULL");
    args.addArgumentInt32(0);

    return m_dbusOp.invokeSyncGetInt("lockstate", &args);
}

int PowerPlatformProxy::deviceUnlockState()
{
    DeviceAPI::Common::DBusOperationArguments args;
    args.addArgumentString("lcddim");
    args.addArgumentString("keeptimer");

    return m_dbusOp.invokeSyncGetInt("unlockstate", &args);
}

int PowerPlatformProxy::deviceSetBrightnessFromSettings()
{
    return m_dbusOp.invokeSyncGetInt("ReleaseBrightness", NULL);
}

int PowerPlatformProxy::deviceSetBrightness(int val)
{
    DeviceAPI::Common::DBusOperationArguments args;
    args.addArgumentInt32(val);

    return m_dbusOp.invokeSyncGetInt("HoldBrightness", &args);
}

PowerPlatformProxy& PowerPlatformProxy::getInstance()
{
    static PowerPlatformProxy instance;

    return instance;
}

} //Power
} //DeviceAPI
