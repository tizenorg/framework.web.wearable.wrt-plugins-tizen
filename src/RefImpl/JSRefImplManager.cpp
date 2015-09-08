/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <SecurityExceptions.h>
#include <CommonsJavaScript/Converter.h>
#include <GlobalContextManager.h>
#include <ArgumentValidator.h>
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include <Logger.h>
#include <MultiCallbackUserData.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <JSArray.h>

#include "TimeTracer.h"
#include "JSRefImplManager.h"
#include "RefImplManager.h"
#include "JSSub.h"
#include "JSSuper.h"
#include "Sub.h"
#include "NestedValue.h"
#include "JSNestedValue.h"

using namespace std;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;


namespace DeviceAPI {
namespace Reference {

JSClassRef JSRefImplManager::m_jsClassRef = NULL;

JSClassDefinition JSRefImplManager::m_jsClassInfo = {
    0,                                       // current (and only) version is 0
    kJSClassAttributeNone,        //attributes
    "RefImplManager",                        //class name
    NULL,                                 // parent class
    JSRefImplManager::m_property,                                 //static values
    JSRefImplManager::m_function,     // static functions
    JSRefImplManager::initialize,         // initialize
    JSRefImplManager::finalize,          //finalize
    NULL,                                //hasProperty
    JSRefImplManager::getNormalProperty,   //getProperty
    JSRefImplManager::setNormalProperty,  //setProperty
    NULL,                                //deleteProperty
    NULL,                                //getPropertyNames
    NULL,                               // callAsConstructor
    NULL,                               // constructor
    NULL,
    NULL // convertToType
};

JSStaticValue JSRefImplManager::m_property[] = {
	{ "Number", getReadOnlyNumber, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
	{ "Sub", getReadOnlySub, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
	{ 0, 0, 0, 0 }
};



JSStaticFunction JSRefImplManager::m_function[] = {
    { "syncToSync", JSRefImplManager::syncToSync, kJSPropertyAttributeNone },
    { "syncToAsync", JSRefImplManager::syncToAsync, kJSPropertyAttributeNone },
    { "asyncToAsync", JSRefImplManager::asyncToAsync, kJSPropertyAttributeNone },
    { "addListener", JSRefImplManager::addListener, kJSPropertyAttributeNone },
    { "removeListener", JSRefImplManager::removeListener, kJSPropertyAttributeNone },
    { "fire", JSRefImplManager::fire, kJSPropertyAttributeNone },
    { "argtest", JSRefImplManager::argtest, kJSPropertyAttributeNone },
    { "arraytest", JSRefImplManager::arraytest, kJSPropertyAttributeNone },
    { "isSuper", JSRefImplManager::isSuper, kJSPropertyAttributeNone },
    { "isSub", JSRefImplManager::isSub, kJSPropertyAttributeNone },
    { "setCallback", JSRefImplManager::setCallback, kJSPropertyAttributeNone },
    { "unsetCallback", JSRefImplManager::unsetCallback, kJSPropertyAttributeNone },
    { "fireCallback", JSRefImplManager::fireCallback, kJSPropertyAttributeNone },
    { "callbackObjectTest", JSRefImplManager::callbackObjectTest, kJSPropertyAttributeNone },
    { "updateNumber", JSRefImplManager::updateNumber, kJSPropertyAttributeNone },
    { "updateSub", JSRefImplManager::updateSub, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSRefImplManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSRefImplManager::getClassInfo()
{
    return &m_jsClassInfo;
}

void JSRefImplManager::initialize(JSContextRef ctx, JSObjectRef object)
{
	TIME_TRACER_INIT();
}

void JSRefImplManager::finalize(JSObjectRef object)
{
	TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"RefImpl");
	TIME_TRACER_RELEASE();
}


JSValueRef JSRefImplManager::getReadOnlyNumber(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception){
    return RefImplManager::getInstance()->mLocalProperty.getProperty(context,propertyName);
}

JSValueRef JSRefImplManager::getReadOnlySub(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception){
    return RefImplManager::getInstance()->mLocalProperty.getProperty(context,propertyName);
}

JSValueRef JSRefImplManager::getNormalProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception){
    string name = JSUtil::JSStringToString(context, propertyName);
    if( name == "Number" )
        return NULL;
    if( name == "Sub")
        return NULL;
    return RefImplManager::getInstance()->mLocalProperty.getProperty(context,propertyName);
}

bool JSRefImplManager::setNormalProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception){
    string name = JSUtil::JSStringToString(context, propertyName);
    if( name == "Number" )
        return false;
    if( name == "Sub")
        return false;
    return RefImplManager::getInstance()->mLocalProperty.setProperty(context,propertyName, value);
}

JSValueRef JSRefImplManager::syncToSync(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
	TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
    try{
        Converter convert(ctx);
        ArgumentValidator validator(ctx, argumentCount, arguments);

        int a = validator.toLong(0);
        int b = validator.toLong(1);
        int result = RefImplManager::getInstance()->syncToSync(a,b);
	TIME_TRACER_ITEM_END(__FUNCTION__, 1);
        return convert.toJSValueRef(result);
    }catch( const TypeMismatchException& err ){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }catch( const ConversionException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception,  "TypeMismatchError" , "can't convert to int");    
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::syncToAsync(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
	TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);

        int a = validator.toLong(0);
        int b = validator.toLong(1);
        RefCallbackUserData *callback = new RefCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(ctx),a,b);
        callback->setSuccessCallback(validator.toFunction(2));
        callback->setErrorCallback(validator.toFunction(3, true));

        RefImplManager::getInstance()->syncToAsync(callback);
        
    }catch( const TypeMismatchException& err ){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
	TIME_TRACER_ITEM_END(__FUNCTION__, 1);
	TIME_TRACER_EXPORT_REPORT();
	TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"RefImpl");
    return JSValueMakeUndefined(ctx);

}

JSValueRef JSRefImplManager::asyncToAsync(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        int a = validator.toLong(0);
        int b = validator.toLong(1);
        RefCallbackUserData *callback = new RefCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(ctx),a,b);
        callback->setSuccessCallback(validator.toFunction(2));
        callback->setErrorCallback(validator.toFunction(3, true));

