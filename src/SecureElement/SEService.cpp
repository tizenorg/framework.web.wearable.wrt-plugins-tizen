/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SEService.h"
#include <Logger.h>
#include <algorithm>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include "SEReader.h"
#include "JSReader.h"
#include "SEUtils.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace Common;

namespace {
class ServiceListener: public SEServiceListener {
public:
    void serviceConnected(SEServiceHelper *service, void *context)
    {
        LOGE("Service connected");
        if (context) {
            SEService* service = static_cast<SEService*>(context);
            service->serviceConnected();
        }
        else {
            LOGE("Context is null");
        }
    }

    void eventHandler(SEServiceHelper *service, char *seName, int event,
            void *context)
    {
        LOGE("Event handler called, seName: %s, event: %d", seName, event);
        if (context) {
            SEService* service = static_cast<SEService*>(context);
            service->eventHandler(seName, event);
        }
        else {
            LOGE("Context is null");
        }
    }

    void errorHandler(SEServiceHelper *service, int error, void *context)
    {
        LOGE("Error handler called, code: %d", error);
    }
};
static ServiceListener listener;
}

SEService::SEService():
                m_listener_counter(1),
                m_initialized(false)
{
    m_service = new smartcard_service_api::SEService(static_cast<void*>(this),
            &listener);
}

SEService::~SEService()
{
    if (m_service) {
        m_service->shutdownSync();
        delete m_service;
        m_service = NULL;
    }
}

SEService& SEService::getInstance()
{
    static SEService instance;
    return instance;
}

void SEService::serviceConnected()
{
    m_initialized = true;
    for(size_t size = m_callbacks.size(), i = 0; i < size; ++i) {
        GetReadersCallbackData* callback = NULL;
        try {
            callback = m_callbacks[i];
            getReaders(callback);
        }
        catch (const BasePlatformException& e) {
            LOGE("getReaders failed, name: %s, msg: %s", e.getName().c_str(),
                    e.getMessage().c_str());
            delete callback;
            callback = NULL;
        }
    }
    m_callbacks.clear();
}

void SEService::clearCallbacks()
{
    while (!m_callbacks.empty()) {
        GetReadersCallbackData* callback = m_callbacks.back();
        delete callback;
        callback = NULL;
        m_callbacks.pop_back();
    }
}

void SEService::eventHandler(char* seName, int event)
{
    if (!m_listeners.empty()) {
        std::vector<smartcard_service_api::ReaderHelper*> readers =
                m_service->getReaders();

        size_t i = 0;
        for (i = readers.size(); i > 0; i--) {
            if ((strlen(readers[i-1]->getName()) == strlen(seName))
                    && !strncmp(readers[i-1]->getName(), seName, strlen(seName))) {
                if (((event == 1) && readers[i-1]->isSecureElementPresent())
                        ||((event == 2) && !readers[i-1]->isSecureElementPresent()))
                    break;
            }
        }
        if (!i) {
            LOGE("No readers found");
            return;
        }

        for_each(m_listeners.begin(), m_listeners.end(),
                [event, i, &readers](ListenerPair listener) {
            SEChangeListenerHolder* holder = new SEChangeListenerHolder();
            holder->listener = listener.second;
            holder->reader = SEReaderPtr(new SEReader(readers[i-1]));
            holder->event = event;
            if (!g_idle_add(changeListenerFunc, holder)) {
                LOGE("g_idle_add_fails");
                delete holder;
                holder = NULL;
            }
        });
    }
}

gboolean SEService::changeListenerFunc(void *data)
{
    auto hold = static_cast<SEChangeListenerHolder*>(data);
    if (!hold) {
        LOGE("listener holder is null");
        return false;
    }

    std::unique_ptr<SEChangeListenerHolder> holder(hold);

    auto listener = holder->listener;
    if (!listener) {
        LOGE("listener is null");
        return false;
    }

    try {
        switch (holder->event) {
        case 1:
            listener->onSEReady(holder->reader);
            break;
        case 2:
            listener->onSENotReady(holder->reader);
            break;
        default:
            LOGD("Incorrect event");
            break;
        }
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Persons change listener failed: %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    }

    return false;
}

void SEService::getReaders(GetReadersCallbackData *callback)
{
    if (!m_initialized) {
        m_callbacks.push_back(callback);
        return;
    }

    callback->setService(m_service);
    pthread_t thread;
    if(pthread_create(&thread, NULL, getReadersThread,
            static_cast<void*>(callback))) {
        LOGE("Thread creation failed");
        throw UnknownException("Thread creation failed");
    }
    if(pthread_detach(thread)) {
        LOGE("Thread detachment failed");
    }
}

void* SEService::getReadersThread(void *data)
{
    auto callback = static_cast<GetReadersCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return NULL;
    }

    smartcard_service_api::SEService* service = callback->getService();
    if (!service) {
        LOGE("service is null");
        delete callback;
        callback = NULL;
        return NULL;
    }

    try {
        std::vector<ReaderHelper*> readers = service->getReaders();
        ReaderVectorPtr readers_ptr(new ReaderVector());

        size_t size = readers.size();
        for(size_t i = 0; i < size; i++) {
            readers_ptr->push_back(SEReaderPtr(new SEReader(readers[i])));
        }
        callback->setReaders(readers_ptr);
    }
    catch (const BasePlatformException &err) {
        LOGE("getReadersThread fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
        callback->setError(err.getName(), err.getMessage());
    }
    catch (...) {
        LOGE("openSessionThread fails");
        callback->setError("UnknownError", "getReadersThread fails");
    }

    guint id = g_idle_add(getReadersCallback, data);
    if (!id) {
        LOGE("g_idle_add fails");
        delete callback;
        callback = NULL;
    }
    return NULL;
}

gboolean SEService::getReadersCallback(void *data)
{
    auto callback = static_cast<GetReadersCallbackData*>(data);
    if (!callback) {
        LOGE("callback is null");
        return false;
    }

    auto readers = callback->getReaders();
    if (!readers) {
        LOGE("readers is null");
        delete callback;
        callback = NULL;
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
            JSObjectRef arr = ptrVectorToJSObjectArray<JSReader, SEReaderPtr>(
                    *readers, context);
            callback->callSuccessCallback(arr);
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("getReadersCallback fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getReadersCallback fails");
    }

    delete callback;
    callback = NULL;

    return false;
}

long SEService::registerSEListener(SEChangeListenerPtr listener)
{
    long listener_id = m_listener_counter++;

    m_listeners.insert(ListenerPair(listener_id, listener));

    return listener_id;
}

void SEService::unregisterSEListener(long listener_id)
{
    ListenersMap::iterator it = m_listeners.find(listener_id);

    if (it != m_listeners.end()) {
        it->second->setActive(false);
        m_listeners.erase(it);
    }
    else {
        LOGE("No listener with id: %d", listener_id);
        throw InvalidValuesException("No listener with such id");
    }
}

void SEService::shutdown()
{
    if (m_initialized) {
        m_service->shutdownSync();
        clearCallbacks();

        delete m_service;

        // re-generate m_service since it belongs to singletone object
        m_service = new smartcard_service_api::SEService(
                static_cast<void*>(this), &listener);
        m_initialized = false;
    }
}

}
}
