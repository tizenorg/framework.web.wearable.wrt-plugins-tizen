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


#include "ArgumentValidator.h"
#include "PlatformException.h"
#include "JSUtil.h"

#include <limits>
#include <dlog.h>
#include <math.h>
#include <stdarg.h>

using namespace std;

namespace DeviceAPI {
namespace Common{

ArgumentValidator::ArgumentValidator(JSContextRef ctx, int argc, const JSValueRef* argv):mContext(ctx),mArgc(argc), mArgv(argv){
}

ArgumentValidator::~ArgumentValidator(){
}

JSValueRef ArgumentValidator::getArgument(int index, bool nullable) const{
    if( index < mArgc ){
        return mArgv[index];
    }

    JSValueRef value = NULL;
    if( nullable )
        value = JSValueMakeNull(mContext);
    else
        value = JSValueMakeUndefined(mContext);
    return value;
}

bool ArgumentValidator::isOmitted(int index){
    if( index < mArgc)
        return false;
    return true;
}
bool ArgumentValidator::isNull(int index){
    if( !isOmitted(index) && JSValueIsNull(mContext, mArgv[index]) ){
        return true;
    }
    return false;
}
bool ArgumentValidator::isUndefined(int index){
    if( !isOmitted(index) && JSValueIsUndefined(mContext, mArgv[index]) ){
        return true;
    }
    return false;
}

double ArgumentValidator::toNumber(int index, bool nullable, double defaultvalue) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return defaultvalue;
    }
    return JSUtil::JSValueToNumber(mContext, value);
}

long ArgumentValidator::toLong(int index, bool nullable, long defaultvalue) const{
    return static_cast<long>(toLongLong(index,nullable,defaultvalue));
}

signed char ArgumentValidator::toByte(int index, bool nullable, signed char defaultvalue) const{
    return static_cast<signed char>(toLong(index,nullable,defaultvalue));
}

unsigned char ArgumentValidator::toOctet(int index, bool nullable, unsigned char defaultvalue) const{
    return static_cast<unsigned char>(toULong(index,nullable,defaultvalue));
}

unsigned long ArgumentValidator::toULong(int index, bool nullable, unsigned long defaultvalue) const{
    return static_cast<unsigned long>(toLongLong(index,nullable,defaultvalue));
}

long long ArgumentValidator::toLongLong(int index, bool nullable, long long defaultvalue) const{
    double double_value = toNumber(index,nullable,defaultvalue);
    if(isnan(double_value)) {
        double_value = 0.0;
    }
    return static_cast<long long>(double_value);
}

unsigned long long ArgumentValidator::toULongLong(int index, bool nullable, unsigned long long defaultvalue) const{
    return static_cast<unsigned long long>(toLongLong(index,nullable,defaultvalue));
}

double ArgumentValidator::toDouble(int index, bool nullable, double defaultvalue) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return defaultvalue;
    }
    return JSUtil::JSValueToDouble(mContext, value);
}

std::string ArgumentValidator::toString(int index, bool nullable, const  string & defaultvalue) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return defaultvalue;
    }

    std::string result;

    JSValueRef exception = NULL;
    JSStringRef str = JSValueToStringCopy(mContext, value, &exception);
    if (exception != NULL) {
        throw TypeMismatchException(mContext, exception);
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

bool ArgumentValidator::toBool(int index, bool nullable, bool defaultvalue) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return defaultvalue;
    }
    bool boolvalue = JSValueToBoolean(mContext, value);
    return boolvalue;
}

time_t ArgumentValidator::toTimeT(int index, bool nullable, time_t defaultvalue) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return defaultvalue;
    }
    return JSUtil::JSValueToTimeT(mContext, value);
}

JSObjectRef ArgumentValidator::toObject(int index, bool nullable) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return NULL;
    }
    if( !JSValueIsObject(mContext, value) ){
        throw TypeMismatchException("Value is not Object");
    }

    JSValueRef exception = NULL;
    JSObjectRef object = JSValueToObject(mContext, value, &exception);
    if( exception ){
        throw TypeMismatchException(mContext, exception);
    }
    return object;
}

