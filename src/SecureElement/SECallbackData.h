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
 * @file        SECallbackData.h
 */

#ifndef __TIZEN_SECUREELEMENT_SECALLBACKDATA_H__
#define __TIZEN_SECUREELEMENT_SECALLBACKDATA_H__

#include <CallbackUserData.h>
#include <string>
#include <memory>
#include <Channel.h>
#include <Reader.h>
#include <SEService.h>
#include <vector>

using namespace smartcard_service_api;

namespace DeviceAPI {
namespace SecureElement {

typedef std::vector<unsigned char> UCharVector;
typedef std::shared_ptr<UCharVector> UCharVectorPtr;

class ChannelCallbackData;
typedef std::shared_ptr<ChannelCallbackData> ChannelCallbackDataPtr;

class BaseCallbackData : public Common::CallbackUserData
{
public:
    BaseCallbackData(JSContextRef globalCtx);
    virtual ~BaseCallbackData();

    void setError(const std::string &err_name,
            const std::string &err_message);
    bool isError() const;
    const std::string& getErrorName() const;
    const std::string& getErrorMessage() const;

private:
    bool m_is_error;
    std::string m_err_name;
    std::string m_err_message;

};

class TransmitCallbackData : public BaseCallbackData
{
public:
    TransmitCallbackData(JSContextRef globalCtx);
    virtual ~TransmitCallbackData();

    void setCommand(UCharVectorPtr command);
    UCharVectorPtr getCommand() const;
    void setResponse(UCharVectorPtr command);
    UCharVectorPtr getResponse() const;
    Channel* getChannel() const;
    void setChannel(Channel* channel);

private:
    UCharVectorPtr m_command;
    UCharVectorPtr m_response;
    Channel* m_channel;
};

class SEChannel;
typedef std::shared_ptr<SEChannel> SEChannelPtr;

class ChannelCallbackData : public BaseCallbackData
{
public:
    ChannelCallbackData(JSContextRef global_ctx,
            UCharVectorPtr aid,
            bool is_basic);
    virtual ~ChannelCallbackData();

    bool isBasicChannel() const;
    SEChannelPtr getChannel() const;
    void setChannel(SEChannelPtr channel);
    UCharVectorPtr getAid() const;

private:
    bool m_is_basic;
    UCharVectorPtr m_aid;
    SEChannelPtr m_channel;
};

class Session;
typedef std::shared_ptr<Session> SessionPtr;

class OpenSessionCallbackData: public BaseCallbackData
{
public:
    OpenSessionCallbackData(JSContextRef globalCtx);
    virtual ~OpenSessionCallbackData();

    void setSession(SessionPtr session);
    SessionPtr getSession() const;
    Reader* getReader() const;
    void setReader(Reader* reader);

private:
    SessionPtr m_session;
    Reader* m_reader;

};

class SEReader;
typedef std::shared_ptr<SEReader> SEReaderPtr;
typedef std::vector<SEReaderPtr> ReaderVector;
typedef std::shared_ptr<ReaderVector> ReaderVectorPtr;

class GetReadersCallbackData: public BaseCallbackData
{
public:
    GetReadersCallbackData(JSContextRef globalCtx);
    virtual ~GetReadersCallbackData();

    void setReaders(ReaderVectorPtr readers);
    ReaderVectorPtr getReaders() const;
    smartcard_service_api::SEService* getService() const;
    void setService(smartcard_service_api::SEService* service);

private:
    ReaderVectorPtr m_readers;
    smartcard_service_api::SEService* m_service;

};

}
}

#endif //__TIZEN_SECUREELEMENT_SECALLBACKDATA_H__
