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

#ifndef _CONTENT_TYPES_H_
#define _CONTENT_TYPES_H_

#include <string>
#include <vector>
#include <map>

namespace DeviceAPI {
namespace Content {

// enum ContentDirectoryStorageType
#define TIZEN_CONTENT_DIRECTORY_STORAGE_TYPE_INTERNAL "INTERNAL"
#define TIZEN_CONTENT_DIRECTORY_STORAGE_TYPE_EXTERNAL "EXTERNAL"

// enum ContentType
#define TIZEN_CONTENT_TYPE_IMAGE "IMAGE"
#define TIZEN_CONTENT_TYPE_VIDEO "VIDEO"
#define TIZEN_CONTENT_TYPE_AUDIO "AUDIO"
#define TIZEN_CONTENT_TYPE_OTHER "OTHER"

// enum AudioContentLyricsType
#define TIZEN_AUDIO_CONTENT_LYRICS_TYPE_SYNCHRONIZED "SYNCHRONIZED"
#define TIZEN_AUDIO_CONTENT_LYRICS_TYPE_UNSYNCHRONIZED "UNSYNCHRONIZED"

// enum ImageContentOrientation
#define TIZEN_IMAGE_CONTENT_ORIENTATION_NORMAL "NORMAL"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_FLIP_HORIZONTAL "FLIP_HORIZONTAL"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_ROTATE_180 "ROTATE_180"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_FLIP_VERTICAL "FLIP_VERTICAL"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_TRANSPOSE "TRANSPOSE"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_ROTATE_90 "ROTATE_90"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_TRANSVERSE "TRANSVERSE"
#define TIZEN_IMAGE_CONTENT_ORIENTATION_ROTATE_270 "ROTATE_270"

// typedef ContentId
typedef std::string ContentId;
// typedef ContentDirectoryId
typedef std::string ContentDirectoryId;

} // Content
} // DeviceAPI

#endif // _CONTENT_TYPES_H_
