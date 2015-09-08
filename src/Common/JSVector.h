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

#ifndef __TIZEN_COMMON_JSVECTOR_H__
#define __TIZEN_COMMON_JSVECTOR_H__

#include <JavaScriptCore/JavaScript.h>
#include <vector>
#include <memory>
#include <exception>
#include "JSUtil.h"
#include "Logger.h"
#include "GlobalContextManager.h"
#include "JSArray.h"

namespace DeviceAPI {
namespace Common {

/** Generic class encapsulating recommended JSArray use.
 *
 *  It is highly recommended to use JSArray instead of std::vector only when
 *  there is context available (using bare JSArray in cpp-layer class lead to
 *  necessity of supplying context in it's constructor which is inconvenient).
 *  This class encapsulates interchangeable use of std::vector and
 *  JSArray. When constructed the class use std::vector and when context appears
 *  second stage is entered - data is rewritten from std::vector to JSArray and
 *  all methods change their behaviour correspondingly.
 */
template <class T>
class JSVector {
public:
    JSVector(typename JSArray<T>::ToNativeFunction nativefun,
        typename JSArray<T>::ToJSFunction jsfun):
            mToNative(nativefun), mToJs(jsfun) {
        LOGD("entered");
    }

    // copy constructor, populating only vector m_array since initial state of
    // JSVector should be std::vector
    JSVector(const JSVector<T>& arr,
        typename JSArray<T>::ToNativeFunction nativefun,
        typename JSArray<T>::ToJSFunction jsfun):
            mToNative(nativefun), mToJs(jsfun) {
        LOGD("entered, arr.size = %d, m_js_array = %s", arr.size(),
            static_cast<bool>(m_js_array) ? "true" : "false");

        m_array = arr;
    }

    JSVector(const std::vector<T>& vec,
        typename JSArray<T>::ToNativeFunction nativefun,
        typename JSArray<T>::ToJSFunction jsfun):
            mToNative(nativefun), mToJs(jsfun) {
        LOGD("entered, vec.size = %d, m_js_array = %s", vec.size(),
            static_cast<bool>(m_js_array) ? "true" : "false");

        m_array = vec;
    }

