//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "SoundDeviceStateChangeCallback.h"
#include "JSSoundDeviceInfo.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Sound {

SoundDeviceStateChangeCallback::SoundDeviceStateChangeCallback(JSContextRef global_ctx):
    MultiCallbackUserData(global_ctx)
{
    LOGD("Entered");

    m_info = NULL;
}

SoundDeviceStateChangeCallback::~SoundDeviceStateChangeCallback()
{
    LOGD("Entered");
}

void SoundDeviceStateChangeCallback::onChanged()
{
    LOGD("Entered");

    JSContextRef context = getContext();
    JSObjectRef Obj = JSSoundDeviceInfo::makeJSObject(context, m_info);

    invokeCallback("onchanged", Obj);

    return;
}

void SoundDeviceStateChangeCallback::setSoundDeviceInfo(SoundDeviceInfoPtr info)
{
    LOGD("Entered");

    m_info = info;
}

SoundDeviceInfoPtr SoundDeviceStateChangeCallback::getSoundDeviceInfo()
{
    LOGD("Entered");

    return m_info;
}

} // Sound
} // DeviceAPI