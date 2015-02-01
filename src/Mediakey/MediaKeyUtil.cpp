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

#include "MediaKeyUtil.h"

#include <PlatformException.h>
#include <Logger.h>

#include <sstream>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace MediaKey {

std::string& MediaKeyUtil::getMediaKeyErrorMessage(const int errorCode){
    switch(errorCode) {
        case MEDIA_KEY_ERROR_INVALID_PARAMETER:
            return errInvalidParameter;
        case MEDIA_KEY_ERROR_OPERATION_FAILED:
            return errOperationFailed;
        default:
            return errUnknown;
    }
}

std::string MediaKeyUtil::getMediaKeyLogMessage(const int errorCode, const std::string &hint){
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getMediaKeyErrorMessage(errorCode) << ", " << errorCode;
    return std::string(ss.str());
}


MediaKeyType MediaKeyUtil::mediakeyTypeToString(media_key_e key){

    LOGD("Entered");

    switch(key)
    {
        case MEDIA_KEY_PLAY:
                LOGD("key : MEDIA_KEY_PLAY");
                return MediaKeyType::MEDIA_KEY_TYPE_PLAY;
                break;
        case MEDIA_KEY_STOP:
                LOGD("key : MEDIA_KEY_STOP");
                return MediaKeyType::MEDIA_KEY_TYPE_STOP;
                break;
        case MEDIA_KEY_PAUSE:
                LOGD("key : MEDIA_KEY_PAUSE");
                return MediaKeyType::MEDIA_KEY_TYPE_PAUSE;
                break;
        case MEDIA_KEY_PREVIOUS:
                LOGD("key : MEDIA_KEY_PREVIOUS");
                return MediaKeyType::MEDIA_KEY_TYPE_PREVIOUS;
                break;
        case MEDIA_KEY_NEXT:
                LOGD("key : MEDIA_KEY_NEXT");
                return MediaKeyType::MEDIA_KEY_TYPE_NEXT;
                break;
        case MEDIA_KEY_FASTFORWARD:
                LOGD("key : MEDIA_KEY_FASTFORWARD");
                return MediaKeyType::MEDIA_KEY_TYPE_FASTFORWARD;
                break;
        case MEDIA_KEY_REWIND:
                LOGD("key : MEDIA_KEY_REWIND");
                return MediaKeyType::MEDIA_KEY_TYPE_REWIND;
                break;
        case MEDIA_KEY_PLAYPAUSE:
                LOGD("key : MEDIA_KEY_PLAYPAUSE");
                return MediaKeyType::MEDIA_KEY_TYPE_PLAYPAUSE;
                break;
        case MEDIA_KEY_UNKNOWN:
                LOGW("media_key callback gave MEDIA_KEY_UNKNOWN");
                throw TypeMismatchException("Invalid media_key");
        default:
                LOGW("media_key callback gave wrong key value");
                throw TypeMismatchException("Invalid media_key");
    }
}

MediaKeyEventType MediaKeyUtil::mediakeyEventTypeToString(media_key_event_e status){

    LOGD("Entered");

    switch(status)
    {
        case MEDIA_KEY_STATUS_PRESSED:
                LOGD("status : MEDIA_KEY_STATUS_PRESSED");
                return MediaKeyEventType::MEDIA_KEY_EVENT_PRESSED;
                break;
        case MEDIA_KEY_STATUS_RELEASED:
                LOGD("status : MEDIA_KEY_STATUS_RELEASED");
                return MediaKeyEventType::MEDIA_KEY_EVENT_RELEASED;
                break;
        case MEDIA_KEY_STATUS_UNKNOWN:
                LOGW("media_key callback gave MEDIA_KEY_STATUS_UNKNOWN");
                throw TypeMismatchException("Invalid media_key_status");
        default:
                LOGW("media_key callback gave wrong status value");
                throw TypeMismatchException("Invalid media_key_status");
    }
}

} // MediaKey
} // DeviceAPI
