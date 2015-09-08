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

#ifndef __TIZEN_NFC_NDEFRECORD_VECTOR_H_
#define __TIZEN_NFC_NDEFRECORD_VECTOR_H_

#include <memory>
#include <vector>
#include <JSVector.h>
#include "NdefRecord.h"


namespace DeviceAPI {
namespace NFC {

class JSNdefRecordVector : public Common::JSVector<NdefRecordPtr> {
        static JSValueRef makeJSValue(JSContextRef ctx, NdefRecordPtr ptr);
        static NdefRecordPtr getPrivateObject(JSContextRef context, JSValueRef value);

    public:
        JSNdefRecordVector(const std::vector<NdefRecordPtr>& vec);
        JSNdefRecordVector();
        JSNdefRecordVector& operator=( const std::vector<NdefRecordPtr>& list );
};

} // NFC
} // DeviceAPI


#endif // __TIZEN_NFC_NDEFRECORD_VECTOR_H_
