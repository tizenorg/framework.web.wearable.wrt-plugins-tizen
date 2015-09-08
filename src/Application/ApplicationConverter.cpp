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

#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <Commons/RegexUtils.h>
#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>
#include <Logger.h>

#include "ApplicationConverter.h"
#include "JSApplication.h"
#include "JSApplicationInformation.h"
#include "JSApplicationContext.h"
#include "JSApplicationControlData.h"
#include "JSApplicationControlData.h"
#include "JSApplicationControl.h"
#include "JSRequestedApplicationControl.h"
#include "JSApplicationMetaData.h"
#include "JSApplicationCert.h"

namespace DeviceAPI {
namespace Application {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis;
using namespace WrtDeviceApis::CommonsJavaScript;

ApplicationConverter::ApplicationConverter(
        JSContextRef context,
        const SecurityAccessor *accessor):
    Converter(context),
    m_securityAccessor(accessor)
{
    LOGD("Entered");
}

ApplicationConverter::~ApplicationConverter()
{
    LOGD("Entered");
}


JSValueRef ApplicationConverter::toJSValueRefFromApplication(
        const ApplicationPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::ConversionException);
    }
    return CommonsJavaScript::JSUtils::makeObject(m_context,
            JSApplication::getClassRef(), arg);
}


JSValueRef ApplicationConverter::toJSValueRefFromApplicationCert(
        const ApplicationCertPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::ConversionException);
    }
    return CommonsJavaScript::JSUtils::makeObject(m_context,
            JSApplicationCert::getClassRef(), arg);
}


JSValueRef ApplicationConverter::toJSValueRefFromApplicationCerts(
        const ApplicationCertArrayPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }
    return toJSValueRef_(*arg,
            &ApplicationConverter::toJSValueRefFromApplicationCert, this);
}


JSValueRef ApplicationConverter::toJSValueRefFromApplicationMetaData(
        const ApplicationMetaDataPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::ConversionException);
    }
    return CommonsJavaScript::JSUtils::makeObject(m_context,
            JSApplicationMetaData::getClassRef(), arg);
}


JSValueRef ApplicationConverter::toJSValueRefFromApplicationMetaDataArray(
        const ApplicationMetaDataArrayPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }
    return toJSValueRef_(*arg,
            &ApplicationConverter::toJSValueRefFromApplicationMetaData, this);
}


JSValueRef ApplicationConverter::toJSValueRefFromApplicationInformation(
        const ApplicationInformationPtr &arg)
{
    LOGD("Entered");
    if (m_securityAccessor == NULL)
        Throw(Commons::ConversionException);

    if (arg == NULL) {
        Throw(Commons::ConversionException);
    }
    JSApplicationInformationPriv *priv = new JSApplicationInformationPriv(m_context, arg);
    priv->copyAceCheckAccessFunction(m_securityAccessor);
    return JSObjectMake(m_context, JSApplicationInformation::getClassRef(), priv);
}

ApplicationInformationPtr ApplicationConverter::toApplicationInformation(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSApplicationInformation::isObjectOfClass(m_context, jsValue))
        return JSApplicationInformation::getApplicationInformation(m_context, jsValue);

    const ScopedJSStringRef nameStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_NAME));
    const ScopedJSStringRef appIdStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_ID));
    const ScopedJSStringRef iconPathStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_ICONPATH));
    const ScopedJSStringRef versionStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_VERSION));
    const ScopedJSStringRef showStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_SHOW));
    const ScopedJSStringRef categoriesStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_CATEGORIES));
    const ScopedJSStringRef installDateStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_INSTALL_DATE));
    const ScopedJSStringRef installSizeStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_INSTALL_SIZE));
    const ScopedJSStringRef packageIdStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_INFORMATION_PACKAGE_ID));

    JSObjectRef jsObject = toJSObjectRef(jsValue);

    JSValueRef nameData =
            JSObjectGetProperty(m_context, jsObject, nameStr.get(), NULL);
    JSValueRef appIdStrData =
            JSObjectGetProperty(m_context, jsObject, appIdStr.get(), NULL);
    JSValueRef iconPathData =
            JSObjectGetProperty(m_context, jsObject, iconPathStr.get(), NULL);
    JSValueRef versionData =
            JSObjectGetProperty(m_context, jsObject, versionStr.get(), NULL);
    JSValueRef showData =
            JSObjectGetProperty(m_context, jsObject, showStr.get(), NULL);
    JSValueRef categoriesData =
            JSObjectGetProperty(m_context, jsObject, categoriesStr.get(), NULL);
    JSValueRef installDateData =
            JSObjectGetProperty(m_context, jsObject, installDateStr.get(), NULL);
    JSValueRef installSizeData =
            JSObjectGetProperty(m_context, jsObject, installSizeStr.get(), NULL);
    JSValueRef pkgIdStrData =
            JSObjectGetProperty(m_context, jsObject, packageIdStr.get(), NULL);

    std::string name;
    std::string appid;
    std::string iconPath;
    std::string version;
    bool show;
    std::vector <std::string> categories;
    time_t installDate;
    long installSize;
    std::string pkgid;

    ApplicationInformationPtr result(new ApplicationInformation());
    if (!result) {
        Throw(Commons::ConversionException);
    }

    if (!JSValueIsUndefined(m_context, nameData)) {
        name = toString(nameData);
        result->setName(name);
    }
    if (!JSValueIsUndefined(m_context, appIdStrData)) {
        appid = toString(appIdStrData);
        result->setAppId(appid);
    }
    if (!JSValueIsUndefined(m_context, iconPathData)) {
        iconPath = toString(iconPathData);
        result->setIconPath(iconPath);
    }
    if (!JSValueIsUndefined(m_context, versionData)) {
        version = toString(versionData);
        result->setVersion(version);
    }
    if (!JSValueIsUndefined(m_context, showData)) {
        show = toBool(showData);
        result->setShow(show);
    }
    if (!JSValueIsUndefined(m_context, categoriesData)) {
        categories = toVectorOfStrings(categoriesData);
        result->setCategories(categories);
    }
    if (!JSValueIsUndefined(m_context, installDateData)) {
        installDate = toDateTimeT(installDateData);
        result->setInstallDate(installDate);
    }
    if (!JSValueIsUndefined(m_context, installSizeData)) {
        installSize = toDateTimeT(installSizeData);
        result->setInstallSize(installSize);
    }
    if (!JSValueIsUndefined(m_context, pkgIdStrData)) {
        pkgid = toString(pkgIdStrData);
        result->setPackageId(pkgid);
    }
    return result;
}

