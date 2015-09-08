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

#ifndef __TIZEN_CONTENT_CHANGE_CALLBACK_H__
#define __TIZEN_CONTENT_CHANGE_CALLBACK_H__

#include <string>
#include <memory>
#include <list>

#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>

#include "Content.h"
#include "ImageContent.h"
#include "VideoContent.h"
#include "AudioContent.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class ContentChangeCallback
{
public:
    ContentChangeCallback(JSContextRef globalCtx, JSObjectRef object);
    virtual ~ContentChangeCallback();

    void oncontentadded(ContentPtr content);
    void oncontentadded(ImageContentPtr content);
    void oncontentadded(VideoContentPtr content);
    void oncontentadded(AudioContentPtr content);

    void oncontentupdated(ContentPtr content);
    void oncontentupdated(ImageContentPtr content);
    void oncontentupdated(VideoContentPtr content);
    void oncontentupdated(AudioContentPtr content);

    void oncontentremoved(std::string id);

    JSContextRef getContext() const;

private:
    MultiCallbackUserData *m_callback;
};

typedef std::shared_ptr<ContentChangeCallback> ContentChangeCallbackPtr;
typedef std::list<std::shared_ptr<ContentChangeCallback>> ContentChangeCallbackPtrList;

} // Content
} // DeviceAPI



#endif // __TIZEN_CONTENT_CHANGE_CALLBACK_H__
