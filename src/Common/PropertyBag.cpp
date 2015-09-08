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

#include "PropertyBag.h"
#include "Logger.h"
#include "JSUtil.h"
#include "GlobalContextManager.h"
#include "PlatformException.h"
#include <cmath>

using namespace std;

namespace DeviceAPI {
namespace Common {

PropertyBag::PropertyBag(){
}

PropertyBag::~PropertyBag(){
    contextMapT::iterator contextItr;
    for( contextItr = mContextMap.begin() ; contextItr != mContextMap.end() ; ++contextItr){
        propertyBagT::iterator propertyItr;
        propertyBagT * bag = contextItr->second;
        for( propertyItr = bag->begin(); propertyItr != bag->end(); ++propertyItr){
            JSValueRef v = propertyItr->second;
            if( v != NULL )
                JSValueUnprotect(contextItr->first, v);
        }
        delete bag;
    }
}

std::map<const std::string, JSValueRef> * PropertyBag::getBag(JSContextRef globalCtx){
    //find property bag for the context
    contextMapT::iterator itr;
    itr = mContextMap.find(globalCtx);
    propertyBagT *bag = NULL;

    if( itr == mContextMap.end() ){
        bag = new propertyBagT();
        mContextMap[globalCtx] = bag;
    }else{
        bag = mContextMap[globalCtx];
    }
    return bag;
}

void PropertyBag::removeItem(JSContextRef globalCtx, propertyBagT *bag, const std::string& propertyName){
    if( bag == NULL )
        return;
    propertyBagT::iterator item = bag->find(propertyName);
    if( item != bag->end() && item->second != NULL){
        JSValueUnprotect(globalCtx, item->second);
        bag->erase(item);
    }
}

void PropertyBag::deleteProperty(JSContextRef ctx, const std::string& propertyName){
    JSContextRef globalCtx = GlobalContextManager::getInstance()->getGlobalContext(ctx);
    if( globalCtx == NULL ){
        LOGE("Cannot found global context!");
        return;
    }
    propertyBagT *bag = getBag(globalCtx);
    removeItem(globalCtx, bag, propertyName);
}
void PropertyBag::deleteProperty(JSContextRef ctx, JSStringRef propertyName){
    deleteProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}


bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef value){
    JSContextRef globalCtx = GlobalContextManager::getInstance()->getGlobalContext(ctx);
    if( globalCtx == NULL ){
        LOGE("Cannot found global context!");
        return false;
    }
    propertyBagT *bag = getBag(globalCtx);
    if( bag == NULL)
        return false;
    removeItem(globalCtx, bag, propertyName);
    if( value != NULL )
        JSValueProtect(globalCtx, value);
    (*bag)[propertyName] = value;
    return true;
}

bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, double number){
    JSValueRef value = JSUtil::toJSValueRef(ctx, number);
    return setProperty(ctx, propertyName, value);
}

bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, std::string& str){
    JSValueRef value = JSUtil::toJSValueRef(ctx, str);
    return setProperty(ctx, propertyName, value);
}

bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, const char *str){
    string strvalue = string(str);
    JSValueRef value = JSUtil::toJSValueRef(ctx, strvalue);
    return setProperty(ctx, propertyName, value);
}

bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, bool boolean){
    JSValueRef value = JSUtil::toJSValueRef(ctx, boolean);
    return setProperty(ctx, propertyName, value);
}


bool PropertyBag::setProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef value, JSClassRef classRef){
    if( !JSValueIsObjectOfClass( ctx, value, classRef) ){
        LOGE("The value is incorrect type");
        return true;    //ignore set property
    }
    return setProperty(ctx, propertyName, value);
}


bool PropertyBag::setFunctionProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef function){
    JSObjectRef obj = JSUtil::JSValueToObject(ctx, function);
    if( !JSObjectIsFunction( ctx, obj) ){
        LOGE("The value is incorrect type");
        return true;    //ignore set property
    }
    return setProperty(ctx, propertyName, function);
}

