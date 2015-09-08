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

#ifndef _JS_TIZEN_POWER_MANAGER_H_
#define _JS_TIZEN_POWER_MANAGER_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <Security.h>
#include <CallbackUserData.h>

namespace DeviceAPI {
namespace Power {

class PowerManagerPriv : public DeviceAPI::Common::CallbackUserData, public DeviceAPI::Common::SecurityAccessor
{
public:
    PowerManagerPriv(JSContextRef ctx) : DeviceAPI::Common::CallbackUserData(ctx), DeviceAPI::Common::SecurityAccessor() {}
    virtual ~PowerManagerPriv() {}
};

class JSPowerManager
{
  public:

    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

  private:

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

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
     * Requests a power resource.
     */
    static JSValueRef request(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Releases a previous power request.
     */
    static JSValueRef release(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Sets the screen state change listener.
     */
    static JSValueRef setScreenStateChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Unsets the screen state change listener.
     */
    static JSValueRef unsetScreenStateChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Gets the screen brightness.
     */
    static JSValueRef getScreenBrightness(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Sets the screen brightness.
     */
    static JSValueRef setScreenBrightness(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Returns true if screen is on.
     */
    static JSValueRef isScreenOn(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Restore the screen brightensss to the default setting value.
     */
    static JSValueRef restoreScreenBrightness(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Turns the screen on.
     */
    static JSValueRef turnScreenOn(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Turns the screen off.
     */
    static JSValueRef turnScreenOff(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

}
}

#endif /* _JS_TIZEN_POWER_MANAGER_H_ */

