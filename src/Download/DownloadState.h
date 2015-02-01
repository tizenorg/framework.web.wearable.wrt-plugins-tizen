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

#ifndef __TIZEN_DOWNLOAD_STATE_H__
#define __TIZEN_DOWNLOAD_STATE_H__

namespace DeviceAPI {
namespace Download {

#define TIZEN_ENUM_DOWNLOAD_STATE_QUEUED "QUEUED"
#define TIZEN_ENUM_DOWNLOAD_STATE_DOWNLOADING "DOWNLOADING"
#define TIZEN_ENUM_DOWNLOAD_STATE_PAUSED "PAUSED"
#define TIZEN_ENUM_DOWNLOAD_STATE_CANCELED "CANCELED"
#define TIZEN_ENUM_DOWNLOAD_STATE_COMPLETED "COMPLETED"
#define TIZEN_ENUM_DOWNLOAD_STATE_FAILED "FAILED"

} // Download
} // DeviceAPI

#endif // __TIZEN_DOWNLOAD_STATE_H__