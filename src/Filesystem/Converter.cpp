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
 * @file        Converter.cpp
 */

#include "Converter.h"

#include <Logger.h>
#include <Commons/Exception.h>
#include <PlatformException.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>

#include "Manager.h"
#include "Path.h"
#include "JSFile.h"
#include "Encodings.h"
#include "JSStorage.h"
#include "FilesystemUtils.h"
#include "FilesystemPathUtils.h"


using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace DeviceAPI {
namespace Filesystem{

namespace {
const char* PROPERTY_FILEFILTER_NAME = "name";
const char* PROPERTY_FILEFILTER_START_CREATED = "startCreated";
const char* PROPERTY_FILEFILTER_END_CREATED = "endCreated";
const char* PROPERTY_FILEFILTER_START_MODIFIED = "startModified";
const char* PROPERTY_FILEFILTER_END_MODIFIED = "endModified";
const char* ACCESS_MODE_READ = "r";
const char* ACCESS_MODE_APPEND = "a";
const char* ACCESS_MODE_WRITE = "w";
const char* ACCESS_MODE_READ_WRITE = "rw";
const char* STORAGE_TYPE_STATE_MOUNTED = "MOUNTED";
const char* STORAGE_TYPE_STATE_REMOVED = "REMOVED";
const char* STORAGE_TYPE_STATE_UNMOUNTABLE = "REMOVED";
const char* STORAGE_TYPE_INTERNAL = "INTERNAL";
const char* STORAGE_TYPE_EXTERNAL = "EXTERNAL";


const char* ENCODINGS[] = {
    DeviceAPI::Filesystem::Encodings::UTF8,
    DeviceAPI::Filesystem::Encodings::ISO88591,
    DeviceAPI::Filesystem::Encodings::SJIS,
    NULL
};
} // namespace


Converter::Converter(JSContextRef context) : WrtDeviceApis::CommonsJavaScript::Converter(context)
{
}

JSValueRef Converter::toJSValueRef(
        const NodeList& arg,
        const File::PermissionList &parentPermissions,
        JSContextRef context)
{
    JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);
    if (!jsResult) {
        LOGW("TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                 "Could not create js array object");
    }

    for (std::size_t i = 0; i < arg.size(); ++i)
    {
        FilePtr priv = FilePtr(new File(arg[i], parentPermissions));
        JSObjectRef jsObject = JSFile::makeJSObject(m_context, priv, &Manager::getInstance());
        if (!jsObject) {
            LOGW("TypeMismatchException");
            throw DeviceAPI::Common::TypeMismatchException(
                     "Could not create JS object.");
        }
        if (!JSSetArrayElement(m_context, jsResult, i, jsObject)) {
            LOGW("TypeMismatchException");
            throw DeviceAPI::Common::TypeMismatchException(
                     "Could not insert value into js array");
        }
    }

    return jsResult;
}


JSValueRef Converter::toJSValueRef(unsigned char* data, std::size_t num)
{
    JSObjectRef result = JSCreateArrayObject(m_context, 0, NULL);
    if (!result) {
        LOGW("TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Could not create array object.");
    }

    for (std::size_t i = 0; i < num; ++i) {
        JSValueRef value = JSValueMakeNumber(m_context, data[i]);
        if (!JSSetArrayElement(m_context, result, i, value)) {
            LOGW("TypeMismatchException");
            throw DeviceAPI::Common::TypeMismatchException(
                    "Could not fill array.");
        }
    }

    return result;
}

JSValueRef Converter::toJSValueRef(unsigned long long arg)
{
    return JSValueMakeNumber(m_context, arg);
}



PathPtr Converter::toPath(const JSValueRef& arg)
{
    try {
        std::string path = toString(arg);
        if (!Utils::isPathValid(path) || path.empty()) {
            LOGW("TypeMismatchException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Invalid path component.");
        }
        return Path::create(path);
    } catch (const Commons::ConversionException exception) {
        LOGW("caught ConversionException");
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException("Not a valid path.");
    } catch (const DeviceAPI::Common::TypeMismatchException exception) {
        LOGW("caught TypeMismatchException");
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException("Not a valid path.");
    }
}
std::string Converter::checkPercentSign(std::string& arg)
{
    size_t pos = 0;

    pos = arg.find("%");

    if (pos == std::string::npos)
    {
        return arg;
    }
    else if (arg[arg.size() - 1] == '%')
    {
        arg[arg.size() - 1] = ')';

        if (arg[0] == '%')
        {
            arg[0] = '(';
        }
        else
        {
            arg.insert(0, "^");
            arg.insert(1, "(");
        }

        return arg;
    }
    LOGW("throw TypeMismatchException");
    throw DeviceAPI::Common::TypeMismatchException("Invalid Filter");
}

NodeFilterPtr Converter::toNodeFilter(const JSValueRef& arg)
{
    JSObjectRef filter = toJSObjectRef(arg);

    NodeFilterPtr result(new NodeFilter());
    JSValueRef prop = NULL;
    prop = JSUtils::getJSProperty(m_context, filter, PROPERTY_FILEFILTER_NAME);
    if (prop) {
        std::string nameFilter = toString(prop);
        result->setName(makeOptionalPair(checkPercentSign(nameFilter)));
    }

    prop = JSUtils::getJSProperty(m_context, filter, PROPERTY_FILEFILTER_START_CREATED);
    if (prop) {
        result->setMinCreated(makeOptionalPair(toDateTimeT(prop)));
    }

    prop = JSUtils::getJSProperty(m_context, filter, PROPERTY_FILEFILTER_END_CREATED);
    if (prop) {
        result->setMaxCreated(makeOptionalPair(toDateTimeT(prop)));
    }

    prop = JSUtils::getJSProperty(m_context, filter, PROPERTY_FILEFILTER_START_MODIFIED);
    if (prop) {
        result->setMinModified(makeOptionalPair(toDateTimeT(prop)));
    }

    prop = JSUtils::getJSProperty(m_context, filter, PROPERTY_FILEFILTER_END_MODIFIED);
    if (prop) {
        result->setMaxModified(makeOptionalPair(toDateTimeT(prop)));
    }

    return result;
}

AccessMode Converter::toAccessMode(const JSValueRef& arg)
{
    std::string mode = toString_(arg);
    if (ACCESS_MODE_READ == mode) {
        return AM_READ;
    } else if (ACCESS_MODE_APPEND == mode) {
        return AM_APPEND;
    } else if (ACCESS_MODE_WRITE == mode) {
        return AM_WRITE;
    } else if (ACCESS_MODE_READ_WRITE == mode) {
        return AM_READ_WRITE;
    }

    LOGW("throw TypeMismatchException");
    throw DeviceAPI::Common::TypeMismatchException("Invalid mode.");
}

std::string Converter::toEncoding(const JSValueRef& arg)
{
    std::string result = toString_(arg);
    const char** ptr = ENCODINGS;
    while (*ptr) {
        if (result == *ptr) {
            return result;
        }
        ++ptr;
    }
    LOGW("throw TypeMismatchException");
    throw DeviceAPI::Common::TypeMismatchException("Invalid encoding");
}
JSValueRef Converter::toStorageState(const short type)
{
    switch (type)
    {
        case JSStorage::STATE_MOUNTED:
            return toJSValueRef(STORAGE_TYPE_STATE_MOUNTED);
        case JSStorage::STATE_REMOVED:
            return toJSValueRef(STORAGE_TYPE_STATE_REMOVED);
        case JSStorage::STATE_UNMOUNTABLE:
            return toJSValueRef(STORAGE_TYPE_STATE_UNMOUNTABLE);
    }
    LOGW("throw TypeMismatchException");
    throw DeviceAPI::Common::TypeMismatchException("Invalid storage type");
}

JSValueRef Converter::toStorageType(const short state)
{
    switch (state)
    {
        case StorageProperties::TYPE_INTERNAL:
            return toJSValueRef(STORAGE_TYPE_INTERNAL);
        case StorageProperties::TYPE_EXTERNAL:
            return toJSValueRef(STORAGE_TYPE_EXTERNAL);
    }
    LOGW("throw TypeMismatchException");
    throw DeviceAPI::Common::TypeMismatchException("Invalid storage type");
}

JSValueRef Converter::toJSValueRef(
        const StoragePropertiesPtr &arg,
        JSContextRef context,
        Common::SecurityAccessor* srcSecurityAccessor)
{
    StorageProperties tmpStorage;

    tmpStorage.setLabel(arg->getLabel());
    tmpStorage.setType(arg->getType());

    switch (arg->getState()) {
    case StorageProperties::STATE_MOUNTED :
    case StorageProperties::STATE_MOUNTED_READONLY :
        tmpStorage.setState(JSStorage::STATE_MOUNTED);
        break;
    case StorageProperties::STATE_REMOVED:
        tmpStorage.setState(JSStorage::STATE_REMOVED);
        break;
    case StorageProperties::STATE_UNMOUNTABLE:
        tmpStorage.setState(JSStorage::STATE_UNMOUNTABLE);
        break;
    }

    JSObjectRef jsObject = JSStorage::createJSObject(context, tmpStorage,
        srcSecurityAccessor);
    if (!jsObject) {
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Could not create JS object.");
    }

    return toJSValueRef(jsObject);
}

JSValueRef Converter::toJSValueRef(
        const std::vector<StoragePropertiesPtr>& arg,
        JSContextRef context)
{
    JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);
    if (!jsResult) {
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Could not create js array object");
    }

    StorageProperties tmpStorage;

    for (size_t i = 0; i < arg.size(); i++) {
        tmpStorage.setLabel(arg[i]->getLabel());
        tmpStorage.setType(arg[i]->getType());

        switch (arg[i]->getState()) {
        case StorageProperties::STATE_MOUNTED :
        case StorageProperties::STATE_MOUNTED_READONLY :
            tmpStorage.setState(JSStorage::STATE_MOUNTED);
            break;
        case StorageProperties::STATE_REMOVED:
            tmpStorage.setState(JSStorage::STATE_REMOVED);
            break;
        case StorageProperties::STATE_UNMOUNTABLE:
            tmpStorage.setState(JSStorage::STATE_UNMOUNTABLE);
            break;
        }

        JSObjectRef jsObject = JSObjectMake(m_context, NULL, NULL);
        const ScopedJSStringRef labelStr(JSStringCreateWithUTF8CString("label"));
        const ScopedJSStringRef typeStr(JSStringCreateWithUTF8CString("type"));
        const ScopedJSStringRef stateStr(JSStringCreateWithUTF8CString("state"));

        JSObjectSetProperty(m_context, jsObject, labelStr.get(), toJSValueRef(tmpStorage.getLabel()), kJSPropertyAttributeReadOnly, NULL);
        JSObjectSetProperty(m_context, jsObject, typeStr.get(), toStorageType(tmpStorage.getType()), kJSPropertyAttributeReadOnly, NULL);
        JSObjectSetProperty(m_context, jsObject, stateStr.get(), toStorageState(tmpStorage.getState()), kJSPropertyAttributeReadOnly, NULL);

        if (!jsObject) {
            LOGW("throw TypeMismatchException");
            throw DeviceAPI::Common::TypeMismatchException(
                    "Could not create JS object.");
        }

        if (!JSSetArrayElement(m_context, jsResult, i, jsObject)) {
            LOGW("throw TypeMismatchException");
            throw DeviceAPI::Common::TypeMismatchException(
                    "Could not insert value into js array");
        }
    }

    return jsResult;
}
}
}
