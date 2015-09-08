//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#ifndef _TIZEN_COMMON_JS_CONSOLE_
#define _TIZEN_COMMON_JS_CONSOLE_

#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Test{

class JSConsole{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static JSObjectRef createJSObject(JSContextRef context);

protected:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static bool hasInstance(JSContextRef context, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception);

    static JSValueRef log(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef assert(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

private:
	static JSClassDefinition m_jsClassInfo;
	static JSClassRef m_jsClassRef;
	static JSStaticFunction m_function[];

};

} // Test
} // TizenApis

#endif //_TIZEN_COMMON_JS_CONSOLE_


