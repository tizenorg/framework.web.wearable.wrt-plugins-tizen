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

/**
 * @file        Converter.h
 */

#ifndef TIZENAPIS_TIZEN_FILESYSTEM_CONVERTER_H_
#define TIZENAPIS_TIZEN_FILESYSTEM_CONVERTER_H_

#include <vector>
#include <string>
#include <map>
#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/Converter.h>
#include "Enums.h"
#include "Path.h"
#include "Node.h"
#include "NodeFilter.h"
#include "StorageProperties.h"
#include "JSFile.h"
#include <SecurityAccessor.h>

namespace DeviceAPI {
namespace Filesystem {
class Converter : public WrtDeviceApis::CommonsJavaScript::Converter
{
public:
    using WrtDeviceApis::CommonsJavaScript::Converter::toJSValueRef;

public:
    explicit Converter(JSContextRef context);

    JSValueRef toJSValueRef(
            const NodeList& arg,
            const File::PermissionList &parentPermissions,
            JSContextRef context);
    JSValueRef toJSValueRef(unsigned char* data,
        std::size_t num);
    JSValueRef toJSValueRef(unsigned long long arg);

    PathPtr toPath(const JSValueRef& arg);

    NodeFilterPtr toNodeFilter(const JSValueRef& arg);

    AccessMode toAccessMode(const JSValueRef& arg);

    std::string toEncoding(const JSValueRef& arg);

    JSValueRef toStorageType(const short type);
    JSValueRef toStorageState(const short state);
    std::string checkPercentSign(std::string& arg);

    JSValueRef toJSValueRef(
            const StoragePropertiesPtr &arg,
            JSContextRef context,
            Common::SecurityAccessor* srcSecurityAccessor);

    JSValueRef toJSValueRef(
        const std::vector<StoragePropertiesPtr>& arg,
        JSContextRef context);
};

typedef WrtDeviceApis::CommonsJavaScript::ConverterFactory<Converter> ConverterFactory;
typedef ConverterFactory::ConverterType ConverterPtr;
}
}

#endif
