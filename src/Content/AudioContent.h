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

#ifndef __TIZEN_AUDIO_CONTENT_H__
#define __TIZEN_AUDIO_CONTENT_H__

#include <Export.h>
#include <MultiCallbackUserData.h>

#include "ContentTypes.h"
#include "AudioContentLyrics.h"

#include "Content.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class AudioContent;
typedef std::shared_ptr<AudioContent> AudioContentPtr;

class DLL_EXPORT AudioContent : public Content
{
public:
    AudioContent();
    virtual ~AudioContent();

    std::string getAlbum() const;
    void setAlbum(std::string album);

    std::vector<std::string> getGenres() const;
    void setGenres(std::vector<std::string> genres);

    std::vector<std::string> getArtists() const;
    void setArtists(std::vector<std::string> artists);

    std::vector<std::string> getComposers() const;
    void setComposers(std::vector<std::string> composers);

    AudioContentLyricsPtr getLyrics() const;
    void setLyrics(AudioContentLyricsPtr lyrics);

    std::string getCopyright() const;
    void setCopyright(std::string copyright);

    unsigned long getBitrate() const;
    void setBitrate(unsigned long bitrate);

    unsigned short getTrackNumber() const;
    void setTrackNumber(unsigned short trackNumber);

    unsigned long getDuration() const;
    void setDuration(unsigned long duration);

private:
    std::string m_album;
    std::vector<std::string> m_genres;
    std::vector<std::string> m_artists;
    std::vector<std::string> m_composers;
    AudioContentLyricsPtr m_lyrics;
    std::string m_copyright;
    unsigned long m_bitrate;
    unsigned short m_trackNumber;
    unsigned long m_duration;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_AUDIO_CONTENT_H__
