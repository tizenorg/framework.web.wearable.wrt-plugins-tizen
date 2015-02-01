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
#include "ContentUtility.h"

#include "AudioLyricsUtil.h"

namespace DeviceAPI {
namespace Content {

AudioLyricsUtil::AudioLyricsUtil() : m_extractor(NULL), m_lyrics(new AudioContentLyrics())
{
}

AudioLyricsUtil::~AudioLyricsUtil()
{
}

AudioContentLyricsPtr AudioLyricsUtil::getAudioLyrics()
{
    return m_lyrics;
}


bool AudioLyricsUtil::fetchLyrics(std::string URI)
{
    LOGD("Entered");
    bool ret = true;
    int err = metadata_extractor_create(&m_extractor);
    if ( METADATA_EXTRACTOR_ERROR_NONE != err)
    {
        LOGE("ret: %d", err);
        ContentUtility::throwMetadataException<UnknownException>(
                err, "metadata_extractor_create()");
    }

    err = metadata_extractor_set_path(m_extractor, URI.c_str());
    if(METADATA_EXTRACTOR_ERROR_NONE == err)
    {
        std::vector<unsigned long> timestamps;
        std::vector<std::string> texts;
        char* strSyncTextNum=NULL;
        metadata_extractor_attr_e attr = METADATA_SYNCLYRICS_NUM;
        err = metadata_extractor_get_metadata(m_extractor, attr, &strSyncTextNum);
        if ( METADATA_EXTRACTOR_ERROR_NONE == err && strSyncTextNum )
        {
            int nSyncTextNum = atoi(strSyncTextNum);
            //istringstream(strSyncTextNum) >> nSyncTextNum;
            free(strSyncTextNum);
            strSyncTextNum = NULL;

            if ( nSyncTextNum > 0)
            {
                m_lyrics->setType("SYNCHRONIZED");
                for(int i=0; i < nSyncTextNum; i++)
                {
                    unsigned long time_info = 0;
                    char * lyrics = NULL;
                    err = metadata_extractor_get_synclyrics(m_extractor, i, &time_info, &lyrics);
                    if ( METADATA_EXTRACTOR_ERROR_NONE == err)
                    {
                        timestamps.push_back(time_info);
                        texts.push_back(lyrics);
                        free(lyrics);
                    } else {
                        LOGW("%s", ContentUtility::getMetadataErrorMessage(
                                err, "metadata_extractor_get_synclyrics()").c_str());
                    }
                }
                ret = true;
            }
            else
            {
                char* unSyncText = NULL;
                attr = METADATA_UNSYNCLYRICS;

                err = metadata_extractor_get_metadata(m_extractor, attr, &unSyncText);
                if ( METADATA_EXTRACTOR_ERROR_NONE == err)
                {
                    if (unSyncText)
                    {
                        m_lyrics->setType("UNSYNCHRONIZED");
                        texts.push_back(unSyncText);

                        free(unSyncText);
                        unSyncText = NULL;
                        ret = true;
                    }
                    else
                    {
                        ret = false;
                    }
                } else {
                    LOGW("%s", ContentUtility::getMetadataErrorMessage(
                            err, "metadata_extractor_get_metadata()").c_str());
                }

            }

            if(ret)
            {
                m_lyrics->setTimestamps(timestamps);
                m_lyrics->setTexts(texts);
            }
        }
        else
        {
            LOGE("%s", ContentUtility::getMetadataErrorMessage(
                    err, "metadata_extractor_get_metadata()").c_str());
            ret = false;
        }

    }
    else
    {
        LOGW("%s", ContentUtility::getMetadataErrorMessage(
                err, "metadata_extractor_set_path()").c_str());
        ret = false;
    }

    if(m_extractor != NULL)
    {
        err = metadata_extractor_destroy(m_extractor);
        if(METADATA_EXTRACTOR_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMetadataErrorMessage(
                    err, "metadata_extractor_destroy()").c_str());
        }
    }
    return ret;
}


} // Content
} // DeviceAPI
