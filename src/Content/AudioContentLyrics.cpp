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

#include "AudioContentLyrics.h"

namespace DeviceAPI {
namespace Content {

AudioContentLyrics::AudioContentLyrics():
            m_type("")
{
}

AudioContentLyrics::~AudioContentLyrics()
{
}

std::string AudioContentLyrics::getType() const
{
    return m_type;
}

void AudioContentLyrics::setType(std::string type)
{
    m_type = type;
}

std::vector<unsigned long> AudioContentLyrics::getTimestamps() const
{
    return m_timestamps;
}

void AudioContentLyrics::setTimestamps(std::vector<unsigned long> timestamps)
{
    m_timestamps = timestamps;
}

std::vector<std::string> AudioContentLyrics::getTexts() const
{
    return m_texts;
}

void AudioContentLyrics::setTexts(std::vector<std::string> texts)
{
    m_texts = texts;
}


} // Content
} // DeviceAPI
