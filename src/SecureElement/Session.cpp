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
#include <Logger.h>
#include <PlatformException.h>
#include <GlobalContextManager.h>

#include "Session.h"
#include "SEUtils.h"
#include "JSChannel.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace smartcard_service_api;
using namespace DeviceAPI::Common;

namespace {
struct ChannelCallbackHolder {
    ChannelCallbackHolder(ChannelCallbackDataPtr callback, Session* session)
    {
        ptr = callback;
        this->session = session;
    }
    ChannelCallbackDataPtr ptr;
    Session* session;
};
}

Session::Session(smartcard_service_api::Session* session)
{
    LOGD("Entered");
    if (NULL == session){
        LOGE("Session is null");
        throw Common::UnknownException("Session is null");
    }
    m_session = session;
}

Session::~Session()
{
    LOGD("Entered");
    close();
    m_session = NULL;
}

void Session::openChannel(ChannelCallbackDataPtr callback)
{
    LOGD("Entered");
    ChannelCallbackHolder* holder = new ChannelCallbackHolder(callback, this);
    pthread_t thread;
    if(pthread_create(&thread, NULL, threadWorker,
            static_cast<void*>(holder))) {
        LOGE("Thread creation failed");
        delete holder;
        throw UnknownException("Thread creation failed");
    }

    if(pthread_detach(thread)) {
        LOGE("Thread detachment failed");
    }
}

void* Session::threadWorker(void* data)
{
    auto holder = static_cast<ChannelCallbackHolder*>(data);
    if (!holder) {
        LOGE("Holder is null");
        return NULL;
    }

    auto callback = holder->ptr;
    if (!callback) {
        LOGE("Callback is null");
        delete holder;
        holder = NULL;
        return NULL;
    }

    auto session = holder->session;
    if(!session){
        LOGE("Session is null");
        delete holder;
        holder = NULL;
        return NULL;
    }

    try {

        if (callback->isBasicChannel()) {
            try {
                Channel* channel = session->m_session->openBasicChannelSync(
                        ByteArray(callback->getAid()->data(), callback->getAid()->size()));
                SEChannelPtr channel_ptr(new SEChannel(channel));
                callback->setChannel(channel_ptr);
            }
            catch (ExceptionBase& e) {
                LOGE("Error thrown, code: %d", e.getErrorCode());
                throwDeducedSecureElementException(e, "channel->openBasicChannelSync()");
            }
        }
        else {
            try {
                Channel* channel = session->m_session->openLogicalChannelSync(
                        ByteArray(callback->getAid()->data(), callback->getAid()->size()));
                SEChannelPtr channel_ptr(new SEChannel(channel));
                callback->setChannel(channel_ptr);
            }
            catch (ExceptionBase& e) {
                LOGE("Error thrown, code: %d", e.getErrorCode());
                throwDeducedSecureElementException(e, "channel->openLogicalChannelSync()");
            }
        }
    }
    catch (const BasePlatformException& e) {
        LOGE("error during execution threadWorker, %s, %s",
                e.getName().c_str(), e.getMessage().c_str());
        callback->setError(e.getName(), e.getMessage());
    }
    catch(...){
        LOGE("error during openChannel");
        callback->setError("UnknownError", "UnknownError");
    }

    guint id = g_idle_add(gidleWorker, data);
    if(!id){
        LOGE("g_idle_add fails");
        delete holder;
    }

    return NULL;
}
gboolean Session::gidleWorker(void *data)
{
    auto holder = static_cast<ChannelCallbackHolder*>(data);
    if (!holder) {
        LOGE("Holder is null");
        return false;
    }

    auto callback = holder->ptr;
    if (!callback) {
        LOGE("Callback is null");
        delete holder;
        holder = NULL;
        return false;
    }

    auto session = holder->session;
    if(!session){
        LOGE("Session is null");
        delete holder;
        holder= NULL;
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete holder;
        holder = NULL;
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
            JSValueRef obj = JSChannel::makeJSObject(context, callback->getChannel());
            callback->callSuccessCallback(obj);
        }
    }
    catch (const BasePlatformException& e) {
        LOGE("error during openChannel: %s, %s",
                e.getName().c_str(), e.getMessage().c_str());
    } catch(...){
        LOGE("error during openChannel");
    }

    delete holder;
    holder = NULL;

    return false;
}

UCharVectorPtr Session::getATR() const
{
    LOGD("Entered");
    ByteArray byte_array;
    UCharVectorPtr to_ret;
    try{
        byte_array = m_session->getATRSync();
        uint8_t* response_buf = byte_array.getBuffer();
        size_t response_len = byte_array.getLength();

        to_ret = UCharVectorPtr(new UCharVector(response_buf,
                response_buf + response_len));
        return to_ret;
    }catch(ExceptionBase& e){
        LOGE("Error during getATR(): code %d , msg %s,", e.getErrorCode(), e.what());
        throw UnknownException(e.what());
    }
    catch (...) {
        LOGE("Error during getATR()");
        throw UnknownException("Error during getATR()");
    }

    return to_ret;
}

void Session::close()
{
    LOGD("Entered");
    try {
        if (!isClosed()) {
            m_session->closeSync();
        }
    }
    catch(ExceptionBase& e){
        LOGE("Error during close(): code %d , msg %s,", e.getErrorCode(), e.what());
        throw UnknownException(e.what());
    }
    catch (...) {
        LOGE("Error during close()");
        throw UnknownException("Error during close()");
    }
}

void Session::closeChannels()
{
    LOGD("Entered");
    try {
        m_session->closeChannels();
    }
    catch (ExceptionBase& e) {
        LOGE("Error during closeChannels(): code %d , msg %s,", e.getErrorCode(), e.what());
        throw UnknownException(e.what());
    }
    catch (...) {
        LOGE("Error during closeChannels()");
        throw UnknownException("Error during closeChannels()");
    }
}

bool Session::isClosed() const
{
    LOGD("Entered");
    return m_session->isClosed();
}

} // SecureElement
} // DeviceAPI
