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

#ifndef __TIZEN_AUDIO_LYRICS_UTIL_H__
#define __TIZEN_AUDIO_LYRICS_UTIL_H__

//#include <IFilter.h>
//#include <SortMode.h>

#include "AudioContentLyrics.h"

#include <metadata_extractor.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class AudioLyricsUtil
{
public:
    AudioLyricsUtil();
    virtual ~AudioLyricsUtil();

    bool fetchLyrics(std::string);

    AudioContentLyricsPtr getAudioLyrics();

private:
    metadata_extractor_h m_extractor;
    AudioContentLyricsPtr m_lyrics;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_AUDIO_LYRICS_UTIL_H__
