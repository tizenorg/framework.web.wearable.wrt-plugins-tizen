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

#ifndef __TIZEN_CONTENT_UPDATEBATCH_CALLBACK_H__
#define __TIZEN_CONTENT_UPDATEBATCH_CALLBACK_H__

#include <string>
#include <memory>

//#include <IFilter.h>
//#include <SortMode.h>

#include <JavaScriptCore/JavaScript.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include "JSContent.h"
#include "JSImageContent.h"
#include "JSVideoContent.h"
#include "JSAudioContent.h"

#include "ContentCallback.h"
#include "Content.h"
#include <TimeTracer.h>

using namespace DeviceAPI::Common;
//using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

class ContentUpdateBatchCallback : public ContentCallback
{
public:
    ContentUpdateBatchCallback(JSContextRef globalCtx ) : ContentCallback(globalCtx){};

    virtual ~ContentUpdateBatchCallback(){
        m_contents.clear();
    };

    void setContents(ContentLst contents)
    {
        m_contents = contents;
    }
    ContentLst* getContents(){
        return &m_contents;
    };

private:
    ContentLst m_contents;
};

typedef std::shared_ptr<ContentUpdateBatchCallback> ContentUpdateBatchCallbackPtr;

} // Content
} // DeviceAPI



#endif // __TIZEN_CONTENT_UPDATEBATCH_CALLBACK_H__
