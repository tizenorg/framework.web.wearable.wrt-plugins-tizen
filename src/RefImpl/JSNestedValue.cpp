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

#include "JSNestedValue.h"
#include "NestedValue.h"

#include <ArgumentValidator.h>
#include <PlatformException.h>
#include <JSUtil.h>

#include <string>

using namespace DeviceAPI::Common;
using namespace std;

namespace DeviceAPI {
namespace Reference {

JSClassRef JSNestedValue::m_jsClassRef = NULL;

JSClassDefinition JSNestedValue::m_jsClassInfo = {
    0, // current (and only) version is 0
    kJSClassAttributeNone, //attributes
    "NestedValue", //class name
    NULL, // parent class
    JSNestedValue::m_property, //static values
    JSNestedValue::m_function, // static functions
    JSNestedValue::initialize, // initialize
    JSNestedValue::finalize, //finalize
    NULL, //hasProperty
    NULL, //getProperty
    NULL, //setProperty
    NULL, //deleteProperty
    NULL, //getPropertyNames
    NULL, // callAsFunction
    NULL, // constructor
    NULL,
    NULL // convertToType
};


JSStaticValue JSNestedValue::m_property[] = {
	{ "Number", JSNestedValue::getProperty, JSNestedValue::setProperty, kJSPropertyAttributeNone},
	{ "Message", JSNestedValue::getProperty, JSNestedValue::setProperty, kJSPropertyAttributeNone},
	{ 0, 0, 0, 0 }
};


JSStaticFunction JSNestedValue::m_function[] = {
    { "print", JSNestedValue::print, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};


const JSClassRef JSNestedValue::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}
const JSClassDefinition* JSNestedValue::getClassInfo()
{
    return &m_jsClassInfo;
}


boost::shared_ptr<NestedValue> JSNestedValue::getNative(JSContextRef ctx, JSValueRef value){
    JSObjectRef object = JSUtil::JSValueToObject(ctx, value);    
    NestedValueHolder* priv = static_cast<NestedValueHolder*>(JSObjectGetPrivate(object));
    if( priv == NULL || priv->obj == NULL )
        throw TypeMismatchException("Not NestedValue Type");
    return priv->obj;
}

JSObjectRef JSNestedValue::makeJSObject(JSContextRef ctx,  boost::shared_ptr < NestedValue > native){
    NestedValueHolder *priv = new NestedValueHolder;
    priv->obj = native;
    JSObjectRef obj = JSObjectMake(ctx, getClassRef(), priv);
    return obj;
}

JSObjectRef JSNestedValue::constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        
        boost::shared_ptr<NestedValue> native( new NestedValue(validator.toLong(0), validator.toString(1).c_str()) );
        JSObjectRef obj = makeJSObject(ctx, native);
        JSUtil::setProperty(ctx, obj, "constructor", constructor, kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete|kJSPropertyAttributeDontEnum);
        return obj;
    }catch( const BasePlatformException &err){
        JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(ctx, err);
        *exception = error;
        return error;
    }
}

void JSNestedValue::initialize(JSContextRef ctx, JSObjectRef object)
{
    printf("JSNestedValue::initialize()\n");
}

void JSNestedValue::finalize(JSObjectRef object)
{
    printf("JSNestedValue::finalize()\n");
    NestedValueHolder * priv = static_cast<NestedValueHolder*>(JSObjectGetPrivate(object));
    if( priv ){
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSNestedValue::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception){
    printf("JSNestedValue::getProperty()\n");
    NestedValueHolder* priv = static_cast<NestedValueHolder*>(JSObjectGetPrivate(object));
    
    string name = JSUtil::JSStringToString(context, propertyName);
    if( name == "Number" ){
        return JSUtil::toJSValueRef(context, priv->obj->getNumber());
    }else if( name == "Message"){
        return JSUtil::toJSValueRef(context, priv->obj->getMessage());
    }
    return NULL;
}

bool JSNestedValue::setProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception){
    printf("JSNestedValue::setProperty()\n");
    NestedValueHolder* priv = static_cast<NestedValueHolder*>(JSObjectGetPrivate(object));
    string name = JSUtil::JSStringToString(context, propertyName);
    try{
        JSValueRef values[1] = { value };
        ArgumentValidator validator(context, 1, values);
        if( name == "Number" ){
            priv->obj->setNumber( validator.toLong(0));
            return true;
        }else if( name == "Message"){
            priv->obj->setMessage( validator.toString(0));
            return true;
        }
    }catch( const BasePlatformException& err){
        printf("Silently error\n"); 
        return true;
    }
    return false;
}


JSValueRef JSNestedValue::print(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSNestedValue::print()\n");
    NestedValueHolder* priv = static_cast<NestedValueHolder*>(JSObjectGetPrivate(thisObject));
    if( priv && priv->obj){
        printf("Number : %d , Message: %s\n" , priv->obj->getNumber(), priv->obj->getMessage().c_str());
    }
    return JSValueMakeUndefined(ctx);
}


} // Reference
} // TizenApis

