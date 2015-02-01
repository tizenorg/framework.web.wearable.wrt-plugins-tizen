//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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


#ifndef _BADGE_JS_BADGE_MANAGER_H_
#define _BADGE_JS_BADGE_MANAGER_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <string>

namespace DeviceAPI {
namespace Badge {

class JSBadgeManager {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
private:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static bool hasInstance(JSContextRef context, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception);

    static JSValueRef getMaxBadgeCount(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef setBadgeCount (
        JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception);

    static JSValueRef getBadgeCount (
        JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception);

    static JSValueRef addChangeListener (
        JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception);

    static JSValueRef removeChangeListener (
        JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception);

    static JSClassDefinition m_classInfo;
    static JSStaticValue m_property[];
    static JSStaticFunction m_function[];
    static JSClassRef m_jsClassRef;
};

}
}

#endif

