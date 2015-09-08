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
 * @file        JSFile.h
 */

#ifndef TIZENAPIS_TIZEN_JSFILE_H_
#define TIZENAPIS_TIZEN_JSFILE_H_

#include <memory>

#include <JavaScriptCore/JavaScript.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include "Node.h"
#include "File.h"

namespace DeviceAPI {
namespace Filesystem {

class JSFile
{
public:
    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static FilePtr getPrivateObject(JSContextRef context, JSValueRef value);
    static void setPrivateObject(JSObjectRef object, FilePtr data);
    static JSObjectRef makeJSObject(JSContextRef context, FilePtr ptr,
            Common::SecurityAccessor* srcSecurityAccessor);

private:
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
    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    /**
    * The callback invoked when collecting the names of an object's properties.
    */
    static void getPropertyNames(JSContextRef context,
        JSObjectRef object,
        JSPropertyNameAccumulatorRef propertyNames);

    /**
    * The callback invoked when an object is used as the target of an 'instanceof' expression.
    */
    static bool hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef possibleInstance,
        JSValueRef* exception);

    /**
    * Returns a URI for the file.
    */
    static JSValueRef toUri(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Returns list of all files of this directory.
    */
    static JSValueRef listFiles(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * WRT_GeneralError exception
    */
    static JSValueRef openStream(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * WRT_GeneralError exception
    */
    static JSValueRef readAsText(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * The operation is only for file except for directory.
    * Make sure the dest directory already exists.
    */
    static JSValueRef copyTo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * This operation is only for file not directory
    * The dest directory should exists in local file system, or the operation fails.
    */
    static JSValueRef moveTo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * create directory even through the parent directories do not  exist in local file system.
    */
    static JSValueRef createDirectory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Creates a new empty file in a specified location.
    */
    static JSValueRef createFile(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Resolves an existing file or directory relative to
    * the current directory this operation is performed on; and
    * returns a file handle for it.
    */
    static JSValueRef resolve(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
     * Removes all files from specified directory if recursive is true,
     * or just remove the directory itself when there is no files or directories underneath it
     */
    static JSValueRef deleteDirectory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Deletes a specified file.
    */
    static JSValueRef deleteFile(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    static bool isDestinationAllowed(PathPtr aDestination);

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

} //namespace DeviceAPI
} //namespace Filesystem

#endif //TIZENAPIS_TIZEN_JSFILE_H_
