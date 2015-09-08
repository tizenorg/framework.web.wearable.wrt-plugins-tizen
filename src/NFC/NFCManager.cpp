//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "NFCManager.h"
#include "nfc.h"
#include <Logger.h>
#include <PlatformException.h>

namespace DeviceAPI {
namespace NFC {

NFCManager::NFCManager():
        SecurityAccessor()
{
    LOGD("Entered");
    initialize();
}

NFCManager::~NFCManager()
{
    LOGD("Entered");
    deinitialize();
}

NFCManager&  NFCManager::getInstance(){
    static NFCManager instance;
    return instance;
}

void NFCManager::initialize()
{
    LOGD("Entered");
    int result = nfc_manager_initialize();
    if (NFC_ERROR_NONE != result) {
        LOGE("Could not initialize NFC Manager: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        m_initialized = false;
    } else {
        m_initialized = true;
    }
}

void NFCManager::deinitialize() {
    if(m_initialized == true)
    {
        if (m_adapter) {
            m_adapter->unsetTagListener();
            m_adapter->unsetPeerListener();
        }
        unsetExclusiveMode();
        int ret = nfc_manager_deinitialize();
        if (NFC_ERROR_NONE == ret) {
            m_initialized = false;
        }
        else {
            LOGE("Could not deinitialize NFC Manager: %d, %s", ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
    }
}

NFCAdapterPtr NFCManager::getDefaultAdapter()
{
    LOGD("Entered");
    if (!m_adapter) {
        m_adapter.reset(new NFCAdapter());
    }
    return m_adapter;
}

void NFCManager::setExclusiveMode(bool mode)
{
    LOGD("Entered");
    if (!m_initialized) {
        initialize();
    }
    int result = nfc_manager_set_system_handler_enable(!mode);
    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to set exclusive mode: %d, %s",
            result, NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Failed to set exclusive mode.");
    }
}

void NFCManager::unsetExclusiveMode() {
    LOGD("Entered");
    if (m_initialized) {
        int ret = nfc_manager_set_system_handler_enable(true);
        if (NFC_ERROR_NONE != ret) {
            LOGE("Failed to unset exclusive mode: %d, %s", ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
    }
}

} // NFC
} // DeviceAPI
