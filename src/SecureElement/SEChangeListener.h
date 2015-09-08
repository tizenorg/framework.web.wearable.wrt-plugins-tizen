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

#ifndef __TIZEN_SECURE_ELEMENT_SE_CHANGE_LISTENER_H__
#define __TIZEN_SECURE_ELEMENT_SE_CHANGE_LISTENER_H__

#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>
#include <vector>
#include <memory>
#include <string>

#include "SEReader.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace smartcard_service_api;

class SEChangeListener;

typedef std::shared_ptr<SEChangeListener> SEChangeListenerPtr;
typedef std::shared_ptr<Common::MultiCallbackUserData> MultiCallbackPtr;

class SEChangeListener {
public:
    SEChangeListener(JSContextRef globalCtx, JSObjectRef object);
    virtual ~SEChangeListener();

    void onSEReady(SEReaderPtr reader);
    void onSENotReady(SEReaderPtr reader);

    static const char* ON_SE_READY;
    static const char* ON_SE_NOT_READY;

    bool isActive() const;
    void setActive(bool active);

private:
    MultiCallbackPtr m_callback;
    bool m_is_active;

};

} //SecureElement
} //DeviceAPI

#endif // __TIZEN_SECURE_ELEMENT_SE_CHANGE_LISTENER_H__
