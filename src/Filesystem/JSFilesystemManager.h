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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/**
 * @file        JSFilesystemManager.h
 */

#ifndef TIZENAPIS_TIZEN_JSFILESYSTEMMANAGER_H_
#define TIZENAPIS_TIZEN_JSFILESYSTEMMANAGER_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>

#include <SecurityAccessor.h>
#include "Manager.h"

namespace DeviceAPI {
namespace Filesystem {

class JSFilesystemManager
{
  public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

  private:
    static Manager* getPrivateObject(JSContextRef context, JSValueRef value);

    /**
     * newPriv - WARNING: pointer is not owned
     *                    (use for example with &Manager::getInstance())
     */
    static void setPrivateObject(JSObjectRef object, Manager* newPriv);

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context,
            JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * The callback invoked when getting a property's value.
     */
    static JSValueRef getMaxPathLength(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * Resolves a location to a File handle.
     */
    static JSValueRef resolve(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argc,
            const JSValueRef argv[],
            JSValueRef* exception);

    /**
     * Get information about a storage based on it's label.
     */
    static JSValueRef getStorage(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argc,
            const JSValueRef argv[],
            JSValueRef* exception);

    /**
     * List the available storages on the device.
     * Get the list of available internal and external storage devices.
     */
    static JSValueRef getStorageList(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argc,
            const JSValueRef argv[],
            JSValueRef* exception);

    static JSValueRef addStorageStateListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argc,
            const JSValueRef argv[],
            JSValueRef* exception) ;

    static JSValueRef removeStorageStateListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argc,
            const JSValueRef argv[],
            JSValueRef* exception);

  private:
    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_functions[];

    /**
     * This structure describes a statically declared value property.
     */
    static JSStaticValue m_properties[];

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    static JSClassRef m_classRef;
};
}
}

#endif
