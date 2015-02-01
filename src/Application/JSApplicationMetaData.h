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


#ifndef TIZENAPIS_TIZEN_JS_APPLICATION_META_DATA_H_
#define TIZENAPIS_TIZEN_JS_APPLICATION_META_DATA_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_APPLICATION_META_DATA "ApplicationMetaData"

#define TIZEN_APPLICATION_META_DATA_KEY     "key"
#define TIZEN_APPLICATION_META_DATA_VALUE   "value"


typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<ApplicationMetaDataPtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSApplicationMetaDataPriv;

class JSApplicationMetaData{
public:
    /*
    * This initializes this JS class in the JS Engine.
    */
    static JSClassRef getClassRef();

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

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
     * This member variable contains the initialization values for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_classRef;

    static ApplicationMetaDataPtr getPrivData(JSObjectRef object);

    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

};

}
}
#endif
