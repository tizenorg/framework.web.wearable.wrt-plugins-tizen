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

#ifndef __TIZEN_SECURE_ELEMENT_SE_SERVICE_H__
#define __TIZEN_SECURE_ELEMENT_SE_SERVICE_H__

#include <memory>
#include <mutex>
#include <vector>
#include <map>
#include <SEService.h>
#include <glib.h>

#include "SEChangeListener.h"
#include "SECallbackData.h"
#include "SEReader.h"

namespace DeviceAPI {
namespace SecureElement {

class SEService;
typedef std::shared_ptr<SEService> SEServicePtr;

class SEService {
public:
    static SEService& getInstance();

    virtual ~SEService();

    void getReaders(GetReadersCallbackData *callback);
    long registerSEListener(SEChangeListenerPtr listener);
    void unregisterSEListener(long id);
    void shutdown();

    void serviceConnected();
    void eventHandler(char *seName, int event);

private:
    SEService();
    SEService(const SEService&);
    SEService& operator=(const SEService&);
    void clearCallbacks();

    long m_listener_counter;

    typedef std::pair<long, SEChangeListenerPtr> ListenerPair;
    typedef std::map<long, SEChangeListenerPtr> ListenersMap;
    ListenersMap m_listeners;

    std::vector<GetReadersCallbackData*> m_callbacks;

    bool m_initialized;
    bool m_shutdown;
    smartcard_service_api::SEService *m_service;

    static void* getReadersThread(void *data);
    static gboolean getReadersCallback(void *data);
    static gboolean changeListenerFunc(void *data);

    struct SEChangeListenerHolder {
        SEReaderPtr reader;
        SEChangeListenerPtr listener;
        int event;
    };
};

} //SecureElement
} //DeviceAPI

#endif //__TIZEN_SECURE_ELEMENT_SE_SERVICE_H__