JSValueRef ApplicationConverter::toJSValueRef(
        const ApplicationInformationArrayPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }
    return toJSValueRef_(*arg,
            &ApplicationConverter::toJSValueRefFromApplicationInformation, this);
}

ApplicationInformationArrayPtr ApplicationConverter::toApplicationInformationArray(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    ApplicationInformationArrayPtr result(new ApplicationInformationArray());

    JSObjectRef jsObject = toJSObjectRef(jsValue);
    for (std::size_t i = 0; i < JSGetArrayLength(m_context, jsObject); ++i) {
        JSValueRef element = JSGetArrayElement(m_context, jsObject, i);
        result->push_back(toApplicationInformation(element));
    }
    return result;
}

JSValueRef ApplicationConverter::toJSValueRefFromApplicationContext(
        const ApplicationContextPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }
    return CommonsJavaScript::JSUtils::makeObject(m_context,
            JSApplicationContext::getClassRef(), arg);
}

ApplicationContextPtr ApplicationConverter::toApplicationContext(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSApplicationContext::isObjectOfClass(m_context, jsValue))
        return JSApplicationContext::getApplicationContext(m_context, jsValue);

    const ScopedJSStringRef appIdStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTEXT_APP_ID));
    const ScopedJSStringRef contextIdStr(
            JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTEXT_ID));

    JSObjectRef jsObject = toJSObjectRef(jsValue);

    JSValueRef appIdStrData =
            JSObjectGetProperty(m_context, jsObject, appIdStr.get(), NULL);
    JSValueRef contextIdStrData =
            JSObjectGetProperty(m_context, jsObject, contextIdStr.get(), NULL);

    std::string appid;
    std::string contextid;

    ApplicationContextPtr result(new ApplicationContext());
    if (!result) {
        Throw(Commons::ConversionException);
    }

    if (!JSValueIsUndefined(m_context, appIdStrData)) {
        appid = toString(appIdStrData);
        result->setAppId(appid);
    }
    if (!JSValueIsUndefined(m_context, contextIdStrData)) {
        contextid = toString(contextIdStrData);
        result->setContextId(contextid);
    }

    return result;
}

JSValueRef ApplicationConverter::toJSValueRef(
        const ApplicationContextArrayPtr &arg)
{
    LOGD("Entered");
    if (arg.Get() == NULL) {
        Throw(Commons::ConversionException);
    }
    return toJSValueRef_(*arg,
            &ApplicationConverter::toJSValueRefFromApplicationContext, this);
}

ApplicationContextArrayPtr ApplicationConverter::toApplicationContextArray(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    ApplicationContextArrayPtr result(new ApplicationContextArray());

    JSObjectRef jsObject = toJSObjectRef(jsValue);
    for (std::size_t i = 0; i < JSGetArrayLength(m_context, jsObject); ++i) {
        JSValueRef element = JSGetArrayElement(m_context, jsObject, i);
        result->push_back(toApplicationContext(element));
    }
    return result;
}


JSValueRef ApplicationConverter::toJSValueRef(
        const RequestedApplicationControlPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }
    return CommonsJavaScript::JSUtils::makeObject(m_context,
            JSRequestedApplicationControl::getClassRef(), arg);
}

RequestedApplicationControlPtr ApplicationConverter::toRequestedApplicationControl(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSRequestedApplicationControl::isObjectOfClass(m_context, jsValue)) {
        return JSRequestedApplicationControl::getRequestedApplicationControl(m_context,
                jsValue);
    } else {
        ThrowMsg(Commons::ConversionException, "Wrong parameter type.");
    }
}


