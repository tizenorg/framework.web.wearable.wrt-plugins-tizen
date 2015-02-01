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

#ifndef TIZENAPIS_TIZEN_JS_APPLICATION_CONTEXT_H_
#define TIZENAPIS_TIZEN_JS_APPLICATION_CONTEXT_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <dpl/shared_ptr.h>
#include "ApplicationContext.h"

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_APPLICATION_CONTEXT "ApplicationContext"

#define TIZEN_APPLICATION_CONTEXT_ID "id"
#define TIZEN_APPLICATION_CONTEXT_APP_ID "appId"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<ApplicationContextPtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSApplicationContextPriv;

class JSApplicationContext {
public:
    /*
    * This initializes this JS class in the JS Engine.
    */
    static JSClassRef getClassRef();

    static JSValueRef createJSObject(JSContextRef context,
            const std::string &appId,
            const std::string &contextId);

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    static ApplicationContextPtr getApplicationContext(JSContextRef context,
            JSValueRef value);

private:
   /**
    * The callback invoked when an object is first created.
    */
    static void initialize(JSContextRef context, JSObjectRef object);

   /**
    * The callback invoked when an object is finalized.
    */
    static void finalize(JSObjectRef object);

   /**
    * This structure contains properties and callbacks that define a type of object.
    */
    static JSClassDefinition m_classInfo;

   /**
    * This structure describes a statically declared function property.
    */
    static JSStaticFunction m_functions[];

    /**
     * This member variable contains the initialization values for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_classRef;

    static ApplicationContextPtr getPrivData(JSContextRef context, JSObjectRef object);

    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

};

}
}
#endif
