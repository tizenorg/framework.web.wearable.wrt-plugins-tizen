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



#ifndef __TIZEN_NFC_JS_NDEFRECORD_URI_H_
#define __TIZEN_NFC_JS_NDEFRECORD_URI_H_

#include "JSNdefRecord.h"
#include "NdefRecordURI.h"

namespace DeviceAPI {
namespace NFC {

class JSNdefRecordURI
{
public:
    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static JSObjectRef constructor(JSContextRef ctx,
                JSObjectRef constructor,
                size_t argumentCount,
                const JSValueRef arguments[],
                JSValueRef* exception);

    static NdefRecordURIPtr getPrivateObject(JSContextRef context, JSValueRef value);

    static void setPrivateObject(JSObjectRef object, NdefRecordURIPtr data);

    static JSObjectRef makeJSObject(JSContextRef context, NdefRecordURIPtr attptr);

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
     * The callback invoked when getting a property's value.
     */
    static JSValueRef getUri(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared value property.
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

};

} //NFC
} //DeviceAPI

#endif /* __TIZEN_NFC_JS_NDEFRECORD_URI_H_ */