bool PropertyBag::setArrayProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef array){
    if( !JSIsArrayValue(ctx, array)){
        LOGE("The value is not Array type");
        return true;    //ignore set property
    }
    return setProperty(ctx, propertyName, array);
}


bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef value){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName), value);
}
bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, double number){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName), number);
}
bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, std::string& str){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName), str);
}
bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, const char * str){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName), str);
}
bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, bool boolean){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName), boolean);
}
bool PropertyBag::setProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef value, JSClassRef classRef){
    return setProperty(ctx, JSUtil::JSStringToString(ctx, propertyName),value, classRef);
}
bool PropertyBag::setFunctionProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef function){
    return setFunctionProperty(ctx, JSUtil::JSStringToString(ctx, propertyName),function);
}
bool PropertyBag::setArrayProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef array){
    return setArrayProperty(ctx, JSUtil::JSStringToString(ctx, propertyName),array);
}



JSValueRef PropertyBag::getProperty(JSContextRef ctx, const std::string& propertyName){
    JSContextRef globalCtx = GlobalContextManager::getInstance()->getGlobalContext(ctx);
    if( globalCtx == NULL ){
        LOGE("Cannot found global context!");
        return NULL;
    }

    propertyBagT *bag = getBag(globalCtx);
    if( bag == NULL)
        return NULL;

    propertyBagT::iterator item = bag->find(propertyName);
    if( item == bag->end()){
        LOGE("Cannot found item");
        return NULL;
    }
    return item->second;
}

double PropertyBag::getNumberProperty(JSContextRef ctx, const std::string& propertyName){
    JSValueRef value = getProperty(ctx, propertyName);
    if( value == NULL )
        return 0.0;
    try{
        return JSUtil::JSValueToDouble(ctx, value);
    }catch(const BasePlatformException& err){
        LOGE("Cannot convert to number");
    }
    return std::nan("not number");
}

double PropertyBag::getDoubleProperty(JSContextRef ctx, const std::string& propertyName){
    JSValueRef value = getProperty(ctx, propertyName);
    if( value == NULL )
        return 0;
    return JSUtil::JSValueToDouble(ctx, value);
}

bool PropertyBag::getBooleanProperty(JSContextRef ctx, const std::string& propertyName){
    JSValueRef value = getProperty(ctx, propertyName);
    if( value == NULL )
        return false;
    return JSUtil::JSValueToBoolean(ctx, value);
}

std::string PropertyBag::getStringProperty(JSContextRef ctx, const std::string& propertyName){
    JSValueRef value = getProperty(ctx, propertyName);
    if( value == NULL )
        return "undefined";
    return JSUtil::JSValueToString(ctx, value);
}

JSObjectRef PropertyBag::getObjectProperty(JSContextRef ctx, const std::string& propertyName){
    JSValueRef value = getProperty(ctx, propertyName);
    if( value == NULL )
        return NULL;
    if( !JSValueIsObject(ctx, value) )
        return NULL;
    return JSUtil::JSValueToObject(ctx, value);
}

double PropertyBag::getNumberProperty(JSContextRef ctx, JSStringRef propertyName){
    return getNumberProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}
double PropertyBag::getDoubleProperty(JSContextRef ctx, JSStringRef propertyName){
    return getDoubleProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}
bool PropertyBag::getBooleanProperty(JSContextRef ctx, JSStringRef propertyName){
    return getBooleanProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}
std::string PropertyBag::getStringProperty(JSContextRef ctx, JSStringRef propertyName){
    return getStringProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}
JSObjectRef PropertyBag::getObjectProperty(JSContextRef ctx, JSStringRef propertyName){
    return getObjectProperty(ctx, JSUtil::JSStringToString(ctx, propertyName));
}
JSValueRef PropertyBag::getProperty(JSContextRef ctx, JSStringRef propertyName){
    return getProperty(ctx, JSUtil::JSStringToString(ctx,propertyName));
}



} //Common
} //DeviceAPI
