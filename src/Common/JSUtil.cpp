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

#include "JSUtil.h"
#include "PlatformException.h"
#include <WKBundle.h>
#include <cmath>
#include <limits>


using namespace std;

namespace DeviceAPI {
namespace Common{

JSValueRef JSUtil::getProperty(JSContextRef ctx , JSObjectRef object, const char *name, JSValueRef *exception){
    JSValueRef value;
    JSStringRef propertyName = JSStringCreateWithUTF8CString(name);
    JSValueRef localException = NULL;
    value = JSObjectGetProperty(ctx, object, propertyName, &localException);
    JSStringRelease(propertyName);

    if( localException != NULL ){
        if( exception != NULL )
            *exception = localException;
        else
            throw TypeMismatchException(ctx,localException);
    }
    return value;
}

void JSUtil::setProperty(JSContextRef ctx , JSObjectRef object, const char *name, JSValueRef value, JSPropertyAttributes attributes, JSValueRef *exception){
    JSStringRef propertyName = JSStringCreateWithUTF8CString(name);
    JSValueRef localException = NULL;
    JSObjectSetProperty(ctx, object, propertyName, value,attributes, &localException);
    JSStringRelease(propertyName);
    if( localException != NULL ){
        if( exception != NULL )
            *exception = localException;
        else
            throw TypeMismatchException(ctx,localException);
    }
}

string JSUtil::JSStringToString(JSContextRef ctx, JSStringRef str){
    std::string result;
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(str);
    {
        char buffer[jsSize];
        JSStringGetUTF8CString(str, buffer, jsSize);
        result = buffer;
    }
    return result;
}

string JSUtil::JSValueToString(JSContextRef ctx, JSValueRef value){
    std::string result;
    JSValueRef exception = NULL;
    JSStringRef str = JSValueToStringCopy(ctx, value, &exception);
    if (exception != NULL) {
        throw TypeMismatchException(ctx, exception);
    }
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(str);
    {
        char buffer[jsSize];
        JSStringGetUTF8CString(str, buffer, jsSize);
        result = buffer;
    }
    JSStringRelease(str);
    return result;
}

long JSUtil::JSValueToLong(JSContextRef ctx, JSValueRef value){
    return static_cast<long>(JSValueToLongLong(ctx,value));
}

unsigned long JSUtil::JSValueToULong(JSContextRef ctx, JSValueRef value){
     return static_cast<unsigned long>(JSValueToLongLong(ctx,value));
}

long long JSUtil::JSValueToLongLong(JSContextRef ctx, JSValueRef value){
    return static_cast<long long>(JSValueToNumber(ctx, value));
}

unsigned long long JSUtil::JSValueToULongLong(JSContextRef ctx, JSValueRef value){
    return static_cast<unsigned long long>(JSValueToLongLong(ctx,value));
}

double JSUtil::JSValueToDouble(JSContextRef ctx, JSValueRef value){
    JSValueRef exception = NULL;

    double doublevalue = ::JSValueToNumber(ctx, value, &exception);
    if(exception != NULL){
        throw TypeMismatchException(ctx, exception);
    }
    if( doublevalue == std::numeric_limits<double>::infinity() )
        throw TypeMismatchException("Value is POSITIVE_INFINITY");
    if( doublevalue == -std::numeric_limits<double>::infinity() )
        throw TypeMismatchException("Value is NEGATIVE_INFINITY");
    if( std::isnan(doublevalue)){
        throw TypeMismatchException("Value is not number");
    }
    return doublevalue;
}

double JSUtil::JSValueToNumber(JSContextRef ctx, JSValueRef value){
    JSValueRef exception = NULL;

    double doublevalue = ::JSValueToNumber(ctx, value, &exception);
    if(exception != NULL){
        throw TypeMismatchException(ctx, exception);
    }
    if( doublevalue == std::numeric_limits<double>::infinity() )
        doublevalue = 0.0;

    if( doublevalue == -std::numeric_limits<double>::infinity() )
        doublevalue = 0.0;

    return doublevalue;
}

signed char JSUtil::JSValueToByte(JSContextRef ctx, JSValueRef value){
    return static_cast<signed char>(JSValueToNumber(ctx,value));
}

unsigned char JSUtil::JSValueToOctet(JSContextRef ctx, JSValueRef value){
    return static_cast<unsigned char>(JSValueToNumber(ctx,value));
}

bool JSUtil::JSValueToBoolean(JSContextRef ctx, JSValueRef value){
    return ::JSValueToBoolean(ctx, value);
}

time_t JSUtil::JSValueToTimeT(JSContextRef ctx, JSValueRef value){
    if(!JSValueIsDateObject(ctx, value))
        throw TypeMismatchException("Value is not Date Object");

    JSObjectRef timeobj = NULL;
    timeobj = JSUtil::JSValueToObject(ctx, value);
    JSValueRef exception = NULL;
    JSObjectRef getTime = NULL;
    try{
        getTime = JSUtil::JSValueToObject(ctx, getProperty(ctx, timeobj, "getTime"));
    }catch( const TypeMismatchException& err){
        throw TypeMismatchException("Value is not Date Object");
    }

    JSValueRef timevalue = JSObjectCallAsFunction(ctx, getTime, timeobj, 0, NULL, &exception);
    if( exception != NULL )
        throw TypeMismatchException("Value is not Date Object");

    double millisecond = JSValueToDouble(ctx, timevalue);
    time_t second = millisecond/1000;
    return second;
}

tm JSUtil::JSValueToDateTm(JSContextRef ctx, JSValueRef value){
    tm result = {0};

    time_t second = JSUtil::JSValueToTimeT(ctx, value);

	if(localtime_r(&second, &result) == NULL)
		throw TypeMismatchException("Value is not Date Object");

    return result;
}

tm JSUtil::JSValueToDateTmUTC(JSContextRef ctx, JSValueRef value){
    tm result = {0};

    time_t second = JSUtil::JSValueToTimeT(ctx, value);

	if(gmtime_r(&second, &result) == NULL)
		throw TypeMismatchException("Value is not Date Object");

    return result;
}

JSObjectRef JSUtil::JSValueToObject(JSContextRef ctx, JSValueRef value){
    JSValueRef exception = NULL;
    JSObjectRef obj = ::JSValueToObject(ctx, value,&exception);
    if( exception != NULL){
        throw TypeMismatchException(ctx, exception);
    }
    return obj;
}

std::map<std::string, std::string> JSUtil::JSValueToStringMap(JSContextRef ctx, JSValueRef value){
    std::map<std::string, std::string> result;
    JSObjectRef obj = JSUtil::JSValueToObject(ctx, value);
    JSPropertyNameArrayRef jsPropNames = JSObjectCopyPropertyNames(ctx, obj);
    for (std::size_t i = 0; i < JSPropertyNameArrayGetCount(jsPropNames); ++i) {
        std::string propName = JSUtil::JSStringToString(ctx, JSPropertyNameArrayGetNameAtIndex(jsPropNames, i));
        std::string propValue = JSUtil::JSValueToString(ctx, JSUtil::getProperty(ctx, obj, propName.c_str(), NULL));
        result.insert(std::make_pair(propName, propValue));
    }
    JSPropertyNameArrayRelease(jsPropNames);
    return result;
}

JSObjectRef JSUtil::makeDateObject(JSContextRef ctx, const time_t value){
    JSValueRef exception = NULL;
    JSValueRef args[1];
    double millisecond = value*1000.0;
    args[0] = toJSValueRef(ctx, millisecond);
    JSObjectRef result = JSObjectMakeDate(ctx, 1, args, &exception);
    if( exception != NULL){
        throw TypeMismatchException("Can't create Date object");
    }
    return result;
}


JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const string& str){
    JSValueRef result = NULL;
    JSStringRef jsString = JSStringCreateWithUTF8CString(str.c_str());
    result = JSValueMakeString(ctx, jsString);
    JSStringRelease(jsString);
    return result;
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const unsigned long value){
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const long value){
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const unsigned long long value) {
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const long long value) {
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const double value){
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const signed char value){
    return JSValueMakeNumber(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const unsigned char value){
    return JSValueMakeNumber(ctx, value);
}


JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const bool value){
    return JSValueMakeBoolean(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const std::vector<std::string>& value){
    return toJSValueRef_(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const std::vector<long>& value){
    return toJSValueRef_(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const std::vector<double>& value){
    return toJSValueRef_(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const std::vector<bool>& value){
    return toJSValueRef_(ctx, value);
}

JSValueRef JSUtil::toJSValueRef(JSContextRef ctx, const std::map<std::string, std::string>& value){
    JSObjectRef obj = JSObjectMake(ctx, NULL, NULL);

    std::map<std::string, std::string>::const_iterator iter;
    for (iter = value.begin(); iter != value.end(); ++iter) {
        std::string propName = iter->first;
        JSUtil::setProperty(ctx, obj, propName.c_str(), JSUtil::toJSValueRef(ctx, iter->second), kJSPropertyAttributeNone);
    }

    return obj;
}

vector<string> JSUtil::JSArrayToStringVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<string>(ctx, value, JSUtil::JSValueToString);
}
vector<double> JSUtil::JSArrayToDoubleVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<double>(ctx, value, JSUtil::JSValueToDouble);
}
vector<long> JSUtil::JSArrayToLongVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<long>(ctx, value, JSUtil::JSValueToLong);
}
vector<time_t> JSUtil::JSArrayToTimeTVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<time_t>(ctx, value, JSUtil::JSValueToTimeT);
}
vector<bool> JSUtil::JSArrayToBoolVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<bool>(ctx, value, JSUtil::JSValueToBoolean);
}

vector<unsigned char> JSUtil::JSArrayToUCharVector(JSContextRef ctx, JSValueRef value){
    return JSArrayToType_<unsigned char>(ctx, value, JSUtil::JSValueToOctet);
}

bool JSUtil::JSValueIsDateObject(JSContextRef ctx, JSValueRef jsValue) {
    JSValueRef exception = NULL;

    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    JSValueRef jsDate = getProperty(ctx, globalObj, "Date", &exception);
    if(exception)
        return false;

    JSObjectRef jsDateObj = ::JSValueToObject(ctx, jsDate, &exception);
    if(exception)
        return false;

    bool result = JSValueIsInstanceOfConstructor(ctx, jsValue, jsDateObj, &exception);
    if(exception)
        return false;

    return result;
}

void JSUtil::addPropertyName(JSPropertyNameAccumulatorRef propertyNames,
        const char* name)
{
    JSStringRef propertyName = NULL;
    propertyName = JSStringCreateWithUTF8CString(name);
    JSPropertyNameAccumulatorAddName(propertyNames, propertyName);
    JSStringRelease(propertyName);
}

}
}
