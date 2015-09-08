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

#include "JSApplicationEventCallbackManager.h"
#include <WebKitProxy.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

JSApplicationEventCallbackManagerPtr JSApplicationEventCallbackManager::createObject(JSContextRef context)
{
    return JSApplicationEventCallbackManagerPtr( new JSApplicationEventCallbackManager(context) );
}

JSApplicationEventCallbackManager::JSApplicationEventCallbackManager(JSContextRef context,
        JSObjectRef onInstalled,
        JSObjectRef onUpdated,
        JSObjectRef onUninstalled,
        JSObjectRef onError ) :
        m_onInstalled(NULL),
        m_onUpdated(NULL),
        m_onUninstalled(NULL),
        m_onError(NULL),
        m_context(context),
        m_object(NULL)
{
    LOGD("Entered");
    setOnInstalled(onInstalled);
    setOnUpdated(onUpdated);
    setOnUninstalled(onUninstalled);
    setOnError(onError);
}

JSApplicationEventCallbackManager::~JSApplicationEventCallbackManager()
{
    LOGD("Entered");
    if (m_onInstalled)
    {
        JSValueUnprotect(m_context, m_onInstalled);
    }

    if (m_onUpdated)
    {
        JSValueUnprotect(m_context, m_onUpdated);
    }

    if (m_onUninstalled)
    {
        JSValueUnprotect(m_context, m_onUninstalled);
    }

    if (m_onError)
    {
        JSValueUnprotect(m_context, m_onError);
    }
}

void JSApplicationEventCallbackManager::setOnInstalled( JSValueRef onInstalled )
{
    LOGD("Entered");
    if (onInstalled)
    {
        if (m_onInstalled != NULL)
        {
            JSValueUnprotect(m_context, m_onInstalled);
        }

        m_onInstalled = JSValueToObject( m_context, onInstalled, NULL );

        if (m_onInstalled != NULL)
        {
            JSValueProtect(m_context, m_onInstalled);
        }
    }
}

JSValueRef JSApplicationEventCallbackManager::getOnInstalled() const
{
    LOGD("Entered");
    return m_onInstalled;
}

void JSApplicationEventCallbackManager::setOnUpdated( JSValueRef onUpdated )
{
    LOGD("Entered");
    if (onUpdated)
    {
        if (m_onUpdated != NULL)
        {
            JSValueUnprotect(m_context, m_onUpdated);
        }

        m_onUpdated = JSValueToObject( m_context, onUpdated, NULL );

        if (m_onUpdated != NULL)
        {
            JSValueProtect(m_context, m_onUpdated);
        }
    }
}

JSValueRef JSApplicationEventCallbackManager::getOnUpdated() const
{
    LOGD("Entered");
    return m_onUpdated;
}

void JSApplicationEventCallbackManager::setOnUninstalled( JSValueRef onUninstalled )
{
    LOGD("Entered");
    if (onUninstalled)
    {
        if (m_onUninstalled != NULL)
        {
            JSValueUnprotect(m_context, m_onUninstalled);
        }

        m_onUninstalled = JSValueToObject( m_context, onUninstalled, NULL );

        if (m_onUninstalled != NULL)
        {
            JSValueProtect(m_context, m_onUninstalled);
        }
    }
}

JSValueRef JSApplicationEventCallbackManager::getOnUninstalled() const
{
    LOGD("Entered");
    return m_onUninstalled;
}

void JSApplicationEventCallbackManager::setOnError( JSValueRef onError )
{
    LOGD("Entered");
    if (onError)
    {
        if (m_onError != NULL)
        {
            JSValueUnprotect(m_context, m_onError);
        }

        m_onError = JSValueToObject( m_context, onError, NULL );

        if (m_onError != NULL)
        {
            JSValueProtect(m_context, m_onError);
        }
    }
}

JSValueRef JSApplicationEventCallbackManager::getOnError() const
{
    LOGD("Entered");
    return m_onError;
}

void JSApplicationEventCallbackManager::setContext( JSContextRef context )
{
    LOGD("Entered");
    m_context = context;
}

void JSApplicationEventCallbackManager::setObject( JSObjectRef object )
{
    LOGD("Entered");
    m_object = object;
}

JSObjectRef JSApplicationEventCallbackManager::getObject() const
{
    LOGD("Entered");
    return m_object;
}

void JSApplicationEventCallbackManager::callOnInstalled( JSValueRef appInfo )
{
    LOGD("Entered");
    if ( m_onInstalled == NULL )
    {
        return;
    }
    JSValueRef objParam[1] = { appInfo };
    makeCallback( m_context, NULL, m_onInstalled, "oninstalled", objParam, 1 );
}

void JSApplicationEventCallbackManager::callOnUpdated( JSValueRef appInfo )
{
    LOGD("Entered");
    if ( m_onUpdated == NULL )
    {
        return;
    }
    JSValueRef objParam[1] = { appInfo };
    makeCallback( m_context, NULL, m_onUpdated, "onupdated", objParam, 1 );
}

void JSApplicationEventCallbackManager::callOnUninstalled( JSValueRef appId )
{
    LOGD("Entered");
    if ( m_onUninstalled == NULL )
    {
        return;
    }
    JSValueRef objParam[1] = { appId };
    makeCallback( m_context, NULL, m_onUninstalled, "onuninstalled", objParam, 1 );
}

void JSApplicationEventCallbackManager::callOnError( JSValueRef error )
{
    LOGD("Entered");
    if ( m_onError == NULL )
    {
        return;
    }
    JSValueRef objParam[1] = { error };
    makeCallback( m_context, NULL, m_onError, "onerror", objParam, 1 );
}

void JSApplicationEventCallbackManager::makeCallback(JSContextRef context,
        JSObjectRef object,
        JSObjectRef callback,
        const char *szName,
        JSValueRef argv[],
        unsigned argc)
{
    LOGD("Entered");
    if (callback == NULL)
    {
        LOGE("callback cannot be  NULL");
        return;
    }

    if (JSObjectIsFunction(context, callback))
    {
        JSValueRef exception = NULL;

        if (argc == 0)
        {
            JSObjectCallAsFunction(context, callback, object, 0, NULL, &exception);
        }
        else
        {
            JSObjectCallAsFunction(context, callback, object, argc, argv, &exception);
        }

        if (exception)
            DeviceAPI::Common::WebKitProxy::reportException(context, exception);

        return;
    }
}

} // Application
} // DeviceAPI
