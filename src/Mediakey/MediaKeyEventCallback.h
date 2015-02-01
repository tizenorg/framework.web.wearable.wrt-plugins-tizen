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

#ifndef __TIZEN_MEDIA_KEY_EVENT_CALLBACK_H__
#define __TIZEN_MEDIA_KEY_EVENT_CALLBACK_H__

#include <MultiCallbackUserData.h>
#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <glib.h>


using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace MediaKey {

enum MediaKeyType
{
        MEDIA_KEY_TYPE_PLAY,
        MEDIA_KEY_TYPE_STOP,
        MEDIA_KEY_TYPE_PAUSE,
        MEDIA_KEY_TYPE_PREVIOUS,
        MEDIA_KEY_TYPE_NEXT,
        MEDIA_KEY_TYPE_FASTFORWARD,
        MEDIA_KEY_TYPE_REWIND,
        MEDIA_KEY_TYPE_PLAYPAUSE
};

enum MediaKeyEventType
{
        MEDIA_KEY_EVENT_PRESSED,
        MEDIA_KEY_EVENT_RELEASED
};



class MediaKeyEventCallback : MultiCallbackUserData
{
public:
    MediaKeyEventCallback(JSContextRef globalCtx, JSObjectRef pressedCallback, JSObjectRef releasedCallback);
    virtual ~MediaKeyEventCallback();

    void setMediaKeyType(MediaKeyType type);
    static gboolean onpressed(void *data);
    static gboolean onreleased(void *data);

private:
    MediaKeyType m_type;
};

} // MediaKey
} // DeviceAPI

#endif // __TIZEN_MEDIA_KEY_EVENT_CALLBACK_H__
