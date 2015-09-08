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

#include "MediaKeyManager.h"
#include "MediaKeyUtil.h"

#include <JSStringRefWrapper.h>
#include <PlatformException.h>
#include <Logger.h>
#include <glib.h>

#include <media_key.h>

namespace DeviceAPI {
namespace MediaKey {

void onMediaKeyEventCb(media_key_e key, media_key_event_e status, void* user_data)
{
    LOGD("Entered");

    MediaKeyManager* mediaKeyManager = MediaKeyManager::getInstance();

    MediaKeyType mediaKeyType = MediaKeyUtil::mediakeyTypeToString(key);
    MediaKeyEventType mediaKeyEventType= MediaKeyUtil::mediakeyEventTypeToString(status);

    mediaKeyManager->onMediaKey(mediaKeyType, mediaKeyEventType);
}


MediaKeyManager::MediaKeyManager() :
    m_listener(MediaKeyEventListenerPtr(NULL)),
    m_listeningPlatformEvent(false)
{
}

MediaKeyManager* MediaKeyManager::getInstance()
{
    static MediaKeyManager instance;
    return &instance;
}


MediaKeyManager::~MediaKeyManager()
{
    m_listener = NULL;

    if(m_listeningPlatformEvent)
    {
        LOGD("::media_key_release");
        int ret = media_key_release();
        if (ret != MEDIA_KEY_ERROR_NONE)
        {
            LOGE("ret : %d", ret);
            MediaKeyUtil::throwMediaKeyException(ret, "media_key_release()");
        }
        m_listeningPlatformEvent = false;
    }
}


void MediaKeyManager::setMediaKeyEventListener(MediaKeyEventListenerPtr listener)
{
    LOGD("Enter MediaKeyManager setMediaKeyEventListener");

    if(m_listener){
        LOGD("delete m_listener");
        m_listener = NULL;
    }

    if(listener){
        m_listener = listener;
    }

    if(!m_listeningPlatformEvent)
    {
        LOGD("::media_key_reserve");
        int ret = media_key_reserve(onMediaKeyEventCb, static_cast<void*>(this));
        if (ret != MEDIA_KEY_ERROR_NONE)
        {
            LOGE("ret : %d", ret);
            MediaKeyUtil::throwMediaKeyException(ret, "media_key_reserve()");
        }
        m_listeningPlatformEvent = true;
    }
}
void MediaKeyManager::unsetMediaKeyEventListener()
{
    LOGD("Enter MediaKeyManager unsetMediaKeyEventListener");
    m_listener = NULL;

    if(m_listeningPlatformEvent)
    {
        LOGD("::media_key_release");
        int ret = media_key_release();
        if (ret != MEDIA_KEY_ERROR_NONE)
        {
            LOGE("ret : %d", ret);
            MediaKeyUtil::throwMediaKeyException(ret, "media_key_release()");
        }
        m_listeningPlatformEvent = false;
    }
}

void MediaKeyManager::onMediaKey(MediaKeyType type, MediaKeyEventType event)
{
        if(!m_listeningPlatformEvent)
        {
                LOGE("event is not registered");
                return;
        }

        if(NULL == m_listener)
        {
                LOGE("event listener is not registered");
                return;
        }


        MediaKeyEventListenerPtr cb = m_listener;
        cb->setMediaKeyType(type);
        MediaKeyEventListenerPtr* data = new MediaKeyEventListenerPtr(cb);

        switch(event)
        {
        case MediaKeyEventType::MEDIA_KEY_EVENT_PRESSED:
                LOGD("event : PRESSED");
                if (!g_idle_add(MediaKeyEventCallback::onpressed, data)) {
                    LOGE("g_idle addition failed");
                    throw UnknownException("g_idle addition failed");
                }
                break;
        case MEDIA_KEY_STATUS_RELEASED:
                LOGD("event : RELEASED");
                if (!g_idle_add(MediaKeyEventCallback::onreleased, data)) {
                    LOGE("g_idle addition failed");
                    throw UnknownException("g_idle addition failed");
                }
                break;
        }
}

} // MediaKey
} // DeviceAPI