JSObjectRef ArgumentValidator::toObject(int index, JSClassRef info, bool nullable) const{
    JSObjectRef obj = toObject(index, nullable);
    if( obj == NULL )
        return NULL;
    if( !JSValueIsObjectOfClass( mContext, obj, info) ){
        throw TypeMismatchException("Value is not correct type");
    }
    return obj;
}

JSValueRef ArgumentValidator::toJSValueRef(int index, bool nullable) const{
    return getArgument(index, nullable);
}

JSObjectRef ArgumentValidator::toFunction(int index, bool nullable) const{
    JSObjectRef obj = toObject(index, nullable);
    if( obj == NULL && nullable){
        return NULL;
    }
    if( !JSObjectIsFunction( mContext, obj )){
        throw TypeMismatchException("Value is not function");
    }
    return obj;
}

JSObjectRef ArgumentValidator::toArrayObject(int index, bool nullable) const{
    JSValueRef value = getArgument(index, nullable);
    if( JSValueIsNull(mContext, value) && nullable){
        return NULL;
    }

    if( !JSIsArrayValue(mContext, value)){
        throw TypeMismatchException("Type is not Array");
    }

    JSValueRef exception = NULL;
    JSObjectRef obj = JSValueToObject(mContext, value, &exception);
    if( exception != NULL )
        throw TypeMismatchException(mContext, exception);
    return obj;
}


std::vector<std::string> ArgumentValidator::toStringVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<std::string>();
    }
    return JSUtil::JSArrayToStringVector(mContext, value);
}

std::vector<long> ArgumentValidator::toLongVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<long>();
    }
    return JSUtil::JSArrayToLongVector(mContext, value);
}


std::vector<double> ArgumentValidator::toDoubleVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<double>();
    }
    return JSUtil::JSArrayToDoubleVector(mContext, value);
}

std::vector<time_t> ArgumentValidator::toTimeTVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<time_t>();
    }
    return JSUtil::JSArrayToTimeTVector(mContext, value);
}

std::vector<bool> ArgumentValidator::toBoolVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<bool>();
    }
    return JSUtil::JSArrayToBoolVector(mContext, value);
}

std::vector<unsigned char> ArgumentValidator::toUCharVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<unsigned char>();
    }
    return JSUtil::JSArrayToUCharVector(mContext, value);
}

std::vector<JSValueRef> ArgumentValidator::toJSValueRefVector(int index, bool nullable) const{
    JSObjectRef value = toArrayObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::vector<JSValueRef>();
    }

    std::vector<JSValueRef> result;
    for (std::size_t i = 0; i < JSGetArrayLength(mContext, value); ++i) {
        JSValueRef element = JSGetArrayElement(mContext, value, i);
        result.push_back(element);
    }
    return result;
}

std::map<std::string, std::string> ArgumentValidator::toStringMap(int index, bool nullable) const{
    JSObjectRef value = toObject(index, nullable);
    if( value == NULL || JSValueIsNull(mContext, value) ){
        return std::map<std::string, std::string>();
    }

    return JSUtil::JSValueToStringMap(mContext, value);
}

JSObjectRef ArgumentValidator::toCallbackObject(int index, bool nullable, const char *callback, ...) const{
    JSObjectRef obj = toObject(index, nullable);
    if( obj == NULL && nullable){
        return NULL;
    }
    va_list var_args;
    va_start (var_args, callback);
    const char * check = callback;
    while( check != NULL ){
        JSStringRef propertyName = JSStringCreateWithUTF8CString(check);
        bool has = JSObjectHasProperty(mContext, obj, propertyName);
        JSStringRelease(propertyName);
        if( has ){
            JSObjectRef o = JSUtil::JSValueToObject(mContext, JSUtil::getProperty(mContext, obj, check));
            if( !JSObjectIsFunction(mContext, o) ){
                va_end(var_args);
                throw TypeMismatchException("Property is not function object");
            }
        }
        check = static_cast<const char *>(va_arg(var_args, const char *));
    }
    va_end(var_args);
    return obj;
}



}
}
