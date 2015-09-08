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

#ifndef _TIZEN_JS_PACKAGE_MANAGER_H_
#define _TIZEN_JS_PACKAGE_MANAGER_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>

namespace DeviceAPI {
namespace Package {

#define TIZEN_INTERFACE_PACKAGE_MANAGER "PackageManager"

class JSPackageManager {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

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
     * Install an package
     */
    static JSValueRef install(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Uninstall an package
     */
    static JSValueRef uninstall(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);


    /**
     * Gets the list of installed packages.
     */
    static JSValueRef getPackagesInfo(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Gets the package information of based on the package ID.
     */
    static JSValueRef getPackageInfo(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);


    /**
     * Watch for package installation & uninstallation.
     */
    static JSValueRef setPackageInfoEventListener(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Unset watch for installation & uninstallation
     */
    static JSValueRef unsetPackageInfoEventListener(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    static JSClassRef m_jsClassRef;
};

}
}
#endif
