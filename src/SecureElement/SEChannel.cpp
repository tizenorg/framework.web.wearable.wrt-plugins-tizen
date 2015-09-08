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

#include "SEChannel.h"

#include <ByteArray.h>
#include <Logger.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include "SEUtils.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace smartcard_service_api;
using namespace Common;

SEChannel::SEChannel(void* channel)
{
    if (!channel) {
        LOGE("Channel is null");
        throw UnknownException("Channel is null");
    }

    m_channel = static_cast<Channel*>(channel);
}

SEChannel::~SEChannel()
{
    try {
        close();
    }
    catch (ExceptionBase& e) {
        LOGE("Exception caught: %d, message: %s", e.getErrorCode(), e.what());
    }
    m_channel = NULL;
}

void SEChannel::close()
{
    if (!m_channel->isClosed()) {
        m_channel->closeSync();
    }
}

bool SEChannel::isBasicChannel() const
{
    return m_channel->isBasicChannel();
}

void SEChannel::transmit(TransmitCallbackData* callback)
{
    callback->setChannel(m_channel);
    pthread_t thread;
    if(pthread_create(&thread, NULL, transmitThread,
            static_cast<void*>(callback))) {
        LOGE("Thread creation failed");
        throw UnknownException("Thread creation failed");
    }
    if(pthread_detach(thread)) {
        LOGE("Thread detachment failed");
    }
}

void* SEChannel::transmitThread(void* data)
{
    auto callback = static_cast<TransmitCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return NULL;
    }

    Channel* channel = callback->getChannel();
    if (!channel) {
        LOGE("chanel is null");
        delete callback;
        callback = NULL;
        return NULL;
    }

    try {
        ByteArray command(callback->getCommand()->data(),
                callback->getCommand()->size());
        ByteArray response;

        try {
            int error = channel->transmitSync(command, response);
            if (error != 0) {
                LOGE("ret : %d", error);
                throwSecureElementException<UnknownException>(error, "channel->transmitSync()");
            }
        }
        catch (ExceptionBase& e) {
            LOGE("Error thrown, code: %d", e.getErrorCode());
            throwDeducedSecureElementException(e, "channel->transmitSync()");
        }

        uint8_t* response_buf = response.getBuffer();
        size_t response_len = response.getLength();

        UCharVectorPtr response_vec(new UCharVector(response_buf,
                response_buf + response_len));

        callback->setResponse(response_vec);
    }
    catch (const BasePlatformException &err) {
        LOGE("transmitThread fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
        callback->setError(err.getName(), err.getMessage());
    }
    catch (...) {
        LOGE("transmitThread fails");
        callback->setError("UnknownError", "transmitThread fails");
    }

    guint id = g_idle_add(transmitCallback, data);
    if (!id) {
        LOGE("g_idle_add fails");
        delete callback;
    }
    return NULL;
}

gboolean SEChannel::transmitCallback(void *data)
{
    auto callback = static_cast<TransmitCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return false;
    }

    auto response = callback->getResponse();
    if (!response) {
        LOGE("response is null");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "response is null");
    }

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }
    try {
        if (callback->isError()) {
            JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(error);
        }
        else {
            JSValueRef arr = JSUtil::toJSValueRef_(context, *callback->getResponse());
            callback->callSuccessCallback(arr);
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("transmitCallback fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("transmitCallback fails");
    }

    delete callback;
    callback = NULL;

    return false;
}

UCharVectorPtr SEChannel::getSelectResponse() const
{
    LOGD("Entered");
    ByteArray byte_array;
    UCharVectorPtr to_ret;
    try{
        byte_array = m_channel->getSelectResponse();
        uint8_t* response_buf = byte_array.getBuffer();
        size_t response_len = byte_array.getLength();

        to_ret = UCharVectorPtr(new UCharVector(response_buf,
                response_buf + response_len));
        return to_ret;
    }catch(ExceptionBase& e){
        LOGE("Error during getSelectResponse(): code %d , msg %s,", e.getErrorCode(), e.what());
        throw UnknownException(e.what());
    }
    catch (...) {
        LOGE("Error during getSelectResponse()");
        throw UnknownException("Error during getSelectResponse()");
    }

    return to_ret;
}

}
}
