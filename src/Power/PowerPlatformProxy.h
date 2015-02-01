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

#ifndef _DEVICEAPI_POWER_POWERPLATFORMPROXY_H_
#define _DEVICEAPI_POWER_POWERPLATFORMPROXY_H_

#include <DBusOperation.h>

namespace DeviceAPI {
namespace Power {

class PowerPlatformProxy
{
private:
    PowerPlatformProxy();

public:
    virtual ~PowerPlatformProxy();

    int deviceLockState();
    int deviceUnlockState();
    int deviceSetBrightnessFromSettings();
    int deviceSetBrightness(int val);

private:
    DeviceAPI::Common::DBusOperation m_dbusOp;

public:
    static PowerPlatformProxy& getInstance();
};

}
}

#endif /* _DEVICEAPI_POWER_POWERPLATFORMPROXY_H_ */