JSValueRef ApplicationConverter::toJSValueRef(
        const ApplicationControlPtr &arg)
{
    LOGD("Entered");
    if (arg == NULL) {
        Throw(Commons::InvalidArgumentException);
    }

    JSValueRef jsValueData = toJSValueRef(arg->getAppControlDataArray());

    return JSApplicationControl::createJSObject(m_context, arg, jsValueData);
}

ApplicationControlPtr ApplicationConverter::toApplicationControl(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSApplicationControl::isObjectOfClass(m_context, jsValue)) {
        ApplicationControlPtr result = JSApplicationControl::getApplicationControl(m_context,
                jsValue);

        JSObjectRef jsObject = toJSObjectRef(jsValue);

        const ScopedJSStringRef appControlDataStr(JSStringCreateWithUTF8CString(
                TIZEN_APPLICATION_CONTROL_DATA_INTERNAL));
        JSValueRef appControlDataData = JSObjectGetProperty(
                m_context, jsObject, appControlDataStr.get(), NULL);

        std::vector<ApplicationControlDataPtr> appControlDataArray;
        try {
            appControlDataArray = toApplicationControlDataArray(appControlDataData);
        } catch(const Commons::ConversionException &err) {
            LOGD("ApplicationControlDataArray is wrong.");
        }

        result->setAppControlDataArray(appControlDataArray);

        return result;
    } else {
        throw TypeMismatchException("not a object of class ApplicationControl");
    }
}

JSValueRef ApplicationConverter::toJSValueRef(
        const ApplicationControlDataPtr &arg)
{
    LOGD("Entered");
    JSValueRef jsValueValue = toJSValueRef(arg->getValue());

    return JSApplicationControlData::createJSObject(m_context, arg, jsValueValue);
}

ApplicationControlDataPtr ApplicationConverter::toApplicationControlData(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSApplicationControlData::isObjectOfClass(m_context, jsValue))
    {
        ApplicationControlDataPtr result =
                JSApplicationControlData::getApplicationControlData(m_context, jsValue);

        JSObjectRef jsObject = toJSObjectRef(jsValue);

        const ScopedJSStringRef appControlDataValueStr(
                JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTROL_DATA_VALUE_INTERNAL));
        JSValueRef appControlDataValueData =
                JSObjectGetProperty(m_context, jsObject, appControlDataValueStr.get(), NULL);

        if (!JSValueIsUndefined(m_context, appControlDataValueData)) {
            result->setValue(toVectorOfStrings(appControlDataValueData));
        }
        return result;
    }
    else
    {
        const ScopedJSStringRef appControlDataKeyStr(
                JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTROL_DATA_KEY));
        const ScopedJSStringRef appControlDataValueStr(
                JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTROL_DATA_VALUE_INTERNAL));

        JSObjectRef jsObject = toJSObjectRef(jsValue);

        JSValueRef appControlDataKeyData = JSObjectGetProperty(m_context,
                jsObject, appControlDataKeyStr.get(), NULL);
        JSValueRef appControlDataValueData = JSObjectGetProperty(m_context,
                jsObject, appControlDataValueStr.get(), NULL);

        ApplicationControlDataPtr result(new ApplicationControlData());
        if (!result) {
            Throw(Commons::ConversionException);
        }

        if (!JSValueIsUndefined(m_context, appControlDataKeyData)) {
            result->setKey(toString(appControlDataKeyData));
        }

        if (!JSValueIsUndefined(m_context, appControlDataValueData)) {
            result->setValue(toVectorOfStrings(appControlDataValueData));
        }
        return result;
    }
}

JSValueRef ApplicationConverter::toJSValueRef(
        const std::vector<ApplicationControlDataPtr> &arg)
{
    LOGD("Entered");
    JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

    if (jsResult == NULL) {
        ThrowMsg(Commons::ConversionException, "Could not create js array object");
    }

    for (std::size_t i = 0; i < arg.size(); ++i) {
        JSValueRef tmpVal = toJSValueRef(arg[i]);
        if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
            ThrowMsg(Commons::ConversionException, "Could not insert value into js array");
        }
    }

    return jsResult;
}

std::vector<ApplicationControlDataPtr> ApplicationConverter::toApplicationControlDataArray(
        const JSValueRef &jsValue)
{
    LOGD("Entered");
    if (JSValueIsNull(m_context, jsValue) || JSValueIsUndefined(m_context, jsValue)) {
        return std::vector<ApplicationControlDataPtr>();
    }

    if (!JSIsArrayValue(m_context, jsValue)) {
        ThrowMsg(Commons::ConversionException, "Argument is not an JS array.");
    }

    std::vector<ApplicationControlDataPtr> result;
    JSObjectRef objArg = toJSObjectRef(jsValue);
    for (std::size_t i = 0; i < JSGetArrayLength(m_context, objArg); ++i) {
        JSValueRef element = JSGetArrayElement(m_context, objArg, i);
        result.push_back(toApplicationControlData(element));
    }
    return result;
}

}
}
