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

#ifndef __TIZEN_COMMON_PROPERTY_BAG_H__
#define __TIZEN_COMMON_PROPERTY_BAG_H__

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <map>

namespace DeviceAPI {
namespace Common{

class PropertyBag{
public:
    PropertyBag();
    virtual ~PropertyBag();

    void deleteProperty(JSContextRef ctx, const std::string& propertyName);
    void deleteProperty(JSContextRef ctx, JSStringRef propertyName);


    bool setProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef value);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef value);

    bool setProperty(JSContextRef ctx, const std::string& propertyName, double number);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, double number);

    bool setProperty(JSContextRef ctx, const std::string& propertyName, std::string& str);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, std::string& str);

    bool setProperty(JSContextRef ctx, const std::string& propertyName, const char *str);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, const char * str);

    bool setProperty(JSContextRef ctx, const std::string& propertyName, bool boolean);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, bool boolean);

    bool setProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef value, JSClassRef classRef);
    bool setProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef value, JSClassRef classRef);

    bool setFunctionProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef function);
    bool setFunctionProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef function);

    bool setArrayProperty(JSContextRef ctx, const std::string& propertyName, JSValueRef array);
    bool setArrayProperty(JSContextRef ctx, JSStringRef propertyName, JSValueRef array);

    JSValueRef getProperty(JSContextRef ctx, const std::string& propertyName);
    JSValueRef getProperty(JSContextRef ctx, JSStringRef propertyName);

    double getNumberProperty(JSContextRef ctx, const std::string& propertyName);
    double getNumberProperty(JSContextRef ctx, JSStringRef propertyName);

    double getDoubleProperty(JSContextRef ctx, const std::string& propertyName);
    double getDoubleProperty(JSContextRef ctx, JSStringRef propertyName);

    bool getBooleanProperty(JSContextRef ctx, const std::string& propertyName);
    bool getBooleanProperty(JSContextRef ctx, JSStringRef propertyName);

    std::string getStringProperty(JSContextRef ctx, const std::string& propertyName);
    std::string getStringProperty(JSContextRef ctx, JSStringRef propertyName);

    JSObjectRef getObjectProperty(JSContextRef ctx, const std::string& propertyName);
    JSObjectRef getObjectProperty(JSContextRef ctx, JSStringRef propertyName);

private:
    typedef std::map<const std::string, JSValueRef> propertyBagT;
    typedef std::map<JSContextRef, propertyBagT*> contextMapT;
    std::map<JSContextRef, propertyBagT*> mContextMap;

    std::map<const std::string, JSValueRef> * getBag(JSContextRef globalCtx);
    void removeItem(JSContextRef globalCtx, propertyBagT *bag, const std::string& propertyName);

};


} //Common
} //DeviceAPI


#endif //__TIZEN_COMMON_PROPERTY_BAG_H__
