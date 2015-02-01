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

#ifndef TIZENAPIS_TIZEN_JS_APPLICATION_MANAGER_H_
#define TIZENAPIS_TIZEN_JS_APPLICATION_MANAGER_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "IApplicationManager.h"

namespace DeviceAPI {
namespace Application {

#define TIZEN_INTERFACE_APPLICATION_MANAGER "ApplicationManager"

class JSApplicationManager {
public:
    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    //static void setTitleProperty(JSContextRef context, std::string propertyValue);

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
     * Gets a current application
     */
    static JSValueRef getCurrentApplication(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Launches a application with given application id
     */
    static JSValueRef launch(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Kills the application based on application context.
     */
    static JSValueRef kill(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);


    /**
     * exit current application
     */
    static JSValueRef setUserAgent(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Gets the list of installed packages.
     */
    static JSValueRef getAppsInfo(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Gets the list of running packages.
     */
    static JSValueRef getAppsContext(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Gets the application information of based on the package name.
     */
    static JSValueRef getAppInfo(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Gets the current application Context
     */
    static JSValueRef getAppContext(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Watch for changes on installed applicaiton.
     */
    static JSValueRef addAppInfoEventListener(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Unset installed Applications listener
     */
    static JSValueRef removeAppInfoEventListener(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Launch a application control
     */
    static JSValueRef launchAppControl(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Find application info list with application control
     */
    static JSValueRef findAppControl(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Get certificate information of given application
     */
    static JSValueRef getAppCerts(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Get shared directory URI of specific application
     */
    static JSValueRef getAppSharedURI(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef* exception);

    /**
     * Get meta data of specific application
     */
    static JSValueRef getAppMetaData(JSContextRef context,
            JSObjectRef object, JSObjectRef thisObject, size_t argumentCount,
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
