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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _TIZEN_APPLICATION_JS_APPLICATION_EVENT_CALLBACK_MANAGER_H_
#define _TIZEN_APPLICATION_JS_APPLICATION_EVENT_CALLBACK_MANAGER_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <JavaScriptCore/JavaScript.h>
#include <Security.h>

namespace DeviceAPI {
namespace Application {

class JSApplicationEventCallbackManager;
typedef DPL::SharedPtr<JSApplicationEventCallbackManager> JSApplicationEventCallbackManagerPtr;

class JSApplicationEventCallbackManager :
    public WrtDeviceApis::Commons::IEventPrivateData,
    public DeviceAPI::Common::SecurityAccessor
{
private:
    JSApplicationEventCallbackManager() {}
    JSApplicationEventCallbackManager( JSContextRef context,
            JSObjectRef onInstalled = NULL,
            JSObjectRef onUpdated = NULL,
            JSObjectRef onUninstalled = NULL,
            JSObjectRef onError = NULL );

public:
    static JSApplicationEventCallbackManagerPtr createObject(JSContextRef context);

    virtual ~JSApplicationEventCallbackManager();

    void    setContext( JSContextRef context );
    JSContextRef getContext() const { return m_context; }

    void    setOnInstalled( JSValueRef onInstalled );
    JSValueRef getOnInstalled() const;
    void    setOnUpdated( JSValueRef onUpdated );
    JSValueRef getOnUpdated() const;
    void    setOnUninstalled( JSValueRef onUninstalled );
    JSValueRef getOnUninstalled() const;
    void    setOnError( JSValueRef onError );
    JSValueRef getOnError() const;
    void    setObject( JSObjectRef object );
    JSObjectRef getObject() const;

    void    callOnInstalled( JSValueRef contacts );
    void    callOnUpdated( JSValueRef contacts );
    void    callOnUninstalled( JSValueRef contactIds );
    void    callOnError( JSValueRef error );

private:
    void    makeCallback(JSContextRef context,
            JSObjectRef object,
            JSObjectRef callback,
            const char *szName,
            JSValueRef argv[],
            unsigned argc);

    JSObjectRef  m_onInstalled;
    JSObjectRef  m_onUpdated;
    JSObjectRef  m_onUninstalled;
    JSObjectRef  m_onError;
    JSContextRef m_context;
    JSObjectRef  m_object;
};

} // Application
} // DeviceAPI

#endif // _TIZEN_APPLICATION_JS_APPLICATION_EVENT_CALLBACK_MANAGER_H_
