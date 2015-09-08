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

/**
 * @file        SECallbackData.cpp
 */

#include "SECallbackData.h"

namespace DeviceAPI {
namespace SecureElement {

//========================BASE_CALLBACK_DATA========================

BaseCallbackData::BaseCallbackData(JSContextRef globalCtx):
                Common::CallbackUserData(globalCtx),
                m_is_error(false)
{
}

BaseCallbackData::~BaseCallbackData()
{
}

void BaseCallbackData::setError(const std::string &err_name,
        const std::string &err_message)
{
    m_err_name = err_name;
    m_err_message = err_message;
    m_is_error = true;
}

bool BaseCallbackData::isError() const
{
    return m_is_error;
}

const std::string& BaseCallbackData::getErrorName() const
{
    return m_err_name;
}

const std::string& BaseCallbackData::getErrorMessage() const
{
    return m_err_message;
}

//========================TRANSMIT_CALLBACK_DATA========================

TransmitCallbackData::TransmitCallbackData(JSContextRef globalCtx):
                BaseCallbackData(globalCtx)
{
}

TransmitCallbackData::~TransmitCallbackData()
{
}

void TransmitCallbackData::setCommand(UCharVectorPtr command)
{
    m_command = command;
}

UCharVectorPtr TransmitCallbackData::getCommand() const
{
    return m_command;
}

void TransmitCallbackData::setResponse(UCharVectorPtr response)
{
    m_response = response;
}

UCharVectorPtr TransmitCallbackData::getResponse() const
{
    return m_response;
}

void TransmitCallbackData::setChannel(Channel* channel)
{
    m_channel = channel;
}

Channel* TransmitCallbackData::getChannel() const
{
    return m_channel;
}

//========================CHANNEL_CALLBACK_DATA========================

ChannelCallbackData::ChannelCallbackData(JSContextRef global_ctx,
        UCharVectorPtr aid,
        bool is_basic) :
                BaseCallbackData(global_ctx),
                m_aid(aid),
                m_is_basic(is_basic)
{
}

ChannelCallbackData::~ChannelCallbackData()
{
}

bool ChannelCallbackData::isBasicChannel() const
{
    return m_is_basic;
}

void ChannelCallbackData::setChannel(SEChannelPtr channel)
{
    m_channel = channel;
}

SEChannelPtr ChannelCallbackData::getChannel() const
{
    return m_channel;
}

UCharVectorPtr ChannelCallbackData::getAid() const
{
    return m_aid;
}

//========================OPEN_SESSION_CALLBACK_DATA========================

OpenSessionCallbackData::OpenSessionCallbackData(JSContextRef globalCtx):
                BaseCallbackData(globalCtx)
{
}

OpenSessionCallbackData::~OpenSessionCallbackData()
{
}

void OpenSessionCallbackData::setSession(SessionPtr session)
{
    m_session = session;
}

SessionPtr OpenSessionCallbackData::getSession() const
{
    return m_session;
}

void OpenSessionCallbackData::setReader(Reader* reader)
{
    m_reader = reader;
}

Reader* OpenSessionCallbackData::getReader() const
{
    return m_reader;
}

//========================GET_READERS_CALLBACK_DATA========================

GetReadersCallbackData::GetReadersCallbackData(JSContextRef globalCtx):
                BaseCallbackData(globalCtx)
{
}

GetReadersCallbackData::~GetReadersCallbackData()
{
}

void GetReadersCallbackData::setReaders(ReaderVectorPtr readers)
{
    m_readers = readers;
}

ReaderVectorPtr GetReadersCallbackData::getReaders() const
{
    return m_readers;
}

void GetReadersCallbackData::setService(smartcard_service_api::SEService* service)
{
    m_service = service;
}

smartcard_service_api::SEService* GetReadersCallbackData::getService() const
{
    return m_service;
}

}
}
