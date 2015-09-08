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

#include <PlatformException.h>
#include <Logger.h>

#include "AudioContent.h"

namespace DeviceAPI {
namespace Content {

AudioContent::AudioContent():
            m_album(""),
            m_copyright(""),
            m_bitrate(0),
            m_duration(0)
{
    m_type = "AUDIO";
}

AudioContent::~AudioContent()
{
    m_genres.clear();
    m_artists.clear();
    m_composers.clear();
}

std::string AudioContent::getAlbum() const
{
    return m_album;
}

void AudioContent::setAlbum(std::string album)
{
    m_album = album;
}

std::vector<std::string> AudioContent::getGenres() const
{
    return m_genres;
}

void AudioContent::setGenres(std::vector<std::string> genres)
{
    m_genres = genres;
}

std::vector<std::string> AudioContent::getArtists() const
{
    return m_artists;
}

void AudioContent::setArtists(std::vector<std::string> artists)
{
    m_artists = artists;
}

std::vector<std::string> AudioContent::getComposers() const
{
    return m_composers;
}

void AudioContent::setComposers(std::vector<std::string> composers)
{
    m_composers = composers;
}

AudioContentLyricsPtr AudioContent::getLyrics() const
{
    return m_lyrics;
}

void AudioContent::setLyrics(AudioContentLyricsPtr lyrics)
{
    m_lyrics = lyrics;
}

std::string AudioContent::getCopyright() const
{
    return m_copyright;
}

void AudioContent::setCopyright(std::string copyright)
{
    m_copyright = copyright;
}

unsigned long AudioContent::getBitrate() const
{
    return m_bitrate;
}

void AudioContent::setBitrate(unsigned long bitrate)
{
    m_bitrate = bitrate;
}

unsigned short AudioContent::getTrackNumber() const
{
    return m_trackNumber;
}

void AudioContent::setTrackNumber(unsigned short trackNumber)
{
    m_trackNumber = trackNumber;
}

unsigned long AudioContent::getDuration() const
{
    return m_duration;
}

void AudioContent::setDuration(unsigned long duration)
{
    m_duration = duration;
}


} // Content
} // DeviceAPI
