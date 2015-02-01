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

#ifndef __TIZEN_CONTENT_SCANFILE_CALLBACK_H__
#define __TIZEN_CONTENT_SCANFILE_CALLBACK_H__

#include <string>
#include <memory>

#include <JavaScriptCore/JavaScript.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>

#include "ContentUtility.h"
#include "ContentCallback.h"
#include <TimeTracer.h>

using namespace DeviceAPI::Common;
//using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

class ContentScanFileCallback : public ContentCallback
{
public:
    ContentScanFileCallback(JSContextRef globalCtx, std::string URI):ContentCallback(globalCtx){
        m_contentURI = URI;
    };

    virtual ~ContentScanFileCallback(){
        LOGD("Entered");
    };

    void onSuccess(){
        JSValueRef jsResult = JSUtil::toJSValueRef(
                m_callback->getContext(),ContentUtility::convertPathToUri(m_contentURI));
        if (jsResult == NULL) {
            throw DeviceAPI::Common::UnknownException("This operation is failed by platform.");
        }
        m_callback->invokeCallback("onsuccess", jsResult);
        TIME_TRACER_ITEM_END(CONTENT_TT_SCAN_FILE_TOTAL, CONTENT_TIME_TRACER_SHOW);
    };

    std::string getContentURI(){
        return m_contentURI;
    };

private:
    std::string m_contentURI;
};


typedef std::shared_ptr<ContentScanFileCallback> ContentScanFileCallbackPtr;

} // Content
} // DeviceAPI



#endif // __TIZEN_CONTENT_GETDIRECTORY_CALLBACK_H__
