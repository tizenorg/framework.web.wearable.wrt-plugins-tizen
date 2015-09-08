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

#include "ISystemSetting.h"
#include "EventSetWallpaper.h"
#include "EventSetRingtone.h"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace SystemSetting {

ISystemSetting::ISystemSetting() :
    EventRequestReceiver< EventSetWallpaper >(ThreadEnum::NULL_THREAD),
    EventRequestReceiver< EventSetRingtone >(ThreadEnum::NULL_THREAD),
    EventRequestReceiver< EventSetSetting >(ThreadEnum::NULL_THREAD),
    EventRequestReceiver< EventGetSetting >(ThreadEnum::NULL_THREAD)
{
}

ISystemSetting::~ISystemSetting()
{
}

}
}
