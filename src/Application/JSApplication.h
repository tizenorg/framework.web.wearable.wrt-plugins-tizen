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


#ifndef TIZENAPIS_TIZEN_JS_APPLICATION_H_
#define TIZENAPIS_TIZEN_JS_APPLICATION_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <memory>
#include <Security.h>
#include "Application.h"

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_APPLICATION "Application"

#define TIZEN_APPLICATION_APP_INFO "appInfo"
#define TIZEN_APPLICATION_APP_CONTEXT_ID "contextId"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<ApplicationPtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSApplicationPriv;

class JSApplication {
public:
    /*
    * This initializes this JS class in the JS Engine.
    */
    static JSClassRef getClassRef();

    static JSValueRef makeObject(JSContextRef ctx,
            const ApplicationPtr value,
            const DeviceAPI::Common::SecurityAccessor *accessor);


    /**
     * hide the application based on application context.
     */
    static JSValueRef hide(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * exit current application
     */
    static JSValueRef exit(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * get request application control
     */
    static JSValueRef getRequestedAppControl(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

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

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    static JSClassRef m_classRef;

};

}
}
#endif  //TIZENAPIS_TIZEN_JS_APPLICATION_H_
