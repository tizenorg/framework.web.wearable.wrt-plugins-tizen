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

#include "VolumeChangeCallback.h"

#include <PlatformException.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

VolumeChangeCallback::VolumeChangeCallback(JSContextRef global_ctx):
    MultiCallbackUserData(global_ctx)
{
}

VolumeChangeCallback::~VolumeChangeCallback()
{
}


void VolumeChangeCallback::onSuccess()
{
    LOGD("Enter");

    JSContextRef context = getContext();
    JSValueRef type = JSUtil::toJSValueRef(context, SoundUtil::soundTypeToString(m_type));
    JSValueRef volume = JSUtil::toJSValueRef(context, m_volume);

    JSValueRef args[2];
    args[0] = type;
    args[1] = volume;

    invokeCallback("onsuccess", 2, args);
}

gboolean VolumeChangeCallback::complete(void *data)
{
    LOGD("Enter");

    VolumeChangeCallback *cb = static_cast<VolumeChangeCallback *>(data);

    try {
        JSContextRef context = cb->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            throw UnknownException("context was closed");
        }

        cb->onSuccess();
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    return false;
}

sound_type_e VolumeChangeCallback::getType()
{
    return m_type;
}

void VolumeChangeCallback::setType(sound_type_e type)
{
    m_type = type;
}

double VolumeChangeCallback::getVolume()
{
    return m_volume;
}

void VolumeChangeCallback::setVolume(double volume)
{
    m_volume = volume;
}

} // Sound
} // DeviceAPI
