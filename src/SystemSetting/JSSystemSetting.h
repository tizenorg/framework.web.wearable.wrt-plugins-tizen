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

#ifndef WRTPLUGINS_TIZEN1_0_JS_SYSTEMSETTING_H_
#define WRTPLUGINS_TIZEN1_0_JS_SYSTEMSETTING_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <Security.h>
#include "ISystemSetting.h"

using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace SystemSetting {

typedef PrivateObject<ISystemSettingPtr, NoOwnership> JSSystemSettingPrivObject;

class JSSystemSettingPriv :
    public JSSystemSettingPrivObject,
    public DeviceAPI::Common::SecurityAccessor
{
public:
    JSSystemSettingPriv(JSContextRef context, ISystemSettingPtr systemSetting) :
        JSSystemSettingPrivObject(context, systemSetting),
        DeviceAPI::Common::SecurityAccessor()
    {
    }

    virtual ~JSSystemSettingPriv()
    {
    }
};

class JSSystemSetting
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

private:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);

    static JSValueRef setProperty(JSContextRef context, JSObjectRef object, JSObjectRef thisObject,
            size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSObjectRef thisObject,
            size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static JSClassDefinition m_classInfo;
    static JSStaticFunction m_function[];

    static JSClassRef m_jsClassRef;
};

}
}

#endif
