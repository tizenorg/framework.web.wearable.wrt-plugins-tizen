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

#ifndef __TIZEN_AUDIO_CONTENT_LYRICS_H__
#define __TIZEN_AUDIO_CONTENT_LYRICS_H__

#include <MultiCallbackUserData.h>

#include "ContentTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class AudioContentLyrics;
typedef std::shared_ptr<AudioContentLyrics> AudioContentLyricsPtr;

class AudioContentLyrics
{
public:
    AudioContentLyrics();
    virtual ~AudioContentLyrics();

    std::string getType() const;
    void setType(std::string type);

    std::vector<unsigned long> getTimestamps() const;
    void setTimestamps(std::vector<unsigned long> timestamps);

    std::vector<std::string> getTexts() const;
    void setTexts(std::vector<std::string> texts);

private:
    std::string m_type;
    std::vector<unsigned long> m_timestamps;
    std::vector<std::string> m_texts;
};
} // Content
} // DeviceAPI

#endif // __TIZEN_AUDIO_CONTENT_LYRICS_H__
