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


#ifndef TIZENAPIS_TIZEN_JS_REQUESTED_APPLICATION_CONTROL_H_
#define TIZENAPIS_TIZEN_JS_REQUESTED_APPLICATION_CONTROL_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "ApplicationControl.h"
#include "RequestedApplicationControl.h"
#include "JSApplicationControl.h"
#include "JSApplicationControlData.h"

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_REQUESTED_APPLICATION_CONTROL "RequestedApplicationControl"
#define TIZEN_APPLICATION_CONTROL "appControl"
#define TIZEN_CALLER_APP_ID "callerAppId"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObjectT<RequestedApplicationControlPtr>::Type JSRequestedApplicationControlPriv;

class JSRequestedApplicationControl {
public:
    static const JSClassDefinition* getClassInfo();

    static JSClassRef getClassRef();

    /**
      * create an JSObject for callback function(onAnswerReceived).
      */
    static JSObjectRef createJSObject(JSContextRef context,
               const RequestedApplicationControlPtr &appsvc);


    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    /**
     * Send Success Result
     */
    static JSValueRef replyResult(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Send Failure Result
     */
    static JSValueRef replyFailure(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static RequestedApplicationControlPtr getRequestedApplicationControl(JSContextRef context,
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

    static RequestedApplicationControlPtr getPrivData(JSObjectRef object);


  /**
     * The callback invoked when getting a property's value.
     */
    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

  /**
     * The callback invoked when setting a property's value.
     */

    static bool setProperty(JSContextRef context,
          JSObjectRef object,
          JSStringRef propertyName,
          JSValueRef value,
          JSValueRef* exception);


  /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

  /**
     * This structure describes a statically declared value property.
     */
    static JSStaticValue m_property[];

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];


    static JSClassRef m_jsClassRef;

};

} //
} //TizenApis

#endif
