#include "JSArray.h"
#include "PlatformException.h"

namespace DeviceAPI {
namespace Common {

JSArrayBase::JSArrayBase( JSContextRef context, JSObjectRef array ):mContext(context), mArray(array){
    if(!JSIsArrayValue(context, array)){
        throw TypeMismatchException("The type is not array");
    }
    JSValueProtect(context, array);
}

JSArrayBase::JSArrayBase( JSContextRef context):mContext(context){
    JSValueRef exception = NULL;
    mArray = JSObjectMakeArray( context, 0, NULL, &exception);
    if(exception != NULL){
        throw UnknownException(context, exception);
    }
    JSValueProtect(context, mArray);
}

JSArrayBase::~JSArrayBase(){
    JSValueUnprotect(mContext, mArray);
}

size_t JSArrayBase::size() const {
    return JSGetArrayLength(mContext, mArray);
}

void JSArrayBase::resize(size_t array_size){
    JSUtil::setProperty(mContext, mArray, "length", JSUtil::toJSValueRef(mContext, static_cast<double>(array_size)), kJSPropertyAttributeNone, NULL);
}

JSValueRef JSArrayBase::get(int index){
    return JSGetArrayElement(mContext, mArray, index);
}

bool JSArrayBase::set( int index, JSValueRef value){
    bool t = JSSetArrayElement(mContext, mArray, index, value);
    return t;
}

bool JSArrayBase::append( JSValueRef value ){
    return set( size(), value);
}

}//Common
}//DeviceAPI
