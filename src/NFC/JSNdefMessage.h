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



#ifndef __TIZEN_NFC_JS_NDEFMESSAGE_H_
#define __TIZEN_NFC_JS_NDEFMESSAGE_H_

#include "NdefMessage.h"

#include <Logger.h>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace NFC {

class JSNdefMessage
{
public:

    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static NdefMessagePtr getPrivateObject(JSContextRef context, JSValueRef value);

    static void setPrivateObject(JSObjectRef object, NdefMessagePtr data);

    static JSObjectRef makeJSObject(JSContextRef context, NdefMessagePtr native);

    static JSObjectRef constructor(JSContextRef ctx,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

private:
    /**
     * This member variable contains the values which has to be passed when
     * the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to the
     * data structure JSPropertySpec.
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);


    /**
     * Getters and setters for properties
     */
    static JSValueRef getRecordCount(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getRecords(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setRecords(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    static JSValueRef toByte(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    template<class T, class U>
    static JSObjectRef toJSObjectArray(JSContextRef context,
            const std::vector<T> & vec)
    {
        size_t vec_size = vec.size();
        JSObjectRef array[vec_size];
        for (size_t i = 0; i < vec_size; ++i) {
            array[i] = U::makeJSObject(context, vec[i]);
        }
        JSObjectRef result = JSObjectMakeArray(context,
                vec_size, vec_size ? array : NULL, NULL);
        if (!result) {
            LOGW("Array is null");
            throw Common::UnknownException("Array is null");
        }
        return result;
    }
};

}
}

#endif /* __TIZEN_NFC_JS_NDEFMESSAGE_H_ */
