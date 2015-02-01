//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _TIZEN_JS_PACKAGE_INFORMATION_H_
#define _TIZEN_JS_PACKAGE_INFORMATION_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "PackageInformation.h"

namespace DeviceAPI {
namespace Package {

#define TIZEN_INTERFACE_PACKAGE_INFORMATION             "PackageInformation"

// Fields
#define TIZEN_PACKAGE_INFORMATION_ID                     "id"
#define TIZEN_PACKAGE_INFORMATION_NAME                 "name"
#define TIZEN_PACKAGE_INFORMATION_ICONPATH             "iconPath"
#define TIZEN_PACKAGE_INFORMATION_VERSION                 "version"
#define TIZEN_PACKAGE_INFORMATION_TOTAL_SIZE             "totalSize"
#define TIZEN_PACKAGE_INFORMATION_DATA_SIZE             "dataSize"
#define TIZEN_PACKAGE_INFORMATION_LAST_MODIFIED         "lastModified"
#define TIZEN_PACKAGE_INFORMATION_AUTHOR                 "author"
#define TIZEN_PACKAGE_INFORMATION_TYPE                     "type"
#define TIZEN_PACKAGE_INFORMATION_DESCRIPTION            "description"
#define TIZEN_PACKAGE_INFORMATION_APP_IDS                "appIds"

class JSPackageInformation {
public:
    /*
     * This initializes this JS class in the JS Engine.
     */
    static JSClassRef getClassRef();

    static JSValueRef createJSObject(JSContextRef context, PackageInformation *pkgInfo);

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);


private:
    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This member variable contains the initialization values for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_classRef;

    static PackageInformation* getPrivData(JSObjectRef object);

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

};

}
}
#endif
