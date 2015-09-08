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

#include <memory>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Converter.h>
#include <Commons/Exception.h>
#include "ApplicationInformation.h"
#include <JSWebAPIErrorFactory.h>
#include "JSApplicationInformation.h"
#include "AppManagerWrapper.h"
#include "plugin_config_impl.h"
#include <Export.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis;
using namespace DeviceAPI::Common;


JSClassRef JSApplicationInformation::m_classRef = NULL;

JSClassDefinition JSApplicationInformation::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION_INFORMATION,
    0,
    m_property,
    0,
    initialize,
    finalize,
    NULL,     //HasProperty,
    NULL,   //GetProperty,
    NULL,     //SetProperty,
    NULL,     //DeleteProperty,
    NULL,     //GetPropertyNames,
    NULL,     //CallAsFunction,
    NULL,     //CallAsConstructor,
    NULL,
    NULL,     //ConvertToType
};

JSStaticValue JSApplicationInformation::m_property[] = {
    { TIZEN_APPLICATION_INFORMATION_ID, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_NAME, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_ICONPATH, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_VERSION, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_SHOW, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_CATEGORIES, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_INSTALL_DATE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_INSTALL_SIZE, getPropertySize, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_INFORMATION_PACKAGE_ID, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef DLL_EXPORT JSApplicationInformation::getClassRef() {
    LOGD("Entered");
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

JSValueRef JSApplicationInformation::createJSObject(JSContextRef context,
        const std::string &name,
        const std::string &appId,
        const std::string &iconPath,
        const std::string &version,
        const bool &show,
        const std::vector<std::string> &categories,
        const time_t &installDate,
        const long &installSize,
        const std::string &pkgId)
{
    LOGD("Entered");
    ApplicationInformationPtr privateData = ApplicationInformationPtr(
            new ApplicationInformation());
    privateData->setName(name);
    privateData->setAppId(appId);
    privateData->setIconPath(iconPath);
    privateData->setVersion(version);
    privateData->setShow(show);
    privateData->setCategories(categories);
    privateData->setInstallDate(installDate);
    privateData->setInstallSize(installSize);
    privateData->setPackageId(pkgId);

    JSApplicationInformationPriv *priv =
            new JSApplicationInformationPriv(context, privateData);

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}

void JSApplicationInformation::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplicationInformation::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationInformationPriv* priv =
            static_cast<JSApplicationInformationPriv*>(JSObjectGetPrivate(object));
    if (priv == NULL)
        return;

    ApplicationInformationPtr privateData = priv->getObject();

    JSValueRef installDateJSValue =
            static_cast<JSValueRef>(privateData->getInstallDateJSValue());
    if (installDateJSValue != NULL)
        JSValueUnprotect(priv->getContext(), installDateJSValue);

    JSValueRef categoriesJSValue =
            static_cast<JSValueRef>(privateData->getCategoriesJSValue());
    if (categoriesJSValue != NULL)
        JSValueUnprotect(priv->getContext(), categoriesJSValue);

    JSObjectSetPrivate(object, NULL);
    delete priv;
}

bool JSApplicationInformation::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ApplicationInformationPtr JSApplicationInformation::getPrivData(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationInformationPriv *priv =
            static_cast<JSApplicationInformationPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    ApplicationInformationPtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}

ApplicationInformationPtr JSApplicationInformation::getApplicationInformation(JSContextRef context,
        JSValueRef value)
{
    LOGD("Entered");
    if (!isObjectOfClass(context, value)) {
        throw TypeMismatchException("is not a object class");
    }

    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        throw TypeMismatchException("Fail to get object");
    }

    JSApplicationInformationPriv *priv =
            static_cast<JSApplicationInformationPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }

    return priv->getObject();
}

JSValueRef JSApplicationInformation::getProperty(JSContextRef context,
    JSObjectRef object,
    JSStringRef propertyName,
    JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CommonsJavaScript::Converter converter(context);
        ApplicationInformationPtr privateData = getPrivData(context, object);
        JSContextRef gContext =
                static_cast<JSApplicationInformationPriv*>(JSObjectGetPrivate(object))->getContext();

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_ID)) {
            return converter.toJSValueRef(privateData->getAppId());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_NAME)) {
            return converter.toJSValueRef(privateData->getName());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_ICONPATH)) {
            return converter.toJSValueRef(privateData->getIconPath());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_SHOW)) {
            return converter.toJSValueRef(privateData->getShow());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_CATEGORIES)) {
            JSValueRef categoriesJSValue = static_cast<JSValueRef>(privateData->getCategoriesJSValue());
            if (categoriesJSValue == NULL) {
                categoriesJSValue = converter.toJSValueRef(privateData->getCategories());
                JSValueProtect(gContext, categoriesJSValue);
                privateData->setCategoriesJSValue(static_cast<const void *>(categoriesJSValue));
            }
            return categoriesJSValue;
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_PACKAGE_ID)) {
            return converter.toJSValueRef(privateData->getPackageId());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_VERSION)) {
            return converter.toJSValueRef(privateData->getVersion());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_INSTALL_DATE)) {
            JSValueRef installDateJSValue = static_cast<JSValueRef>(privateData->getInstallDateJSValue());
            if (installDateJSValue == NULL) {
                installDateJSValue = converter.toJSValueRef(privateData->getInstallDate());
                JSValueProtect(gContext, installDateJSValue);
                privateData->setInstallDateJSValue(static_cast<const void *>(installDateJSValue));
            }
            return installDateJSValue;
        }
    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}

JSValueRef JSApplicationInformation::getPropertySize(JSContextRef context,
    JSObjectRef object,
    JSStringRef propertyName,
    JSValueRef* exception)
{
    LOGD("Entered");
    JSApplicationInformationPriv *priv =
            static_cast<JSApplicationInformationPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is not exists.");
        return JSValueMakeUndefined(context);
    }

    TIZEN_CHECK_ACCESS(context, exception, priv, APPLICATION_FUNCTION_API_SIZE);

    ApplicationInformationPtr privateData = priv->getObject();
    if (!privateData) {
        LOGE("Private object is wrong");
        return JSValueMakeUndefined(context);
    }

    try {
        CommonsJavaScript::Converter converter(context);

        if (!privateData->isInitialized()) {
            AppManagerWrapperSingleton::Instance().initializeAppInfo(privateData);
        }

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_INFORMATION_INSTALL_SIZE)) {
            return converter.toJSValueRefLong(privateData->getInstallSize());
        }

    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}

}
}
