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

#ifndef __TIZEN_CONTENT_FIND_CALLBACK_H__
#define __TIZEN_CONTENT_FIND_CALLBACK_H__

#include <string>
#include <memory>

#include <AbstractFilter.h>
#include <SortMode.h>

#include <JavaScriptCore/JavaScript.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include "JSContent.h"
#include "JSImageContent.h"
#include "JSVideoContent.h"
#include "JSAudioContent.h"




#include "ContentCallback.h"
#include "Content.h"


using namespace DeviceAPI::Common;
using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

class ContentFindCallback : public ContentCallback
{
public:
    ContentFindCallback(JSContextRef globalCtx ) : ContentCallback(globalCtx){
        m_count = -1;
        m_offset =  0;
    };

    virtual ~ContentFindCallback(){
        m_contents.clear();
    };

    void onSuccess(){
        JSValueRef valueArray[m_contents.size()];
        for( unsigned int i = 0 ; i < m_contents.size(); i++){
            if(std::dynamic_pointer_cast<ImageContent>(m_contents[i])){
                valueArray[i] = JSImageContent::CreateJSObject(m_callback->getContext(), m_contents[i]);
            }
            else if(std::dynamic_pointer_cast<VideoContent>(m_contents[i])){
                valueArray[i] = JSVideoContent::CreateJSObject(m_callback->getContext(), m_contents[i]);
            }
            else if(std::dynamic_pointer_cast<AudioContent>(m_contents[i])){
                valueArray[i] = JSAudioContent::CreateJSObject(m_callback->getContext(), m_contents[i]);
            }
            else if(m_contents[i]){
                valueArray[i] = JSContent::CreateJSObject(m_callback->getContext(), m_contents[i]);
            }
        }
        JSValueRef exception = NULL;
        JSObjectRef jsResult = JSObjectMakeArray(m_callback->getContext(), m_contents.size(), valueArray, &exception);
        if (exception != NULL) {
            throw DeviceAPI::Common::UnknownException(m_callback->getContext(), exception);
        }
        m_callback->invokeCallback("onsuccess", jsResult);
        TIME_TRACER_ITEM_END(CONTENT_TT_FIND_TOTAL, CONTENT_TIME_TRACER_SHOW);
    };

    std::string getDirectoryId(){
        return m_directoryId;
    }

    void setDirectoryId(std::string& directoryId){
        m_directoryId = directoryId;
    }

    AbstractFilterPtr& getFilter(){
        return m_filter;
    };

    void  setFilter(AbstractFilterPtr& filter){
        m_filter = filter;
    };

    SortModePtr& getSortMode(){
        return m_sortMode;
    }

    void  setSortMode(SortModePtr& sortMode){
        m_sortMode = sortMode;
    }

    unsigned long getCount(){
        return m_count;
    }

    void setCount(unsigned long count){
        m_count = count;
    }

    unsigned long getOffset(){
        return m_offset;
    }

    void setOffset(unsigned long offset){
        m_offset = offset;
    }

    ContentLst* getContents(){
        return &m_contents;
    };

private:
    std::string m_directoryId;
    AbstractFilterPtr m_filter;
    SortModePtr m_sortMode;
    unsigned long m_count;
    unsigned long m_offset;

    ContentLst m_contents;
};

typedef std::shared_ptr<ContentFindCallback> ContentFindCallbackPtr;

} // Content
} // DeviceAPI

#endif // __TIZEN_CONTENT_FIND_CALLBACK_H__
