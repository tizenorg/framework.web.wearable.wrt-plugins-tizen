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

#include "SEReader.h"
#include "JSSession.h"

#include <PlatformException.h>
#include <Logger.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SecureElement {

using namespace smartcard_service_api;
using namespace Common;

SEReader::SEReader(void* reader)
{
    if (!reader) {
        LOGE("Reader is null");
        throw UnknownException("Reader is null");
    }

    m_reader = static_cast<Reader*>(reader);
}

SEReader::~SEReader()
{
    try {
        closeSessions();
    }
    catch (ExceptionBase& e) {
        LOGE("Exception caught: %d, message: %s", e.getErrorCode(), e.what());
    }
}

bool SEReader::isPresent() const
{
    return m_reader->isSecureElementPresent();
}

std::string SEReader::getName() const
{
    return m_reader->getName();
}

void SEReader::openSession(OpenSessionCallbackData* callback)
{
    callback->setReader(m_reader);
    pthread_t thread;
    if(pthread_create(&thread, NULL, openSessionThread,
            static_cast<void*>(callback))) {
        LOGE("Thread creation failed");
        throw UnknownException("Thread creation failed");
    }
    if(pthread_detach(thread)) {
        LOGE("Thread detachment failed");
    }
}

void* SEReader::openSessionThread(void* data)
{
    auto callback = static_cast<OpenSessionCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return NULL;
    }

    Reader* reader = callback->getReader();
    if (!reader) {
        LOGE("reader is null");
        delete callback;
        callback = NULL;
        return NULL;
    }

    try {
        SessionHelper* session = reader->openSessionSync();
        SessionPtr session_ptr(new Session(
                static_cast<smartcard_service_api::Session*>(session)));
        callback->setSession(session_ptr);
    }
    catch (const BasePlatformException &err) {
        LOGE("openSessionThread fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
        callback->setError(err.getName(), err.getMessage());
    }
    catch (...) {
        LOGE("openSessionThread fails");
        callback->setError("UnknownError", "openSessionThread fails");
    }

    guint id = g_idle_add(openSessionCallback, data);
    if (!id) {
        LOGE("g_idle_add fails");
        delete callback;
        callback = NULL;
    }
    return NULL;
}

gboolean SEReader::openSessionCallback(void *data)
{
    auto callback = static_cast<OpenSessionCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return false;
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
            auto session = callback->getSession();
            if (!session) {
                LOGE("session is null");
                delete callback;
                callback = NULL;
                return false;
            }

            JSObjectRef obj = JSSession::makeJSObject(context, session);
            callback->callSuccessCallback(obj);
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("openSessionCallback fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("openSessionCallback fails");
    }

    delete callback;
    callback = NULL;

    return false;
}

void SEReader::closeSessions()
{
    m_reader->closeSessions();
}

}
}
