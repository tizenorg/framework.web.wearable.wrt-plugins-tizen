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

#ifndef _JSUTIL_H_
#define _JSUTIL_H_
#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <map>
#include <vector>
#include "PlatformException.h"
#include <ctime>


namespace DeviceAPI {
namespace Common{

class JSUtil{
public:
    /**
     * @brief Gets a property from an object.
     *
     * @remarks
     * if pass NULL in exception, when occurred error, it throw C++ exception(TypeMismatchException).
     *
     * @param[in] ctx The execution context to use.
     * @param[in] object The JSObject whose property you want to get.
     * @param[in] name The name of property
     * @param[out] exception A pointer to a JSValueRef in which to store an exception, if any.
     *
     * @exception TypeMismatchException
     */
    static JSValueRef getProperty(JSContextRef ctx , JSObjectRef object, const char *name, JSValueRef *exception=NULL);

    /**
     * @brief Sets a property on an object.
     *
     * @remarks
     * if pass NULL in exception, when occurred error, it throw C++ exception(TypeMismatchException).
     *
     * @param[in] ctx The execution context to use.
     * @param[in] object The JSObject whose property you want to set.
     * @param[in] name The name of property
     * @param[in] attributes A logically ORed set of JSPropertyAttributes to give to the property.
     * @param[out] exception A pointer to a JSValueRef in which to store an exception, if any.
     *
     * @exception TypeMismatchException
     */
    static void setProperty(JSContextRef ctx , JSObjectRef object, const char *name, JSValueRef value, JSPropertyAttributes attributes, JSValueRef *exception=NULL);

    /**
     * @brief Converts a JavaScript string to STL string
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSString to convert.
     *
     * @return A STL string with the result of conversion
     */
    static std::string JSStringToString(JSContextRef ctx, JSStringRef str);

