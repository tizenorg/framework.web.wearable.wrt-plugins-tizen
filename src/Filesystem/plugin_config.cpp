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
 * @file        plugin_config.cpp
 */

#include <map>
#include <utility>

#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <Export.h>
#include "Enums.h"

#include "plugin_config_impl.h"
#include "plugin_config.h"

#define FILESYSTEM_FEATURE_API_READ "http://tizen.org/privilege/filesystem.read"
#define FILESYSTEM_FEATURE_API_WRITE "http://tizen.org/privilege/filesystem.write"

namespace DeviceAPI {
namespace Filesystem {


#pragma GCC visibility push(default)
const char* FILESYSTEM_PARAM_LOCATION = "param:location";
const char* FILESYSTEM_DEVICE_CAP_READ = "filesystem.read";
const char* FILESYSTEM_DEVICE_CAP_WRITE = "filesystem.write";

const char* FILESYSTEM_FUNCTION_API_MGR_RESOLVE_ID = "manager_resolve";
const char* FILESYSTEM_FUNCTION_API_MGR_RESOLVE = "resolve";

const char* FILESYSTEM_FUNCTION_API_GET_STORAGE = "getStorage";
const char* FILESYSTEM_FUNCTION_API_LIST_STORAGE = "listStorage";
const char* FILESYSTEM_FUNCTION_API_ADD_STORAGE_LISTENER = "addStorageStateChangeListener";
const char* FILESYSTEM_FUNCTION_API_REMOVE_STORAGE_LISTENER = "removeStorageStateChangeListener";

const char* FILESYSTEM_FUNCTION_API_RESOLVE = "resolve";
const char* FILESYSTEM_FUNCTION_API_TO_URI = "toURI";
const char* FILESYSTEM_FUNCTION_API_LIST_FILES = "listFiles";
const char* FILESYSTEM_FUNCTION_API_OPEN_STREAM = "openStream";
const char* FILESYSTEM_FUNCTION_API_READ_AS_TEXT = "readAsText";
const char* FILESYSTEM_FUNCTION_API_COPY_TO = "copyTo";
const char* FILESYSTEM_FUNCTION_API_MOVE_TO = "moveTo";
const char* FILESYSTEM_FUNCTION_API_CREATE_DIR = "createDirectory";
const char* FILESYSTEM_FUNCTION_API_CREATE_FILE = "createFile";
const char* FILESYSTEM_FUNCTION_API_DELETE_DIR = "deleteDirectory";
const char* FILESYSTEM_FUNCTION_API_DELETE_FILE = "deleteFile";

const char* FILESYSTEM_FUNCTION_API_CLOSE = "close";
const char* FILESYSTEM_FUNCTION_API_READ = "read";
const char* FILESYSTEM_FUNCTION_API_READ_BYTES = "readBytes";
const char* FILESYSTEM_FUNCTION_API_READ_BASE64 = "readBase64";
const char* FILESYSTEM_FUNCTION_API_WRITE = "write";
const char* FILESYSTEM_FUNCTION_API_WRITE_BYTES = "writeBytes";
const char* FILESYSTEM_FUNCTION_API_WRITE_BASE64 = "writeBase64";
#pragma GCC visibility pop

static WrtDeviceApis::Commons::FunctionMapping createFilesystemFunctions();

static WrtDeviceApis::Commons::FunctionMapping FilesystemFunctions =
    createFilesystemFunctions();

#pragma GCC visibility push(default)
DEFINE_FUNCTION_GETTER(Filesystem, FilesystemFunctions);
#pragma GCC visibility pop

static WrtDeviceApis::Commons::FunctionMapping createFilesystemFunctions()
{
    using namespace WrtDeviceApis::Commons;

    /**
     * Device capabilities
     */

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_FILESYSTEM_READ,
                          FILESYSTEM_DEVICE_CAP_READ);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_FILESYSTEM_WRITE,
                          FILESYSTEM_DEVICE_CAP_WRITE);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_FILESYSTEM_READ);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_FILESYSTEM_READ, DEVICE_CAP_FILESYSTEM_READ);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_FILESYSTEM_WRITE);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_FILESYSTEM_WRITE,
                       DEVICE_CAP_FILESYSTEM_WRITE);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_READ, FILESYSTEM_FEATURE_API_READ);
    ACE_CREATE_FEATURE(FEATURE_WRITE, FILESYSTEM_FEATURE_API_WRITE);

    ACE_CREATE_FEATURE_LIST(FILESYSTEM_FEATURES_FILESYSTEM_READ);

    ACE_ADD_API_FEATURE(FILESYSTEM_FEATURES_FILESYSTEM_READ,
                        FEATURE_READ);

    ACE_CREATE_FEATURE_LIST(FILESYSTEM_FEATURES_FILESYSTEM_WRITE);

    ACE_ADD_API_FEATURE(FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
                        FEATURE_WRITE);

    /**
     * Functions
     */

    FunctionMapping FilesystemMapping;

    AceFunction mgrResolveFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MGR_RESOLVE,
            FILESYSTEM_FUNCTION_API_MGR_RESOLVE,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST); // check of device-cap will be done inside resolve()

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_MGR_RESOLVE_ID,
                                 mgrResolveFunc));

    AceFunction getStorageFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_STORAGE,
            FILESYSTEM_FUNCTION_API_GET_STORAGE,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_GET_STORAGE,
                                 getStorageFunc));

    AceFunction listStorageFunc = ACE_CREATE_FUNCTION(
            FUNCTION_LIST_STORAGE,
            FILESYSTEM_FUNCTION_API_LIST_STORAGE,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_LIST_STORAGE,
                                 listStorageFunc));

    AceFunction addStorageListenerFunc = ACE_CREATE_FUNCTION(
                FUNCTION_ADD_STORAGE_LISTENER,
                FILESYSTEM_FUNCTION_API_ADD_STORAGE_LISTENER,
                FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
                EMPTY_DEVICE_LIST);

        FilesystemMapping.insert(std::make_pair(
                                     FILESYSTEM_FUNCTION_API_ADD_STORAGE_LISTENER,
                                     addStorageListenerFunc));

        AceFunction removeStorageListenerFunc = ACE_CREATE_FUNCTION(
                FUNCTION_REMOVE_STORAGE_LISTENER,
                FILESYSTEM_FUNCTION_API_REMOVE_STORAGE_LISTENER,
                FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
                EMPTY_DEVICE_LIST);

        FilesystemMapping.insert(std::make_pair(
                                     FILESYSTEM_FUNCTION_API_REMOVE_STORAGE_LISTENER,
                                     removeStorageListenerFunc));

    AceFunction resolveFunc = ACE_CREATE_FUNCTION(
            FUNCTION_RESOLVE,
            FILESYSTEM_FUNCTION_API_RESOLVE,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_RESOLVE,
                                 resolveFunc));

    AceFunction toUriFunc = ACE_CREATE_FUNCTION(
            FUNCTION_TO_URI,
            FILESYSTEM_FUNCTION_API_TO_URI,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_TO_URI,
                                 toUriFunc));

    AceFunction listFilesFunc = ACE_CREATE_FUNCTION(
            FUNCTION_LIST_FILES,
            FILESYSTEM_FUNCTION_API_LIST_FILES,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_LIST_FILES,
                                 listFilesFunc));

    AceFunction openStreamFunc = ACE_CREATE_FUNCTION(
            FUNCTION_OPEN_STREAM,
            FILESYSTEM_FUNCTION_API_OPEN_STREAM,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_OPEN_STREAM,
                                 openStreamFunc));

    AceFunction readAsTextFunc = ACE_CREATE_FUNCTION(
            FUNCTION_READ_AS_TEXT,
            FILESYSTEM_FUNCTION_API_READ_AS_TEXT,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_READ_AS_TEXT,
                                 readAsTextFunc));




    AceFunction copyToFunc = ACE_CREATE_FUNCTION(
            FUNCTION_COPY_TO,
            FILESYSTEM_FUNCTION_API_COPY_TO,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_COPY_TO,
                                 copyToFunc));

    AceFunction moveToFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MOVE_TO,
            FILESYSTEM_FUNCTION_API_MOVE_TO,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_MOVE_TO,
                                 moveToFunc));

    AceFunction createDirFunc = ACE_CREATE_FUNCTION(
            FUNCTION_CREATE_DIR,
            FILESYSTEM_FUNCTION_API_CREATE_DIR,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_CREATE_DIR,
                                 createDirFunc));

    AceFunction createFileFunc = ACE_CREATE_FUNCTION(
            FUNCTION_CREATE_FILE,
            FILESYSTEM_FUNCTION_API_CREATE_FILE,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_CREATE_FILE,
                                 createFileFunc));

    AceFunction deleteDirFunc = ACE_CREATE_FUNCTION(
            FUNCTION_DELETE_DIR,
            FILESYSTEM_FUNCTION_API_DELETE_DIR,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_DELETE_DIR,
                                 deleteDirFunc));

    AceFunction deleteFileFunc = ACE_CREATE_FUNCTION(
            FUNCTION_DELETE_FILE,
            FILESYSTEM_FUNCTION_API_DELETE_FILE,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_DELETE_FILE,
                                 deleteFileFunc));

    AceFunction closeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_CLOSE,
            FILESYSTEM_FUNCTION_API_CLOSE,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_CLOSE,
                                 closeFunc));

    AceFunction readFunc = ACE_CREATE_FUNCTION(
            FUNCTION_READ,
            FILESYSTEM_FUNCTION_API_READ,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_READ,
                                 readFunc));

    AceFunction readBytesFunc = ACE_CREATE_FUNCTION(
            FUNCTION_READ_BYTES,
            FILESYSTEM_FUNCTION_API_READ_BYTES,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_READ_BYTES,
                                 readBytesFunc));

    AceFunction readBase64Func = ACE_CREATE_FUNCTION(
            FUNCTION_READ_BASE64,
            FILESYSTEM_FUNCTION_API_READ_BASE64,
            FILESYSTEM_FEATURES_FILESYSTEM_READ,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_READ_BASE64,
                                 readBase64Func));

    AceFunction writeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_WRITE,
            FILESYSTEM_FUNCTION_API_WRITE,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_WRITE,
                                 writeFunc));

    AceFunction writeBytesFunc = ACE_CREATE_FUNCTION(
            FUNCTION_WRITE_BYTES,
            FILESYSTEM_FUNCTION_API_WRITE_BYTES,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_WRITE_BYTES,
                                 writeBytesFunc));

    AceFunction writeBase64Func = ACE_CREATE_FUNCTION(
            FUNCTION_WRITE_BASE64,
            FILESYSTEM_FUNCTION_API_WRITE_BASE64,
            FILESYSTEM_FEATURES_FILESYSTEM_WRITE,
            EMPTY_DEVICE_LIST);

    FilesystemMapping.insert(std::make_pair(
                                 FILESYSTEM_FUNCTION_API_WRITE_BASE64,
                                 writeBase64Func));

    return FilesystemMapping;
}

}
}
