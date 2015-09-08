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

#include <PlatformException.h>
#include "JSNdefRecord.h"
#include "JSNdefRecordMedia.h"
#include "JSNdefRecordText.h"
#include "JSNdefRecordURI.h"
#include "JSNdefRecordVector.h"
#include "NdefRecordMedia.h"
#include "NdefRecordText.h"
#include "NdefRecordURI.h"

namespace DeviceAPI {
namespace NFC {

JSValueRef JSNdefRecordVector::makeJSValue(JSContextRef ctx, NdefRecordPtr ptr)
{
    switch(ptr->getRecordType()) {
        case NFC_RECORD_TEXT:
            return JSNdefRecordText::makeJSObject(ctx,
                    std::static_pointer_cast<NdefRecordText>(ptr));
            break;
        case NFC_RECORD_URI:
            return JSNdefRecordURI::makeJSObject(ctx,
                    std::static_pointer_cast<NdefRecordURI>(ptr));
            break;
        case NFC_RECORD_MEDIA:
            return JSNdefRecordMedia::makeJSObject(ctx,
                    std::static_pointer_cast<NdefRecordMedia>(ptr));
            break;
        default:
            return JSNdefRecord::makeJSObject(ctx, ptr);
    }
}

NdefRecordPtr JSNdefRecordVector::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    LOGD("Entered");
    if (JSValueIsObjectOfClass(context, value, JSNdefRecordURI::getClassRef())) {
        // URI support
        return std::static_pointer_cast<NdefRecord>(
                JSNdefRecordURI::getPrivateObject(context, value));
    }
    if (JSValueIsObjectOfClass(context, value, JSNdefRecordText::getClassRef())) {
        // Text support
        return std::static_pointer_cast<NdefRecord>(
                JSNdefRecordText::getPrivateObject(context, value));
    }
    if (JSValueIsObjectOfClass(context, value, JSNdefRecordMedia::getClassRef())) {
        // Media support
        return std::static_pointer_cast<NdefRecord>(
                JSNdefRecordMedia::getPrivateObject(context, value));
    }
    if (JSValueIsObjectOfClass(context, value, JSNdefRecord::getClassRef())) {
        // Base record
        return JSNdefRecord::getPrivateObject(context, value);
    }

    LOGE("Given object does not match any of known objects.");
    throw Common::TypeMismatchException("Type mismatch");
}


JSNdefRecordVector::JSNdefRecordVector(const std::vector<NdefRecordPtr>& vec):
        Common::JSVector<NdefRecordPtr>(vec, getPrivateObject, makeJSValue)
{
}

JSNdefRecordVector::JSNdefRecordVector():
        Common::JSVector<NdefRecordPtr>(getPrivateObject, makeJSValue)
{
}

JSNdefRecordVector& JSNdefRecordVector::operator=(const std::vector<NdefRecordPtr>& list )
{
    Common::JSVector<NdefRecordPtr>::operator=(list);
    return *this;
}

} // NFC
} // DeviceAPI
