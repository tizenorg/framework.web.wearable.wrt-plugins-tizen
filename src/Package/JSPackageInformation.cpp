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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSUtils.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include "JSPackageInformation.h"
#include "PackageInformation.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Package {

using namespace std;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassRef JSPackageInformation::m_classRef = NULL;

JSClassDefinition JSPackageInformation::m_classInfo = {
    0,                                        // current (and only) version is 0
    kJSClassAttributeNone,                    // attributes
    TIZEN_INTERFACE_PACKAGE_INFORMATION,    // class name
    NULL,                                    // parent class
    m_property,                                // static values
    NULL,                                    // static functions
    initialize,                                // initialize
    finalize,                                // finalize
    NULL,                                    // hasProperty
    NULL,                                    // getProperty
    NULL,                                    // setProperty
    NULL,                                    // deleteProperty
    NULL,                                    // getPropertyNames
    NULL,                                    // callAsConstructor
    NULL,                                    // callAsConstructor
    NULL,                                    // hasInstance
    NULL                                    // convertToType
};

JSStaticValue JSPackageInformation::m_property[] = {
    { TIZEN_PACKAGE_INFORMATION_ID, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_NAME, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_ICONPATH, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_VERSION, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_TOTAL_SIZE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_DATA_SIZE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_LAST_MODIFIED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_AUTHOR, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_DESCRIPTION, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PACKAGE_INFORMATION_APP_IDS, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSPackageInformation::getClassRef() {
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}


JSValueRef JSPackageInformation::createJSObject(JSContextRef context, PackageInformation *pkgInfo)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(pkgInfo));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSPackageInformation::initialize(JSContextRef context, JSObjectRef object)
{
}

void JSPackageInformation::finalize(JSObjectRef object)
{
}

bool JSPackageInformation::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

PackageInformation* JSPackageInformation::getPrivData(JSObjectRef object)
{
    PackageInformation *priv = static_cast<PackageInformation*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is null");
        throw TypeMismatchException("Private object is null");
    }

    return priv;
}

JSValueRef JSPackageInformation::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        PackageInformation* privateData = getPrivData(object);
        if (!privateData) {
            LOGE("Private object is NULL");
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_ID)) {
            return JSUtil::toJSValueRef(context, privateData->m_id);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_NAME)) {
            return JSUtil::toJSValueRef(context, privateData->m_name);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_ICONPATH)) {
            return JSUtil::toJSValueRef(context, privateData->m_iconPath);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_VERSION)) {
            return JSUtil::toJSValueRef(context, privateData->m_version);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_LAST_MODIFIED)) {
            return JSUtil::makeDateObject(context, privateData->m_lastModified);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_AUTHOR)) {
            return JSUtil::toJSValueRef(context, privateData->m_author);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_DESCRIPTION)) {
            return JSUtil::toJSValueRef(context, privateData->m_description);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_APP_IDS)) {
            return JSUtil::toJSValueRef(context, privateData->m_appIds);
        } else {
            // Operation to get size need long time (IPC Delay).
            if (!privateData->m_isInitialized) {
                privateData->initializePackageInfo();
            }

            if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_TOTAL_SIZE)) {
                return JSUtil::toJSValueRef(context, privateData->m_totalSize);
            } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PACKAGE_INFORMATION_DATA_SIZE)) {
                return JSUtil::toJSValueRef(context, privateData->m_dataSize);
            }
        }
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

}
}