    JSVector<T>& operator =(const JSVector<T>& arr) {
        LOGD("entered, arr.size = %d, m_js_array = %s, jsMode() = %s",
            arr.size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        if (jsMode()){
            // static_casting to vector to make deep copy of arr
            *m_js_array = static_cast<std::vector<T>>(arr);
        }
        else {
            m_array = arr;
        }

        return *this;
    }

    JSVector<T>& operator =(const std::vector<T>& vec) {
        LOGD("entered, vec.size = %d, m_js_array = %s, jsMode() = %s",
            vec.size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        if (jsMode()){
            m_js_array = std::shared_ptr<JSArray<T>>(
                    new JSArray<T>(getContext(), mToNative, mToJs));
            *m_js_array = vec;
        }
        else {
            m_array = vec;
        }

        return *this;
    }

    operator std::vector<T>() const
    {
        LOGD("entered, size = %d, m_js_array = %s, jsMode() = %s",
            size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        if (jsMode()) {
            return static_cast<std::vector<T>>(*m_js_array);
        }
        else {
            return m_array;
        }
    }

    // first use of getJSArray(JSContextRef) make current object move to jsMode
    JSObjectRef getJSArray(JSContextRef global_ctx)
    {
        LOGD("entered, size = %d, m_js_array = %s, jsMode() = %s",
            size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        // check if supplied context is the same as before
        if (m_js_array && m_js_array->getContext() != global_ctx) {
            LOGE("Wrong context supplied");
            throw Common::UnknownException("Wrong context supplied");
        }

        // check if context is "alive" (not-null and global for current code)
        if (!Common::GlobalContextManager::getInstance()
                ->isAliveGlobalContext(global_ctx)) {
            LOGE("Context is not alive");
            throw Common::UnknownException("Context is not alive");
        }

        if (!m_js_array) {
            // move data from m_array to m_js_array
            m_js_array = std::shared_ptr<JSArray<T>>(
                new JSArray<T>(global_ctx, mToNative, mToJs));
            *m_js_array = m_array;
            m_array.clear();
        }

        return *m_js_array;
    }

    size_t size() const {
        if (jsMode()) {
            return m_js_array->size();
        }
        else {
            return m_array.size();
        }
    }

    bool empty() const {
        LOGD("entered, size = %d, m_js_array = %s, jsMode() = %s",
            size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        return size() == 0;
    }

    void push_back(const T& val) {
        LOGD("entered, size = %d, m_js_array = %s, jsMode() = %s",
            size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        if (jsMode()) {
            if (!m_js_array->append(val)) {
                LOGE("jsarray alloc fail");
                // if append fails throw same exception as vector does
                throw std::bad_alloc();
            }
        }
        else {
            m_array.push_back(val);
        }
    }

    void clear() {
        LOGD("entered, size = %d, m_js_array = %s, jsMode() = %s",
            size(), static_cast<bool>(m_js_array) ? "true" : "false",
            jsMode() ? "true" : "false");

        if (jsMode()) {
            m_js_array->resize(0);
        }
        else {
            m_array.clear();
        }
    }

    JSContextRef getContext() {
        return m_js_array ? m_js_array->getContext() : NULL;
    }

private:
    std::vector<T> m_array;
    std::shared_ptr<JSArray<T>> m_js_array;

    typename JSArray<T>::ToNativeFunction mToNative;
    typename JSArray<T>::ToJSFunction mToJs;

    bool jsMode() const {
        return m_js_array && Common::GlobalContextManager::getInstance()
                ->isAliveGlobalContext(m_js_array->getContext());
    }
};


class JSStringVector : public JSVector<std::string>{
    static JSValueRef makeJSValue(JSContextRef ctx, std::string v){
        return JSUtil::toJSValueRef(ctx, v);
    }

    public:
        JSStringVector(const std::vector<std::string>& vec):
            JSVector<std::string>(vec, JSUtil::JSValueToString, makeJSValue){}
        JSStringVector():
            JSVector<std::string>(JSUtil::JSValueToString, makeJSValue){}

        JSStringVector& operator=( const std::vector<std::string>& list ){
            JSVector<std::string>::operator=(list);
            return *this;
        }
};


class JSLongVector : public JSVector<long>{
    static JSValueRef makeJSValue(JSContextRef ctx, long v){
        return JSUtil::toJSValueRef(ctx, v);
    }

    public:
        JSLongVector(const std::vector<long>& vec):
            JSVector<long>(vec, JSUtil::JSValueToLong,
                    makeJSValue){}
        JSLongVector():
            JSVector<long>(JSUtil::JSValueToLong, makeJSValue){}
        void operator=( const std::vector<long>& list ){
            JSVector<long>::operator=(list);
        }

};


class JSLongLongVector : public JSVector<long long int>{
    static JSValueRef makeJSValue(JSContextRef ctx, long long int v){
        return JSUtil::toJSValueRef(ctx, v);
    }

    public:
        JSLongLongVector(const std::vector<long long int>& vec):
            JSVector<long long int>(vec, JSUtil::JSValueToLongLong,
                    makeJSValue){}
        JSLongLongVector():
            JSVector<long long int>(JSUtil::JSValueToLongLong, makeJSValue){}
        void operator=( const std::vector<long long int>& list ){
            JSVector<long long int>::operator=(list);
        }

};


template <class ObjectSharedPtr, class ObjectJSClass>
class JSObjectVector : public JSVector<ObjectSharedPtr>{
    static JSValueRef makeJSValue(JSContextRef ctx, ObjectSharedPtr v){
        return ObjectJSClass::makeJSObject(ctx, v);
    }
    public:
        JSObjectVector(const std::vector<ObjectSharedPtr>& vec):
            JSVector<ObjectSharedPtr>(vec, ObjectJSClass::getPrivateObject,
                makeJSValue){}
        JSObjectVector():
            JSVector<ObjectSharedPtr>(ObjectJSClass::getPrivateObject,
                makeJSValue){}

        JSObjectVector& operator=( const std::vector<ObjectSharedPtr>& list ){
            JSVector<ObjectSharedPtr>::operator=(list);
            return *this;
        }
};


template <typename EnumT,
         EnumT (*ToEnumFun)(const std::string&),
         std::string (*ToStringFun)(EnumT)>
class JSEnumVector : public JSVector<EnumT>{
    static JSValueRef makeJSValue(JSContextRef ctx, EnumT v){
        return JSUtil::toJSValueRef(ctx, ToStringFun(v));
    }
    static EnumT makeNative(JSContextRef ctx, JSValueRef v){
        return ToEnumFun(JSUtil::JSValueToString(ctx, v));
    }
    public:
        JSEnumVector(const std::vector<EnumT>& vec):
            JSVector<EnumT>(vec, makeNative, makeJSValue){};
        JSEnumVector():
            JSVector<EnumT>(makeNative, makeJSValue){};

        JSEnumVector& operator=( const std::vector<EnumT>& list ){
            JSVector<EnumT>::operator=(list);
            return *this;
        }
};

} // Common
} // DeviceAPI
#endif // __TIZEN_COMMON_JSVECTOR_H__
