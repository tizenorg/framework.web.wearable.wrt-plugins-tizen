//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include <PlatformException.h>
#include <Logger.h>
#include <JSStringRefWrapper.h>
#include <JSUtil.h>
#include <GlobalContextManager.h>
#include "MediaKeyManager.h"
#include "MediaKeyEventCallback.h"

#define PRESSED_CALLBACK "onpressed"
#define RELEASED_CALLBACK "onreleased"

namespace DeviceAPI {
namespace MediaKey {

void MediaKeyEventCallback::setMediaKeyType(MediaKeyType type){
    m_type = type;
}

MediaKeyEventCallback::MediaKeyEventCallback(JSContextRef context, JSObjectRef onpressed, JSObjectRef onreleased)
:MultiCallbackUserData(context)
{
    LOGD("Entered");
    setCallback(PRESSED_CALLBACK, onpressed);
    setCallback(RELEASED_CALLBACK, onreleased);
}

MediaKeyEventCallback::~MediaKeyEventCallback()
{
}

gboolean MediaKeyEventCallback::onpressed(void *data)
{
        LOGD("Enter");
        MediaKeyEventListenerPtr *holder = static_cast<MediaKeyEventListenerPtr *>(data);
        MediaKeyEventListenerPtr cb = *holder;

        try {
            JSContextRef context = cb->getContext();
            if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
                LOGE("context was closed");
                throw UnknownException("context was closed");
            }

        std::string typeStr;

        switch(cb->m_type)
        {
        case MEDIA_KEY_TYPE_PLAY:
                typeStr = "PLAY";
                break;
        case MEDIA_KEY_TYPE_STOP:
                typeStr = "STOP";
                break;
        case MEDIA_KEY_TYPE_PAUSE:
                typeStr = "PAUSE";
                break;
        case MEDIA_KEY_TYPE_PREVIOUS:
                typeStr = "PREVIOUS";
                break;
        case MEDIA_KEY_TYPE_NEXT:
                typeStr = "NEXT";
                break;
        case MEDIA_KEY_TYPE_FASTFORWARD:
                typeStr = "FASTFORWARD";
                break;
        case MEDIA_KEY_TYPE_REWIND:
                typeStr = "REWIND";
                break;
        case MEDIA_KEY_TYPE_PLAYPAUSE:
                typeStr = "PLAYPAUSE";
                break;
        default:
                LOGE("Wrong type value");
                break;
        }

        cb->invokeCallback(PRESSED_CALLBACK, JSUtil::toJSValueRef(context, typeStr));

        } catch (BasePlatformException &err) {
            LOGE("%s : %s", (err.getName()).c_str(), (err.getMessage()).c_str());
        } catch (...) {
            LOGE("Something strange happened");
        }

        delete holder;
        return false;
}

gboolean MediaKeyEventCallback::onreleased(void *data)
{
        LOGD("Enter");
        MediaKeyEventListenerPtr *holder = static_cast<MediaKeyEventListenerPtr *>(data);
        MediaKeyEventListenerPtr cb = *holder;

        try {
            JSContextRef context = cb->getContext();
            if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
                LOGE("context was closed");
                throw UnknownException("context was closed");
            }

        std::string typeStr;

        switch(cb->m_type)
        {
        case MEDIA_KEY_TYPE_PLAY:
                typeStr = "PLAY";
                break;
        case MEDIA_KEY_TYPE_STOP:
                typeStr = "STOP";
                break;
        case MEDIA_KEY_TYPE_PAUSE:
                typeStr = "PAUSE";
                break;
        case MEDIA_KEY_TYPE_PREVIOUS:
                typeStr = "PREVIOUS";
                break;
        case MEDIA_KEY_TYPE_NEXT:
                typeStr = "NEXT";
                break;
        case MEDIA_KEY_TYPE_FASTFORWARD:
                typeStr = "FASTFORWARD";
                break;
        case MEDIA_KEY_TYPE_REWIND:
                typeStr = "REWIND";
                break;
        case MEDIA_KEY_TYPE_PLAYPAUSE:
                typeStr = "PLAYPAUSE";
                break;
        default:
                LOGE("Wrong type value");
                break;
        }

        cb->invokeCallback(RELEASED_CALLBACK, JSUtil::toJSValueRef(context, typeStr));

        } catch (BasePlatformException &err) {
            LOGE("%s : %s", (err.getName()).c_str(), (err.getMessage()).c_str());
        } catch (...) {
            LOGE("Something strange happened");
        }

        delete holder;
        return false;
}

} // MediaKey
} // DeviceAPI