        RefImplManager::getInstance()->asyncToAsync(callback);

    }catch( const TypeMismatchException& err ){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }

    return JSValueMakeUndefined(ctx);

}

JSValueRef JSRefImplManager::argtest(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        string tocheck = validator.toString(0);

        if( validator.isOmitted(0) || validator.isNull(0)){
            printf("argument is omitted\n");
        }

        if( tocheck == "Boolean"){
            bool value = validator.toBool(1);
            printf("arg[1] = %d\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "Long"){
            long value = validator.toLong(1);
            printf("arg[1] = %ld\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "ULong"){
            unsigned long value = validator.toULong(1);
            printf("arg[1] = %lu\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "ULongLong"){
            unsigned long long value = validator.toULongLong(1);
            printf("arg[1] = %llu\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "Byte"){
            signed char value = (int)validator.toByte(1);
            printf("arg[1] = %d\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "Octet"){
            unsigned char value = (int)validator.toOctet(1);
            printf("arg[1] = %d\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }
        else if( tocheck == "String"){
            string value = validator.toString(1);
            printf("arg[1] = %s\n", value.c_str());
            return JSUtil::toJSValueRef(ctx, value);
        }else if( tocheck == "Object"){
            JSObjectRef value = validator.toObject(1);
            printf("arg[1] = %p\n", value);
            return value;
        }else if( tocheck == "Date"){
            time_t value = validator.toTimeT(1);
            printf("arg[1] = %ld\n", value);
            return JSUtil::makeDateObject(ctx, value);
        }else if( tocheck == "Double" ){
            double value = validator.toDouble(1);
            printf("arg[1] = %f\n", value);
            return JSUtil::toJSValueRef(ctx, value);
        }else if( tocheck == "Function"){
            JSObjectRef value = validator.toFunction(1);
            printf("arg[1] = %p\n", value);
            return value;
        }else if( tocheck == "Array"){
            JSObjectRef value = validator.toArrayObject(1);
            printf("arg[1] = %p\n", value);
            return value;
        }else{
            printf("usage: argtest([Long|String|Object|Date|Double|Function], ...)\n");
            return JSUtil::toJSValueRef(ctx, "usage: argtest([Long|String|Object|Date|Double|Function], ...)");
        }
    }catch( const TypeMismatchException& err ){
        printf("TypeMismatch exception %s : %s\n", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef makeJSBool(JSContextRef ctx, bool v){
    return JSUtil::toJSValueRef(ctx, v);
}

JSValueRef makeNestedValue(JSContextRef ctx,  boost::shared_ptr < NestedValue > native){
    return JSNestedValue::makeJSObject(ctx, native);
}

JSValueRef JSRefImplManager::arraytest(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        string type = validator.toString(0);
        JSObjectRef jsArray = validator.toArrayObject(1, true);
        if( type == "String" && jsArray != NULL){
            JSStringArray receivedArray(ctx, jsArray);

            // basic getting test
            for( unsigned int i = 0 ; i < receivedArray.size() ; i++){
                string element = receivedArray[i];
                printf("received array[%d] = %s\n", i, element.c_str());
            }

            // JSArray to JSArray Copy test
            JSStringArray copyTest(ctx);
            copyTest = receivedArray;

            for( unsigned int i = 0 ; i < copyTest.size() ; i++){
                string element = copyTest[i];
                printf("copyTest array[%d] = %s\n", i, element.c_str());
            }

            // to Native Vector converting test
            vector<string> tmp = receivedArray;
            for( unsigned int i = 0 ; i < tmp.size() ; i++){
                printf("to Native Vector [%d] = %s\n", i, tmp[i].c_str());
            }

            //Copy from vector
            vector<string> aa;
            aa.push_back("one");
            aa.push_back("two");
            aa.push_back("three");
            JSStringArray newArray(ctx);
            newArray = aa;
            for( unsigned int i = 0 ; i < newArray.size() ; i++){
                string element = newArray[i];
                printf("Copy from Vector[%d] = %s\n", i, element.c_str());
            }

            // Modification test
            newArray[newArray.size()] = "last";
            newArray[0] = "zero";
            printf("Modification test\n");
            printf("except:zero,two,three,last\n");

            return newArray;
        }
        else if( type == "Long" && jsArray != NULL ){
            JSLongArray receivedArray(ctx, jsArray);

            //Basic getting test
            for( unsigned int i = 0 ; i < receivedArray.size() ; i++){
                long v = receivedArray[i];
                printf("received array[%d] = %d\n", i, v );
            }

            // JSArray to JSArray Copy test
            JSLongArray copyTest(ctx);
            copyTest = receivedArray;

            for( unsigned int i = 0 ; i < copyTest.size() ; i++){
                long v = copyTest[i];
                printf("copyTest array[%d] = %d\n", i, v);
            }

            // to Native Vector converting test
            vector<long> tmp = receivedArray;
            for( unsigned int i = 0 ; i < tmp.size() ; i++){
                printf("to Native Vector [%d] = %d\n", i, tmp[i]);
            }

            //Copy from vector
            vector<long> aa;
            aa.push_back(1);
            aa.push_back(2);
            aa.push_back(3);
            JSLongArray newArray(ctx);
            newArray = aa;
            for( unsigned int i = 0 ; i < newArray.size() ; i++){
                long v = newArray[i];
                printf("Copy from Vector[%d] = %d\n", i, v);
            }

            // Modification test
            newArray[newArray.size()] = 99;
            newArray[0] = 0;
            printf("Modification test\n");
            printf("except:0,2,3,99\n");

            return newArray;
        }
        else if( type == "Boolean" && jsArray != NULL ){
            JSArray<bool> receivedArray(ctx, jsArray, JSUtil::JSValueToBoolean, makeJSBool);

            //Basic getting test
            for( unsigned int i = 0 ; i < receivedArray.size() ; i++){
                printf("received array[%d] = %s\n", i, receivedArray[i] ? "true" : "false" );
            }

            // JSArray to JSArray Copy test
            JSArray<bool> copyTest(ctx,  JSUtil::JSValueToBoolean, makeJSBool);
            copyTest = receivedArray;

            for( unsigned int i = 0 ; i < copyTest.size() ; i++){
                printf("copyTest array[%d] = %s\n", i, copyTest[i] ? "true" : "false" );
            }

            // to Native Vector converting test
            vector<bool> tmp = receivedArray;
            for( unsigned int i = 0 ; i < tmp.size() ; i++){
                printf("to Native Vector[%d] = %s\n", i, tmp[i] ? "true" : "false" );
            }

            //Copy from vector
            vector<bool> aa;
            aa.push_back(true);
            aa.push_back(false);
            aa.push_back(true);
            JSArray<bool> newArray(ctx,  JSUtil::JSValueToBoolean, makeJSBool);
            newArray = aa;
            for( unsigned int i = 0 ; i < newArray.size() ; i++){
                printf("Copy from Vector[%d] = %s\n", i, newArray[i] ? "true" : "false" );
            }

            // Modification test
            newArray[newArray.size()] = true;
            newArray[0] = false;
            printf("Modification test\n");
            printf("except:false,false,true,false\n");
            return newArray;

        }
        else if( type == "NestedValue" && jsArray != NULL ){
            JSArray<boost::shared_ptr<NestedValue>> receivedArray(ctx, jsArray, JSNestedValue::getNative, makeNestedValue);
            for( unsigned int i = 0 ; i< receivedArray.size() ; i++){
                boost::shared_ptr<NestedValue> ptr = receivedArray[i];
                ptr->print();
            }
            JSArray<boost::shared_ptr<NestedValue>> newArray(ctx, JSNestedValue::getNative, makeNestedValue);
            newArray = receivedArray;
            newArray[0] = boost::shared_ptr<NestedValue>(new NestedValue(0, "newAdded!"));
            for( unsigned int i = 0 ; i< newArray.size() ; i++){
                boost::shared_ptr<NestedValue> ptr = newArray[i];
                ptr->print();
            }
            return newArray;
        }
        else{
            printf("usage: arraytest([Long|String|Boolean], array)\n");
            return JSUtil::toJSValueRef(ctx, "usage: arraytest([Long|String|Boolean], array)");
        }
        return JSValueMakeUndefined(ctx);
    }catch( const TypeMismatchException& err ){
        printf("TypeMismatch exception %s : %s\n", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }catch( const BasePlatformException& err){
        printf("exception %s : %s\n", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);

}

JSValueRef JSRefImplManager::addListener(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef fun = validator.toFunction(0);
        boost::shared_ptr<CallbackUserData> callback(new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(ctx)));
        callback->setSuccessCallback(fun);
        long id = RefImplManager::getInstance()->addListener(callback);
        return JSUtil::toJSValueRef(ctx, id);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::removeListener(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        int id = validator.toLong(0);
        RefImplManager::getInstance()->removeListener(id);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::fire(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    RefImplManager::getInstance()->fireListener();
    return JSValueMakeUndefined(ctx);
}



JSValueRef JSRefImplManager::isSuper(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef test = validator.toObject(0);
        if( JSValueIsObjectOfClass(ctx, test, JSSuper::getClassRef()) )
            return JSUtil::toJSValueRef(ctx, true);
        else
            return JSUtil::toJSValueRef(ctx, false);
    }catch( const BasePlatformException& err){
        printf("exception %s %s\n", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::isSub(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef test = validator.toObject(0);
        if( JSValueIsObjectOfClass(ctx, test, JSSub::getClassRef()) )
            return JSUtil::toJSValueRef(ctx, true);
        else
            return JSUtil::toJSValueRef(ctx, false);
    }catch( const BasePlatformException& err){
        printf("exception %s %s\n", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::setCallback(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef jsCallbackObj = validator.toObject(0);
        boost::shared_ptr<MultiCallbackUserData> callback(new MultiCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(ctx), jsCallbackObj));
        RefImplManager::getInstance()->setCallbackObject(GlobalContextManager::getInstance()->getGlobalContext(ctx), callback);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}
JSValueRef JSRefImplManager::unsetCallback(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    RefImplManager::getInstance()->unsetCallbackObject(GlobalContextManager::getInstance()->getGlobalContext(ctx));
    return JSValueMakeUndefined(ctx);
}
JSValueRef JSRefImplManager::fireCallback(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        string functionName= validator.toString(0);
        RefImplManager::getInstance()->namedCallbackFire(GlobalContextManager::getInstance()->getGlobalContext(ctx), functionName.c_str());
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::callbackObjectTest(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        printf("validator.toCallbackObject(0, true, \"fun1\", \"fun2\", \"fun3\", NULL);\n");
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef obj = validator.toCallbackObject(0, true, "fun1", "fun2", "fun3", NULL);
        printf("returnd obj = %p\n", obj);
    }catch( const BasePlatformException& err){
        printf("exception %s %s\n", err.getName().c_str(), err.getMessage().c_str());
    }
    try{
        printf("validator.toCallbackObject(0, false, \"fun1\", \"fun2\", \"fun3\", NULL);\n");
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef obj = validator.toCallbackObject(0, false, "fun1", "fun2", "fun3", NULL);
        printf("returnd obj = %p\n", obj);
    }catch( const BasePlatformException& err){
        printf("exception %s %s\n", err.getName().c_str(), err.getMessage().c_str());
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::updateNumber(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        int readonlyValue= validator.toLong(0);
        RefImplManager::getInstance()->mLocalProperty.setProperty(ctx, "Number", (double)readonlyValue);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSRefImplManager::updateSub(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    try{
        ArgumentValidator validator(ctx, argumentCount, arguments);
        JSObjectRef o = validator.toObject(0, JSSub::getClassRef());
        RefImplManager::getInstance()->mLocalProperty.setProperty(ctx, "Sub", o);
    }catch( const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
    return JSValueMakeUndefined(ctx);
}

} // Reference
} // TizenApis

