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

/**
 * @file        JSStorage.h
 */

#ifndef TIZENAPIS_TIZEN_JS_STORAGE_H_
#define TIZENAPIS_TIZEN_JS_STORAGE_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "StorageProperties.h"
#include <SecurityAccessor.h>

namespace DeviceAPI {
namespace Filesystem {

class JSStoragePriv : public Common::SecurityAccessor
{
public:
    JSStoragePriv();
    virtual ~JSStoragePriv();

    void setProperties(StoragePropertiesPtr newProp);
    StoragePropertiesPtr getProperties() const;

    JSContextRef getContext() const;
    void setContext(JSContextRef context);

private:
    StoragePropertiesPtr m_storageProperties;
    JSContextRef m_context;
};

class JSStorage
{
public:
    enum StorageType
    {
        TYPE_INTERNAL = 0,
        TYPE_EXTERNAL,
    };

    enum StorageState
    {
        STATE_MOUNTED = 0,
        STATE_REMOVED,
        STATE_UNMOUNTABLE,
    };

    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    /**
    * create an JSObject for callback function(onAnswerReceived).
    */
    static JSObjectRef createJSObject(JSContextRef context,
            const StorageProperties &storages,
            Common::SecurityAccessor* srcSecurityAccessor);

private:
    /**
    * The callback invoked when an object is first created.
    */
    static void initialize(JSContextRef context,
        JSObjectRef object);

    /**
    * The callback invoked when an object is finalized.
    */
    static void finalize(JSObjectRef object);

    /**
    * The callback invoked when getting a property's value.
    */
    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    /**
    * The callback invoked when an object is used as the target of an 'instanceof' expression.
    */
    static bool hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef possibleInstance,
        JSValueRef* exception);

    /**
    * This structure contains properties and callbacks that define a type of object.
    */
    static JSClassDefinition m_classInfo;

    /**
    * This structure describes a statically declared value property.
    */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};
}
}

#endif

