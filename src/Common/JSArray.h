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

#ifndef __TIZEN_COMMON_JSARRAY_H__
#define __TIZEN_COMMON_JSARRAY_H__

#include <JavaScriptCore/JavaScript.h>
#include <vector>
#include "JSUtil.h"

namespace DeviceAPI {
namespace Common {

class JSArrayBase{
public:
    JSArrayBase( JSContextRef context, JSObjectRef array );
    JSArrayBase( JSContextRef context );

    virtual ~JSArrayBase();
    size_t size() const;
    void resize(size_t size);
    JSValueRef get(int index);
    bool set(int index, JSValueRef value);
    bool append(JSValueRef value);

protected:
    JSContextRef mContext;
    JSObjectRef mArray;
};


template <typename T>
class JSArray : protected JSArrayBase{
    friend class ItemProxy;
public:
    typedef JSValueRef (*ToJSFunction)(JSContextRef, T);
    typedef T (*ToNativeFunction)(JSContextRef, JSValueRef );


    JSArray( JSContextRef context, JSObjectRef array, ToNativeFunction nativefun, ToJSFunction jsfun)
       :JSArrayBase(context,array),mToNative(nativefun), mToJs(jsfun) {
    }
    JSArray( JSContextRef context, ToNativeFunction nativefun, ToJSFunction jsfun)
       :JSArrayBase(context),mToNative(nativefun), mToJs(jsfun) {
    }

    ~JSArray(){
    }

    class ItemProxy {
        JSArray<T> *mArray;
        int mIndex;
        public:
            ItemProxy(JSArray<T>* array, int index):mArray(array), mIndex(index){
            }
            operator T(){
                return mArray->mToNative(mArray->mContext, mArray->get(mIndex));
            }
            ItemProxy& operator=( const T native){
                JSValueRef v = mArray->mToJs(mArray->mContext, native);
                mArray->set(mIndex, v);
                return *this;
            }
            ItemProxy& operator=( const ItemProxy& other){
                JSValueRef v = other.mArray->get(other.mIndex);
                mArray->set(mIndex, v);
                return *this;
            }

    };
    size_t size() const{
        return JSArrayBase::size();
    }

    void resize(size_t array_size){
        JSArrayBase::resize(array_size);
    }

    bool empty() const{
        return size() == 0;
    }

    bool append( T v){
        return JSArrayBase::set( size(), mToJs(mContext, v));
    }

    ItemProxy operator[]( int index ){
        return ItemProxy(this, index);
    }

    operator JSObjectRef(){
        return mArray;
    }

    operator std::vector<T>(){
        std::vector<T> v;
        size_t length = size();
        for( unsigned int i = 0 ; i < length ; i++){
            JSValueRef t = get(i);
            T tmp = mToNative(mContext, t);
            v.push_back(tmp);
        }
        return v;
    }

    void operator=( const std::vector<T>& list ){
        overwrite(list);
    }

    void operator=( const JSArray<T>& rhs){
        resize(rhs.size());
        for(unsigned int i = 0 ; i < rhs.size(); i++){
            set(i, rhs.get(i));
        }
    }

    JSContextRef getContext(){
        return mContext;
    }

protected:
    void overwrite( const std::vector<T>& list ){
        unsigned int i;
        unsigned int listSize = list.size();
        resize(listSize);
        for( i = 0 ; i < listSize ; i++){
            JSValueRef v = mToJs(mContext, list[i]);
            set(i, v);
        }
    }

private:
    ToNativeFunction mToNative;
    ToJSFunction mToJs;
};


class JSStringArray : public JSArray<std::string>{
    static JSValueRef makeJSValue(JSContextRef ctx, std::string v){
        return JSUtil::toJSValueRef(ctx, v);
    }
    public:
        // We need to pass pointer to function converting
        // std::string -> JSValueRef. JSUtil::toJSValueRef is overloaded so we
        // specify which version to use here by defining makeJSValue function.
        JSStringArray(JSContextRef ctx, JSObjectRef array): JSArray<std::string>(ctx, array, JSUtil::JSValueToString, makeJSValue){}
        JSStringArray(JSContextRef ctx): JSArray<std::string>(ctx, JSUtil::JSValueToString, makeJSValue){}
        // We need to duplicate this function from JSArray because otherwise
        // compiler see only assignment operator for JSArray and it doesn't
        // propagate to derived classes.
        void operator=( const std::vector<std::string>& list ){
            JSArray<std::string>::operator=(list);
        }
};


class JSLongArray : public JSArray<long>{
    static JSValueRef makeJSValue(JSContextRef ctx, long v){
        return JSUtil::toJSValueRef(ctx, v);
    }
    public:
        JSLongArray(JSContextRef ctx, JSObjectRef array): JSArray<long>(ctx, array, JSUtil::JSValueToLong, makeJSValue){}
        JSLongArray(JSContextRef ctx): JSArray<long>(ctx, JSUtil::JSValueToLong, makeJSValue){}
        void operator=( const std::vector<long>& list ){
            JSArray<long>::operator=(list);
        }

};


template <class ObjectSharedPtr, class ObjectJSClass>
class JSObjectArray : public JSArray<ObjectSharedPtr>{
    static JSValueRef makeJSValue(JSContextRef ctx, ObjectSharedPtr v){
        return ObjectJSClass::makeJSObject(ctx, v);
    }
    public:
        // We need to pass pointer to function converting
        // ObjectSharedPtr -> JSValueRef. ObjectJSClass::makeJSObject returns
        // JSObjectRef so we use makeJSValue function to convert JSObjectRef to
        // JSValueRef.
        JSObjectArray(JSContextRef ctx, JSObjectRef array):
            JSArray<ObjectSharedPtr>(ctx, array,
                ObjectJSClass::getPrivateObject, makeJSValue){}
        JSObjectArray(JSContextRef ctx):
            JSArray<ObjectSharedPtr>(ctx,
                ObjectJSClass::getPrivateObject, makeJSValue){}
        void operator=( const std::vector<ObjectSharedPtr>& list ){
            JSArray<ObjectSharedPtr>::operator=(list);
        }
};

}
}
#endif //__TIZEN_COMMON_JSARRAY_H__