    /**
     * @brief Converts a JavaScript value to STL string
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A STL string with the result of conversion
     *
     * @exception TypeMismatchException
     */
    static std::string JSValueToString(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to long number and returns the resulting long number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static long JSValueToLong(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to unsigned long number and returns the resulting unsigned long number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static unsigned long JSValueToULong(JSContextRef ctx, JSValueRef value);


    /**
     * @brief Converts a JavaScript value to long long number and returns the resulting long long number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static long long JSValueToLongLong(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to unsigned long long number and returns the resulting unsigned long long number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static unsigned long long JSValueToULongLong(JSContextRef ctx, JSValueRef value);


    /**
     * @brief Converts a JavaScript value to double number and returns the resulting double number.
     *
     * @remarks TypeMismatchException is thrown when the result of conversion was NaN(Not a Number).
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static double JSValueToDouble(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to number and returns the resulting number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static double JSValueToNumber(JSContextRef ctx, JSValueRef value);


    /**
     * @brief Converts a JavaScript value to byte(signed) number and returns the resulting byte(signed) number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static signed char JSValueToByte(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to Octet(unsigned) number and returns the resulting Octet(unsigned) number.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static unsigned char JSValueToOctet(JSContextRef ctx, JSValueRef value);


    /**
     * @brief Converts a JavaScript value to boolean and returns the resulting bollean
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     */
    static bool JSValueToBoolean(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to time_t and returns the resulting time_t.
     *
     * @remarks TypeMismatchException is thrown when the value was not Date type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static time_t JSValueToTimeT(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to tm and returns the resulting tm.
     *
     * @remarks TypeMismatchException is thrown when the value was not Date type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static std::tm JSValueToDateTm(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to tm as UTC time and returns the resulting tm.
     *
     * @remarks TypeMismatchException is thrown when the value was not Date type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The result of conversion
     *
     * @exception TypeMismatchException
     */
    static std::tm JSValueToDateTmUTC(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to object and returns the resulting object.
     *
     * @remarks TypeMismatchException is thrown when the value was not Object type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The JSObject result of conversion
     *
     * @exception TypeMismatchException
     */
    static JSObjectRef JSValueToObject(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to STL string vector and returns the resulting STL string vector
     *
     * @remarks TypeMismatchException is thrown when the array element could not converts to string.\n
     * If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A STL string vector
     *
     * @exception TypeMismatchException
     */
    static std::vector<std::string> JSArrayToStringVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to double number vector and returns the resulting double number vector
     *
     * @remarks TypeMismatchException is thrown when the array element could not converts to double.\n
     * If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A double number vector
     *
     * @exception TypeMismatchException
     */
    static std::vector<double> JSArrayToDoubleVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to long number vector and returns the resulting long number vector
     *
     * @remarks TypeMismatchException is thrown when the array element could not converts to long.\n
     * If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A long number vector
     *
     * @exception TypeMismatchException
     */
    static std::vector<long> JSArrayToLongVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to time_t vector and returns the resulting time_t vector
     *
     * @remarks TypeMismatchException is thrown when the array element could not converts to time_t.\n
     * If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A time_t vector
     *
     * @exception TypeMismatchException
     */
    static std::vector<time_t> JSArrayToTimeTVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to boolean vector and returns the resulting boolean vector
     *
     * @remarks If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A boolean vector
     */
    static std::vector<bool> JSArrayToBoolVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to unsigned char vector and returns the resulting unsigned char vector
     *
     * @remarks If the value is not Array object, Will return empty vector
     *
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return A unsigned char vector
     */
    static std::vector<unsigned char> JSArrayToUCharVector(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Converts a JavaScript value to map<string,string> and returns the resulting object.
     *
     * @remarks TypeMismatchException is thrown when the value was not Object type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The JSValue to convert.
     *
     * @return The JSObject result of conversion
     *
     * @exception TypeMismatchException
     */
    static std::map<std::string, std::string> JSValueToStringMap(JSContextRef ctx, JSValueRef value);

    /**
     * @brief Creates a JavaScript value of the string type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] str The STL string to assign to the newly created JSValue. The newly created JSValue retains string, and releases it upon garbage collection.
     *
     * @return A JSValue of the string type, representing the value of string.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::string& str);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The long to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const long value);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The unsigned long to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const unsigned long value);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The long long to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const long long value);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The unsigned long long to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const unsigned long long value);
    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The double to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const double value);

    /**
     * @brief Creates a JavaScript value of the boolean type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The bool to assign to the newly created JSValue.
     *
     * @return A JSValue of the boolean type, representing the value of boolean.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const bool value);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The signed char to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const signed char value);

    /**
     * @brief Creates a JavaScript value of the number type.
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The signed char to assign to the newly created JSValue.
     *
     * @return A JSValue of the number type, representing the value of number.
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const unsigned char value);



    /**
     * @brief Creates a JavaScript value of the string Array type.
     *
     * @remarks UnknownException is thrown when could not create Array object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The STL string vector to assign to the newly created JSArray
     *
     * @return A JSArray of the string type
     *
     * @exception UnknownException
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::vector<std::string>& value);


    /**
     * @brief Creates a JavaScript value of the number Array type.
     *
     * @remarks UnknownException is thrown when could not create Array object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The long vector to assign to the newly created JSArray
     *
     * @return A JSArray of the number type
     *
     * @exception UnknownException
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::vector<long>& value);

    /**
     * @brief Creates a JavaScript value of the number Array type.
     *
     * @remarks UnknownException is thrown when could not create Array object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The double vector to assign to the newly created JSArray
     *
     * @return A JSArray of the number type
     *
     * @exception UnknownException
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::vector<double>& value);


    /**
     * @brief Creates a JavaScript value of the boolean Array type.
     *
     * @remarks UnknownException is thrown when could not create Array object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The boolean vector to assign to the newly created JSArray
     *
     * @return A JSArray of the boolean type
     *
     * @exception UnknownException
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::vector<bool>& value);

    /**
     * @brief Creates a JavaScript value of the string map type.
     *
     * @remarks UnknownException is thrown when could not create Array object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The string map to assign to the newly created JSValue
     *
     * @return A JSValue of the string map type
     *
     * @exception UnknownException
     */
    static JSValueRef toJSValueRef(JSContextRef ctx, const std::map<std::string, std::string>& value);

    /**
     * @brief Creates a JavaScript Date object with time_t value
     *
     * @remarks TypeMismatchException is thrown when could not create Date object.\n
     *
     * @param[in] ctx The execution context to use.
     * @param[in] value The time_t value to create
     *
     * @return A JSObject that is a Date.
     *
     * @exception TypeMismatchException
     */
    static JSObjectRef makeDateObject(JSContextRef ctx, const time_t value);


    template<class T>
    static std::vector<T> JSArrayToType_(JSContextRef ctx, JSValueRef value, T (*convert)(JSContextRef, JSValueRef)){
        std::vector<T> result;
        if( !JSIsArrayValue(ctx, value)){
            return result;
        }
        JSObjectRef arrayobj = JSUtil::JSValueToObject(ctx, value);

        for (std::size_t i = 0; i < JSGetArrayLength(ctx, arrayobj); ++i) {
            JSValueRef element = JSGetArrayElement(ctx, arrayobj, i);
            T v = convert(ctx, element);
            result.push_back(v);
        }
        return result;
    }

    template<class T>
    static JSValueRef toJSValueRef_(JSContextRef ctx, const std::vector<T>& value){
        JSValueRef valueArray[value.size()];
        for( unsigned int i = 0 ; i < value.size(); i++){
            valueArray[i] = toJSValueRef(ctx,value[i]);
        }
        JSValueRef exception = NULL;
        JSObjectRef jsResult = JSObjectMakeArray(ctx, value.size(), valueArray, &exception);
        if (exception != NULL) {
            throw UnknownException(ctx, exception);
        }
        return jsResult;
    };

    template<class T>
    static JSValueRef toJSValueRef_(JSContextRef ctx, const std::vector<std::pair<std::string, std::vector<T>>>& value)
    {
        JSObjectRef js_result = JSCreateArrayObject(ctx, 0, NULL);
        if (NULL == js_result) {
            throw Common::UnknownException("Could not create js array object");
        }

        for (std::size_t i = 0; i < value.size(); ++i) {
            JSValueRef tmp_val = toJSValueRef_(ctx, value[i].second);
            if (!JSSetArrayElementInArrayList(ctx, js_result,
                    value[i].first.c_str(), tmp_val)) {
                throw Common::UnknownException("Could not insert value into js array");
            }
        }
        return js_result;
    }

    /**
     * @brief Adds property name to PropertyNameAccumulator
     *
     * @remarks Used in getPropertyNames\n
     *
     * @param[in] propertyNames PropertyNameAccumulator to be supplemented.
     * @param[in] name The name of the property to be added.
     */
    static void addPropertyName(JSPropertyNameAccumulatorRef propertyNames,
            const char* name);

private:
    static bool JSValueIsDateObject(JSContextRef ctx, JSValueRef jsValue);
};

}}

#endif //_JSUTIL_H_


