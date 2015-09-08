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


#ifndef TIZENAPIS_TIZEN_JS_APPLICATION_INFORMATION_H_
#define TIZENAPIS_TIZEN_JS_APPLICATION_INFORMATION_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <dpl/shared_ptr.h>
#include <Security.h>
#include <Logger.h>
#include "ApplicationInformation.h"

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_APPLICATION_INFORMATION "ApplicationInformation"

#define TIZEN_APPLICATION_INFORMATION_ID "id"
#define TIZEN_APPLICATION_INFORMATION_NAME "name"
#define TIZEN_APPLICATION_INFORMATION_ICONPATH "iconPath"
#define TIZEN_APPLICATION_INFORMATION_VERSION "version"
#define TIZEN_APPLICATION_INFORMATION_SHOW "show"
#define TIZEN_APPLICATION_INFORMATION_CATEGORIES "categories"
#define TIZEN_APPLICATION_INFORMATION_INSTALL_DATE "installDate"
#define TIZEN_APPLICATION_INFORMATION_INSTALL_SIZE "size"
#define TIZEN_APPLICATION_INFORMATION_PACKAGE_ID "packageId"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<ApplicationInformationPtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSApplicationInformationPrivObject;

class JSApplicationInformationPriv :
    public JSApplicationInformationPrivObject,
    public DeviceAPI::Common::SecurityAccessor
{
public:
    JSApplicationInformationPriv(JSContextRef context, ApplicationInformationPtr appinfo) :
        JSApplicationInformationPrivObject(context, appinfo),
        DeviceAPI::Common::SecurityAccessor()
    {
        LOGD("Entered");
    }

    virtual ~JSApplicationInformationPriv()
    {
        LOGD("Entered");
    }
};

class JSApplicationInformation {
public:
    /*
    * This initializes this JS class in the JS Engine.
    */
    static JSClassRef getClassRef();

    static JSValueRef createJSObject(JSContextRef context,
            const std::string &name,
            const std::string &package,
            const std::string &iconPath,
            const std::string &version,
            const bool &show,
            const std::vector<std::string> &categories,
            const time_t &installDate,
            const long &installSize,
            const std::string &pkgId);

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    static ApplicationInformationPtr
        getApplicationInformation(JSContextRef context, JSValueRef value);

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

    static ApplicationInformationPtr getPrivData(JSContextRef context, JSObjectRef object);

    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    static JSValueRef getPropertySize(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

};

}
}
#endif
