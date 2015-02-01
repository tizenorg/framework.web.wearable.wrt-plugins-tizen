//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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


#ifndef _FILESYSTEM_PLUGIN_CONFIG_IMPL_H_
#define _FILESYSTEM_PLUGIN_CONFIG_IMPL_H_

#define TIZEN_FILESYSTEM_FILESYSTEM_MANAGER_CLASS "filesystem"

namespace DeviceAPI {
namespace Filesystem {

extern const char* FILESYSTEM_FUNCTION_API_MGR_RESOLVE_ID;
extern const char* FILESYSTEM_FUNCTION_API_GET_STORAGE;
extern const char* FILESYSTEM_FUNCTION_API_LIST_STORAGE;
extern const char* FILESYSTEM_FUNCTION_API_ADD_STORAGE_LISTENER;
extern const char* FILESYSTEM_FUNCTION_API_REMOVE_STORAGE_LISTENER;

extern const char* FILESYSTEM_FUNCTION_API_RESOLVE;
extern const char* FILESYSTEM_FUNCTION_API_TO_URI;
extern const char* FILESYSTEM_FUNCTION_API_LIST_FILES;
extern const char* FILESYSTEM_FUNCTION_API_OPEN_STREAM;
extern const char* FILESYSTEM_FUNCTION_API_READ_AS_TEXT;
extern const char* FILESYSTEM_FUNCTION_API_COPY_TO;
extern const char* FILESYSTEM_FUNCTION_API_MOVE_TO;
extern const char* FILESYSTEM_FUNCTION_API_CREATE_DIR;
extern const char* FILESYSTEM_FUNCTION_API_CREATE_FILE;
extern const char* FILESYSTEM_FUNCTION_API_DELETE_DIR;
extern const char* FILESYSTEM_FUNCTION_API_DELETE_FILE;

extern const char* FILESYSTEM_FUNCTION_API_CLOSE;
extern const char* FILESYSTEM_FUNCTION_API_READ;
extern const char* FILESYSTEM_FUNCTION_API_READ_BYTES;
extern const char* FILESYSTEM_FUNCTION_API_READ_BASE64;
extern const char* FILESYSTEM_FUNCTION_API_WRITE;
extern const char* FILESYSTEM_FUNCTION_API_WRITE_BYTES;
extern const char* FILESYSTEM_FUNCTION_API_WRITE_BASE64;

} // namespace DeviceAPI
} // namespace Filesystem

#endif // _FILESYSTEM_PLUGIN_CONFIG_IMPL_H_
