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

#ifndef __TIZEN_CONTENT_GETDIRECTORY_CALLBACK_H__
#define __TIZEN_CONTENT_GETDIRECTORY_CALLBACK_H__

#include <string>
#include <memory>

#include <JavaScriptCore/JavaScript.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>

#include "ContentDirectory.h"
#include "JSContentDirectory.h"
#include "ContentCallback.h"
#include <TimeTracer.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class ContentGetDirectoryCallback : public ContentCallback
{
public:
    ContentGetDirectoryCallback(JSContextRef globalCtx):ContentCallback(globalCtx){};

    virtual ~ContentGetDirectoryCallback(){
//        for(unsigned int i = 0; i < m_directroies.size(); i++){
//            delete m_directroies[i];
//        }
        m_directroies.clear();
    };

    void onSuccess(){
        JSValueRef valueArray[m_directroies.size()];
        for( unsigned int i = 0 ; i < m_directroies.size(); i++){
            valueArray[i] = JSContentDirectory::CreateJSObject(m_callback->getContext(),m_directroies[i]);
        }
        JSValueRef exception = NULL;
        JSObjectRef jsResult = JSObjectMakeArray(m_callback->getContext(), m_directroies.size(), valueArray, &exception);
        if (exception != NULL) {
            throw DeviceAPI::Common::UnknownException(m_callback->getContext(), exception);
        }
        m_callback->invokeCallback("onsuccess", jsResult);
        TIME_TRACER_ITEM_END(CONTENT_TT_GET_DIR_TOTAL, CONTENT_TIME_TRACER_SHOW);
    };

    DirectoryLst* getDirectories(){
        return &m_directroies;
    };
private:
    DirectoryLst m_directroies;
};


typedef std::shared_ptr<ContentGetDirectoryCallback> ContentGetDirectoryCallbackPtr;

} // Content
} // DeviceAPI



#endif // __TIZEN_CONTENT_GETDIRECTORY_CALLBACK_H__
