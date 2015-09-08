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

#include "SystemInfo.h"
#include "SystemInfoDeviceCapability.h"
#include "SystemInfoProperty.h"
#include "SystemInfoUtil.h"
#include "JSSystemInfoBattery.h"
#include "JSSystemInfoLocale.h"
#include "JSSystemInfoStorage.h"
#include "JSSystemInfoDisplay.h"
#include "JSSystemInfoDeviceOrientation.h"
#include "JSSystemInfoCpu.h"
#include "JSSystemInfoBuild.h"
#include "JSSystemInfoPeripheral.h"
#include "JSSystemInfoNetwork.h"
#include "JSSystemInfoMemory.h"

#ifdef FEATURE_OPTIONAL_WI_FI
#include "JSSystemInfoWifiNetwork.h"
#endif

#include "JSSystemInfoCellularNetwork.h"
#include "JSSystemInfoSIM.h"

#include <algorithm>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>
#include <Logger.h>
#include <GlobalContextManager.h>
#include <device.h>

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const double PROPERTY_WATCHER_TIME = 1;
const int MEMORY_TO_BYTE = 1024;
const int BASE_GATHERING_INTERVAL = 100;

}

CpuInfo SystemInfo::m_cpu_info;

std::mutex SystemInfo::m_cpu_info_lock;

using namespace DeviceAPI::Common;

SystemInfo::SystemInfo() :
                m_storage_timer(NULL),
                m_cpu_timer(NULL),
                m_cpu_last_load(0),
                m_connection_handle(NULL),
                m_current_id(0),
                m_sensor_handle(NULL),
                m_is_telephony_enable(false)
{
    LOGD("Entered");

    // add codes to check device's capability for telephony
    SystemInfoDeviceCapabilityPtr deviceCapability = getCapabilities();
    m_is_telephony_enable = deviceCapability->isTelephony();

    int i = 0;
    for (i = 0; i <= TAPI_HANDLE_MAX; i++) {
        m_tapi_handle[i] = NULL;
    }

    // create thread pool with max threads = 1 to make API calls async but
    // only one call at time
    m_pool = g_thread_pool_new(getPropertyValueThread, NULL, 1, true, NULL);
}

SystemInfo::~SystemInfo()
{
    LOGD("Entered");

    std::for_each(m_watcher_map.begin(), m_watcher_map.end(),
            [] (const std::pair<unsigned long, Ecore_Timer*>& watcher) {
                Ecore_Timer* timer = watcher.second;
                if (timer) {
                    ecore_timer_del(timer);
                    timer = NULL;
                }
            });

    if (!m_battery_callbacks.empty()) {
        unregisterVconfCallback(VCONFKEY_SYSMAN_BATTERY_CAPACITY, SystemInfo::onBatteryChanged);
        unregisterVconfCallback(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, SystemInfo::onBatteryChanged);
    }

    if (!m_peripheral_callbacks.empty()) {
        unregisterVconfCallback(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS,
                SystemInfo::onPeripheralChanged);
        unregisterVconfCallback(VCONFKEY_SYSMAN_HDMI, SystemInfo::onPeripheralChanged);
        unregisterVconfCallback(VCONFKEY_POPSYNC_ACTIVATED_KEY, SystemInfo::onPeripheralChanged);
    }

    if (!m_device_orientation_callbacks.empty()) {
        unregisterVconfCallback(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL,
                SystemInfo::onDeviceAutoRotationChanged);
        if (m_sensor_handle) {
            sensord_unregister_event(m_sensor_handle, AUTO_ROTATION_EVENT_CHANGE_STATE);
            SystemInfoDeviceOrientation::disconnectSensor(m_sensor_handle);
            m_sensor_handle = NULL;
        }
    }

    if (!m_network_callbacks.empty()) {
        LOGD("the native callback for NETWORK listeners will be removed.");
        unregisterNetworkTypeChangeCallback();
    }

    if (m_is_telephony_enable) {
        if (!m_cellular_network_callbacks.empty() || !m_wifi_network_callbacks.empty()) {
            //wifi and cellular has common ip callback
            unregisterIpChangeCallback();
        }

        if (!m_cellular_network_callbacks.empty()) {
            unregisterVconfCallback(VCONFKEY_TELEPHONY_FLIGHT_MODE, onConnectionNetworkParamChanged);
            unregisterVconfCallback(VCONFKEY_TELEPHONY_CELLID, onConnectionNetworkParamChanged);
            unregisterVconfCallback(VCONFKEY_TELEPHONY_LAC, onConnectionNetworkParamChanged);
            unregisterVconfCallback(VCONFKEY_TELEPHONY_SVC_ROAM, onConnectionNetworkParamChanged);
        }
    }

    if (!m_locale_callbacks.empty()) {
        if (SYSTEM_SETTINGS_ERROR_NONE
                != system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE)) {
            LOGE("unregistration of language change callback failed");
        }
        if (SYSTEM_SETTINGS_ERROR_NONE
                != system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY)) {
            LOGE("unregistration of country change callback failed");
        }
    }

    if (!m_storage_callbacks.empty()) {
        unregisterVconfCallback(VCONFKEY_SYSMAN_MMC_STATUS, SystemInfo::mmcStatusChanged);
    }

    if (!m_memory_callbacks.empty()) {
        unregisterVconfCallback(VCONFKEY_SYSMAN_LOW_MEMORY, SystemInfo::onMemoryChanged);
    }

    stopAndDestroyTimer(&m_cpu_timer, std::string("Cpu"));
    stopAndDestroyTimer(&m_storage_timer, std::string("Storage"));

    //finish only current task and wait for thread to stop
    g_thread_pool_free(m_pool, true, true);

    if (m_is_telephony_enable) {
        unsigned int i = 0;
        while (m_tapi_handle[i]) {
            tel_deinit(m_tapi_handle[i]);
            i++;
        }
    }

    for (auto it = m_processing_properties.begin(); it != m_processing_properties.end(); ++it) {
        delete *it;
    }
    m_processing_properties.clear();

    if (NULL != m_connection_handle) {
        connection_destroy(m_connection_handle);
    }
}

SystemInfo& SystemInfo::getInstance()
{
    static SystemInfo instance;
    return instance;
}

long long SystemInfo::getTotalMemory()
{
    LOGD("Entered");

    unsigned int value = 0;

    int ret = device_memory_get_total(&value);
    if (ret != DEVICE_ERROR_NONE) {
        std::string log_msg = "Failed to get total memory: ";
        LOGE("%s %d", log_msg.c_str(), ret);
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    return static_cast<long long>(value * MEMORY_TO_BYTE);
}

long long SystemInfo::getAvailableMemory()
{
    LOGD("Entered");

    unsigned int value = 0;

    int ret = device_memory_get_available(&value);
    if (ret != DEVICE_ERROR_NONE) {
        std::string log_msg = "Failed to get total memory: ";
        LOGE("%s %d", log_msg.c_str(), ret);
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    return static_cast<long long>(value * MEMORY_TO_BYTE);
}

SystemInfoDeviceCapabilityPtr SystemInfo::getCapabilities()
{
    SystemInfoDeviceCapabilityPtr capability(new SystemInfoDeviceCapability());
    return capability;
}

unsigned long SystemInfo::getCount(SystemInfoPropertyId propertyId)
{
    LOGD("Entered");

    unsigned long count = 0;

    switch (propertyId) {
        case SystemInfoPropertyId::BATTERY:
        case SystemInfoPropertyId::CPU:
        case SystemInfoPropertyId::STORAGE:
        case SystemInfoPropertyId::DISPLAY:
        case SystemInfoPropertyId::DEVICE_ORIENTATION:
        case SystemInfoPropertyId::BUILD:
        case SystemInfoPropertyId::LOCALE:
        case SystemInfoPropertyId::NETWORK:
        case SystemInfoPropertyId::PERIPHERAL:
        case SystemInfoPropertyId::MEMORY:
            count = DEFAULT_PROPERTY_COUNT;
            break;
#ifdef FEATURE_OPTIONAL_WI_FI
        case SystemInfoPropertyId::WIFI_NETWORK:
            count = DEFAULT_PROPERTY_COUNT;
            break;
#endif
        case SystemInfoPropertyId::CELLULAR_NETWORK:
            if (m_is_telephony_enable) {
                count = DEFAULT_PROPERTY_COUNT;
                break;
            }
        case SystemInfoPropertyId::SIM:
            if (m_is_telephony_enable) {
                count = TAPI_HANDLE_MAX;
                break;
            }
        default:
            std::string log_msg = "Not supported property: " + std::to_string(propertyId);
    }
    return count;
}

gboolean SystemInfo::getPropertyValueCallback(void *data)
{
    LOGD("Entered");

    auto holder = static_cast<PropertyCallbackDataHolder*>(data);
    if (!holder) {
        LOGE("callback holder is null");
        return false;
    }
    auto callback = holder->ptr;
    if (!callback) {
        LOGE("callback is null");
        delete holder;
        holder = NULL;
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
        JSObjectRef js_property = NULL;
        JSObjectRef js_array = JSCreateArrayObject(context, 0, NULL);
        JSObjectRef js_result = NULL;
        bool isArrayCallback = callback->isArrayCallback();

        if (js_array == NULL) {
            std::string log_msg = "Failed to create JSPropertyValueArray";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException(0, log_msg);
        }

        std::vector<SystemInfoPropertyHolder*> properties = callback->getPropertyArray();
        LOGD("%d objects exist", properties.size());
        LOGD("%d property", callback->getPropertyId());

        try {
            for (size_t i = 0; i < properties.size(); i++) {
                SystemInfoPropertyPtr property = properties[i]->ptr;
                LOGD("Entered");
                if (property == NULL) {
                    std::string log_msg = "property is null";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }

                switch (callback->getPropertyId()) {
                    case SystemInfoPropertyId::BATTERY:
                        js_property = JSSystemInfoBattery::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::CPU:
                        js_property = JSSystemInfoCpu::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::STORAGE:
                        js_property = JSSystemInfoStorage::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::DISPLAY:
                        js_property = JSSystemInfoDisplay::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::DEVICE_ORIENTATION:
                        js_property = JSSystemInfoDeviceOrientation::makeJSObject(context,
                                property);
                        break;
                    case SystemInfoPropertyId::BUILD:
                        js_property = JSSystemInfoBuild::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::LOCALE:
                        js_property = JSSystemInfoLocale::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::NETWORK:
                        js_property = JSSystemInfoNetwork::makeJSObject(context, property);
                        break;
#ifdef FEATURE_OPTIONAL_WI_FI
                    case SystemInfoPropertyId::WIFI_NETWORK:
                        js_property = JSSystemInfoWifiNetwork::makeJSObject(context, property);
                        break;
#endif
                    case SystemInfoPropertyId::CELLULAR_NETWORK: {
                        if (SystemInfo::getInstance().m_is_telephony_enable) {
                            js_property = JSSystemInfoCellularNetwork::makeJSObject(context, property);
                            break;
                        } else {
                            std::string log_msg = "Not supported property";
                            LOGE("%s", log_msg.c_str());
                            SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
                        }
                    }
                    case SystemInfoPropertyId::SIM: {
                        if (SystemInfo::getInstance().m_is_telephony_enable) {
                            js_property = JSSystemInfoSIM::makeJSObject(context, property);
                            break;
                        } else {
                            std::string log_msg = "Not supported property";
                            LOGE("%s", log_msg.c_str());
                            SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
                        }
                    }
                    case SystemInfoPropertyId::PERIPHERAL:
                        js_property = JSSystemInfoPeripheral::makeJSObject(context, property);
                        break;
                    case SystemInfoPropertyId::MEMORY:
                        js_property = JSSystemInfoMemory::makeJSObject(context, property);
                        break;
                    default:
                        std::string log_msg = "Not supported property";
                        LOGE("%s", log_msg.c_str());
                        SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
                }

                if (!isArrayCallback) {
                    break;
                }

                if (!JSSetArrayElement(context, js_array, i, js_property)) {
                    std::string log_msg = "Failed to add property into array";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
            }

            if (isArrayCallback) {
                js_result = js_array;
            } else {
                js_result = js_property;
            }

        } catch (const BasePlatformException &err) {
            LOGE("getPropertyValueCallback fails, %s: %s", err.getName().c_str(),
                    err.getMessage().c_str());
            JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(context, err);
            callback->callErrorCallback(error);
        } catch (...) {
            LOGE("getPropertyValueCallback fails");
            JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(context,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "UnknownError");
            callback->callErrorCallback(error);
        }

        if (js_result != NULL) {
            callback->callSuccessCallback(js_result);
        } else {
            LOGE("Invalid callback data");
            JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(context,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "property object creation failed");
            callback->callErrorCallback(error);
        }
    } catch (const BasePlatformException &err) {
        LOGE("getPropertyValueCallback fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("getPropertyValueCallback fails");
    }

    delete holder;
    holder = NULL;

    return false;
}

void SystemInfo::notifyGetPropertyValueReady()
{
    LOGD("Entered");
    std::lock_guard < std::mutex > lock(m_processing_properties_lock);
    for (auto it = m_processing_properties.begin(); it != m_processing_properties.end(); ++it) {
        if ((*it)->ptr->isPropertyReady()) {
            if (!g_idle_add(getPropertyValueCallback, *it)) {
                LOGE("g_idle_add fails");
                delete *it;
            }
            m_processing_properties.erase(it);
            break;
        }
    }
}

void SystemInfo::addProcessingProperty(PropertyCallbackDataHolder* holder)
{
    std::lock_guard < std::mutex > lock(m_processing_properties_lock);
    m_processing_properties.push_back(holder);
}

#ifdef FEATURE_OPTIONAL_WI_FI
void SystemInfo::registerIpChangeCallback()
{
    LOGD("Registering connection callback");
    connection_h handle = getConnectionHandle();
    int ret = connection_set_ip_address_changed_cb(handle, onConnectionIpAddressChanged, NULL);
    if (CONNECTION_ERROR_NONE != ret) {
        std::string log_msg = "Failed to register ip change callback";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        SystemInfoUtil::throwConnectionException(ret, log_msg);
    }
}

void SystemInfo::unregisterIpChangeCallback()
{
    LOGD("Unregistering connection callback");
    connection_h handle = getConnectionHandle();

    int ret = connection_unset_ip_address_changed_cb(handle);
    if (CONNECTION_ERROR_NONE != ret) {
        std::string log_msg = "Failed to unregister ip change callback";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        SystemInfoUtil::throwConnectionException(ret, log_msg);
    }
}
#else
void SystemInfo::registerIpChangeCallback()
{
    LOGD("Registering connection callback");
    //Not supported
}

void SystemInfo::unregisterIpChangeCallback()
{
    LOGD("Unregistering connection callback");
    //Not supported
}
#endif
void SystemInfo::registerNetworkTypeChangeCallback()
{
    LOGD("Registering connection callback");
    connection_h handle = getConnectionHandle();
    int ret = connection_set_type_changed_cb(handle, onConnectionNetworkTypeChanged, NULL);
    if (ret != CONNECTION_ERROR_NONE) {
        std::string log_msg = "Failed to register network type change callback";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        SystemInfoUtil::throwConnectionException(ret, log_msg);
    }
}

void SystemInfo::unregisterNetworkTypeChangeCallback()
{
    LOGD("Unregistering connection callback");
    connection_h handle = getConnectionHandle();
    int ret = connection_unset_type_changed_cb(handle);
    if (ret != CONNECTION_ERROR_NONE) {
        std::string log_msg = "Failed to unregister network type change callback";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        SystemInfoUtil::throwConnectionException(ret, log_msg);
    }
}

connection_h SystemInfo::getConnectionHandle()
{
    if (NULL == m_connection_handle) {
        int ret = connection_create(&m_connection_handle);
        if (CONNECTION_ERROR_NONE != ret) {
            std::string log_msg = "Failed to create connection";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }
    }
    return m_connection_handle;
}

void SystemInfo::onConnectionIpAddressChanged(const char* /*ipv4_address*/,
        const char* /*ipv6_address*/, void* /*user_data*/)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastCellularNetworkChanged();
    SystemInfo::getInstance().broadcastWifiNetworkChanged();
}

void SystemInfo::onConnectionNetworkParamChanged(keynode_t *node, void *user_data)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastCellularNetworkChanged();
}

void SystemInfo::onConnectionNetworkTypeChanged(connection_type_e type, void *user_data)
{
    LOGD("Enterd");
    SystemInfo::getInstance().broadcastNetworkTypeChanged();
}

void SystemInfo::broadcastCellularNetworkChanged()
{
    LOGD("Enterd");

    if (!m_is_telephony_enable) {
        LOGD("\"http://tizen.org/feature/network.telephony\" is not supported");
        return;
    }

    try {
        SystemInfoPropertyPtr property(new SystemInfoCellularNetwork());

        PVCLmap::iterator itr = m_cellular_network_callbacks.begin();

        while (itr != m_cellular_network_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(property);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add fails");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastCellularNetworkChanged fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastCellularNetworkChanged fails");
    }
}

#ifdef FEATURE_OPTIONAL_WI_FI
void SystemInfo::broadcastWifiNetworkChanged()
{
    try {
        SystemInfoPropertyPtr property(new SystemInfoWifiNetwork());

        PVCLmap::iterator itr = m_wifi_network_callbacks.begin();

        while (itr != m_wifi_network_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(property);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add failed");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastWifiNetworkChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastWifiNetworkChanged failed");
    }
}
#else
void SystemInfo::broadcastWifiNetworkChanged()
{
    LOGD("Entered");
    //Not supported
}
#endif
void SystemInfo::broadcastNetworkTypeChanged()
{
    LOGD("Entered");
    try {
        SystemInfoPropertyPtr property(new SystemInfoNetwork());

        PVCLmap::iterator itr = m_network_callbacks.begin();

        while (itr != m_network_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(property);

            PropertyCallbackDataHolder* holder = new(std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add failed");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastWifiNetworkChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastWifiNetworkChanged failed");
    }
}

void SystemInfo::getPropertyValueThread(gpointer data, gpointer user_data)
{
    LOGD("Entered");

    auto holder = static_cast<PropertyCallbackDataHolder*>(data);
    if (!holder) {
        LOGE("callback holder is null");
        return;
    }
    auto callback = holder->ptr;
    if (!callback) {
        LOGE("callback is null");
        delete holder;
        holder = NULL;
        return;
    }

    bool add_to_idle = false;
    int modemNum = 0;

    try {
        SystemInfoPropertyPtr property;

        switch (callback->getPropertyId()) {
            case SystemInfoPropertyId::BATTERY:
                property.reset(new SystemInfoBattery());
                break;
            case SystemInfoPropertyId::CPU:
                property = createSystemInfoCpu();
                break;
            case SystemInfoPropertyId::STORAGE:
                property.reset(new SystemInfoStorage());
                break;
            case SystemInfoPropertyId::DISPLAY:
                property.reset(new SystemInfoDisplay());
                break;
            case SystemInfoPropertyId::DEVICE_ORIENTATION:
                property.reset(new SystemInfoDeviceOrientation());
                break;
            case SystemInfoPropertyId::BUILD:
                property.reset(new SystemInfoBuild());
                break;
            case SystemInfoPropertyId::LOCALE:
                property.reset(new SystemInfoLocale());
                break;
            case SystemInfoPropertyId::NETWORK:
                property.reset(new SystemInfoNetwork());
                break;
#ifdef FEATURE_OPTIONAL_WI_FI
            case SystemInfoPropertyId::WIFI_NETWORK:
                property.reset(new SystemInfoWifiNetwork());
                break;
#endif
            case SystemInfoPropertyId::CELLULAR_NETWORK: {
                if (SystemInfo::getInstance().m_is_telephony_enable) {
                    property.reset(new SystemInfoCellularNetwork());
                    break;
                }
                std::string log_msg = "Not supported property";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
            }
            case SystemInfoPropertyId::SIM: {
                if (SystemInfo::getInstance().m_is_telephony_enable) {
                    if (!(getInstance().m_tapi_handle[0])) {
                        getInstance().setTapiHandles();
                    }
                    modemNum = 0;
                    callback->clearArray();
                    while (modemNum < TAPI_HANDLE_MAX) {
                        property.reset(new SystemInfoSIM(getInstance().m_tapi_handle[modemNum]));
                        if (property) {
                            SystemInfoPropertyHolder *propertyHolder =
                                    new (std::nothrow) SystemInfoPropertyHolder();
                            propertyHolder->ptr = property;
                            callback->addNativeProperty(propertyHolder);
                        } else {
                            std::string log_msg = "Failed to create property object";
                            LOGE("%s", log_msg.c_str());
                            SystemInfoUtil::throwSystemInfoException(0, log_msg);
                        }
                        modemNum++;
                    }
                    add_to_idle = callback->isPropertyReady();
                    break;
                }
                std::string log_msg = "Not supported property";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
            }
            case SystemInfoPropertyId::PERIPHERAL:
                property.reset(new SystemInfoPeripheral());
                break;
            case SystemInfoPropertyId::MEMORY:
                property.reset(new SystemInfoMemory());
                break;
            default:
                std::string log_msg = "Not supported property";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
        }
        if (callback->isArrayEmpty()) {
            if (property) {
                callback->setNativeProperty(property);
                add_to_idle = callback->isPropertyReady();
            } else {
                std::string log_msg = "Failed to create property object";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
        }
    } catch (const BasePlatformException &err) {
        LOGE("getPropertyValueThread fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
        callback->setError(err.getName(), err.getMessage());
        add_to_idle = true;
    } catch (...) {
        LOGE("getPropertyValueThread fails");
        callback->setError("UnknownError", "getPropertyValueThread fails");
        add_to_idle = true;
    }

    if (add_to_idle) {
        if (callback->isError()) {
            if (!g_idle_add(complete, data)) {
                LOGE("g_idle_add fails");
                delete holder;
                holder = NULL;
            }
        } else {
            if (!g_idle_add(getPropertyValueCallback, data)) {
                LOGE("g_idle_add fails");
                delete holder;
                holder = NULL;
            }
        }
    } else {
        LOGD("Property is being processed, not calling js callback");
        getInstance().addProcessingProperty(holder);
    }
    return;
}

void SystemInfo::getPropertyValue(PropertyCallbackDataPtr callback)
{
    LOGD("Entered");
    if (!callback) {
        std::string log_msg = "callback is null";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
    PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
    if (!holder) {
        std::string log_msg = "Failed to allocate memory";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
    holder->ptr = callback;

    if (!g_thread_pool_push(m_pool, static_cast<gpointer>(holder), NULL)) {
        delete holder;
        holder = NULL;
        std::string log_msg = "Failed to add task to thread pool";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
}

unsigned long SystemInfo::addPropertyValueChangeListener(PropertyCallbackDataPtr callback)
{
    LOGD("Entered");

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        return 0;
    }

    //Register the successCallback to receive system events
    //that the status of the requested properties may have changed.
    //If property value is 'BUILD', listener would not be registered
    unsigned long new_id = ++m_current_id;
    std::pair<unsigned long, PropertyCallbackDataPtr> property_cb(new_id, callback);

    // Check type of property for which listener should be registered
    switch (callback->getPropertyId()) {
        case SystemInfoPropertyId::BATTERY: {
            m_battery_callbacks.insert(property_cb);
            if (m_battery_callbacks.size() == 1) {
                registerVconfCallback(VCONFKEY_SYSMAN_BATTERY_CAPACITY,
                        SystemInfo::onBatteryChanged);
                registerVconfCallback(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW,
                        SystemInfo::onBatteryChanged);
            }
            LOGD("Added callback for BATTERY, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::CPU: {
            m_cpu_callbacks.insert(property_cb);
            if (m_cpu_callbacks.size() == 1) {
                if (!m_cpu_timer) {
                    LOGD("Creating timer for Cpu listener");
                    m_cpu_timer = ecore_timer_add(PROPERTY_WATCHER_TIME, SystemInfo::onCpuChanged,
                    NULL);
                } else {
                    LOGE("Timer not created");
                }
            }
            LOGD("Added callback for CPU, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::STORAGE: {
            m_storage_callbacks.insert(property_cb);
            LOGD("m_storage_callbacks.size() = %d", m_storage_callbacks.size());
            if (m_storage_callbacks.size() == 1) {
                registerVconfCallback(VCONFKEY_SYSMAN_MMC_STATUS, SystemInfo::mmcStatusChanged);
                if (!m_storage_timer) {
                    LOGD("Creating timer for Storage listener");
                    m_storage_timer = ecore_timer_add(PROPERTY_WATCHER_TIME,
                            SystemInfo::onStorageChanged, NULL);
                } else {
                    LOGE("Timer not created");
                }
            }
            LOGD("Added callback for STORAGE, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::DISPLAY: {
            m_display_callbacks.insert(property_cb);
            if (m_display_callbacks.size() == 1) {
                registerVconfCallback(VCONFKEY_SETAPPL_LCD_BRIGHTNESS,
                        SystemInfo::onDisplayChanged);
            }
            LOGD("Added callback for DISPLAY, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::DEVICE_ORIENTATION: {
            m_device_orientation_callbacks.insert(property_cb);
            if (m_device_orientation_callbacks.size() == 1) {
                registerVconfCallback(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL,
                        SystemInfo::onDeviceAutoRotationChanged);
                unsigned int interval = BASE_GATHERING_INTERVAL;
                if (!m_sensor_handle) {
                    m_sensor_handle = SystemInfoDeviceOrientation::connectSensor();
                }
                int ret = sensord_register_event(m_sensor_handle, AUTO_ROTATION_EVENT_CHANGE_STATE,
                        interval, 0, SystemInfo::onDeviceOrientationChanged, NULL);
                if (0 != ret) {
                    LOGE("Failed to register auto rotation event listener");
                } else {
                    LOGD("sensord_unregister_event succeed to gather data");
                }
            }
            LOGD("Added callback for DEVICE_ORIENTATION, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::BUILD: {
            LOGD("BUILD property's value is a fixed value");
            m_inactive_callbacks.insert(property_cb);
            break;
        }
        case SystemInfoPropertyId::LOCALE: {
            m_locale_callbacks.insert(property_cb);
            if (m_locale_callbacks.size() == 1) {
                if (SYSTEM_SETTINGS_ERROR_NONE
                        != system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY,
                                SystemInfo::onLocaleChanged, NULL)) {
                    std::string log_msg = "Failed to register country change callback";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
                if (SYSTEM_SETTINGS_ERROR_NONE
                        != system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE,
                                SystemInfo::onLocaleChanged, NULL)) {
                    std::string log_msg = "Failed to register language change callback";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
            }
            LOGD("Added callback for LOCALE, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::NETWORK: {
            connection_h connection;
            int ret = connection_create(&connection);
            if (ret == CONNECTION_ERROR_NONE) {
                LOGD("it support network connection.");
                m_network_callbacks.insert(property_cb);
                connection_destroy(connection);
                if (m_network_callbacks.size() == 1) {
                    registerNetworkTypeChangeCallback();
                }

            } else if(ret == CONNECTION_ERROR_NOT_SUPPORTED) {
                LOGD("it not support network connection.");
                m_inactive_callbacks.insert(property_cb);
            } else {
                std::string log_msg = "Failed to register network type change callback";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }

            LOGD("Added callback for NETWORK, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::WIFI_NETWORK: {
#ifdef FEATURE_OPTIONAL_WI_FI
            m_wifi_network_callbacks.insert(property_cb);
            LOGD("Added callback for WIFI_NETWORK, id: %d", new_id);
            //register callback if there were no callbacks in cellular and
            //this was new callback for wifi
            if (0 == m_cellular_network_callbacks.size() && 1 == m_wifi_network_callbacks.size()) {
                registerIpChangeCallback();
            }
#else
            LOGE("WIFI_NETWORK is not supported");
            m_inactive_callbacks.insert(property_cb);
            callback->setError(JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR, "Not supported property");

#endif
            break;
        }
        case SystemInfoPropertyId::CELLULAR_NETWORK: {
            if (m_is_telephony_enable) {
                m_cellular_network_callbacks.insert(property_cb);
                LOGD("Added callback for CELLULAR_NETWORK, id: %d", new_id);
                if (1 == m_cellular_network_callbacks.size()) {
                    if (0 == m_wifi_network_callbacks.size()) {
                        //register callback if there were no callbacks in wifi and
                        //this was new callback for cellular
                        registerIpChangeCallback();
                    }
                    registerVconfCallback(VCONFKEY_TELEPHONY_FLIGHT_MODE,
                            onConnectionNetworkParamChanged);
                    registerVconfCallback(VCONFKEY_TELEPHONY_CELLID, onConnectionNetworkParamChanged);
                    registerVconfCallback(VCONFKEY_TELEPHONY_LAC, onConnectionNetworkParamChanged);
                    registerVconfCallback(VCONFKEY_TELEPHONY_SVC_ROAM, onConnectionNetworkParamChanged);
                }
            } else {
                LOGE("CELLULAR_NETWORK is not supported");
                m_inactive_callbacks.insert(property_cb);
                callback->setError(JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR, "Not supported property");
            }
            break;
        }
        case SystemInfoPropertyId::SIM: {
            if (m_is_telephony_enable) {
                //SIM listeners are not supported by core API, so we just
                //store callback
                m_sim_callbacks.insert(property_cb);
                LOGD("Added callback for SIM, id: %d", new_id);
            } else {
                LOGE("SIM is not supported");
                m_inactive_callbacks.insert(property_cb);
                callback->setError(JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR, "Not supported property");
            }
            break;
        }
        case SystemInfoPropertyId::PERIPHERAL: {
            m_peripheral_callbacks.insert(property_cb);
            if (m_peripheral_callbacks.size() == 1) {
                /*check if the key exists.*/
                int value = 0;
                if (vconf_get_int(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS, &value) != -1) {
                    registerVconfCallback(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS,
                            SystemInfo::onPeripheralChanged);
                }
                if (vconf_get_int(VCONFKEY_SYSMAN_HDMI, &value) != -1) {
                    registerVconfCallback(VCONFKEY_SYSMAN_HDMI, SystemInfo::onPeripheralChanged);
                }
                if (vconf_get_int(VCONFKEY_POPSYNC_ACTIVATED_KEY, &value) != -1) {
                    registerVconfCallback(VCONFKEY_POPSYNC_ACTIVATED_KEY,
                            SystemInfo::onPeripheralChanged);
                }
            }
            LOGD("Added callback for PERIPHERAL, id: %d", new_id);
            break;
        }
        case SystemInfoPropertyId::MEMORY: {
            m_memory_callbacks.insert(property_cb);
            if (m_memory_callbacks.size() == 1) {
                /*check if the key exists.*/
                int value = 0;
                if (vconf_get_int(VCONFKEY_SYSMAN_LOW_MEMORY, &value) != -1) {
                    registerVconfCallback(VCONFKEY_SYSMAN_LOW_MEMORY, SystemInfo::onMemoryChanged);
                }
            }
            LOGD("Added callback for MEMORY, id: %d", new_id);
            break;
        }
        default:
            std::string log_msg = "Not supported property";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<NotSupportedException>(0, log_msg);
    }
    startPropertyWatcherTimer(new_id, callback->getOptions().timeout);

    PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
    if (!holder) {
        std::string log_msg = "Failed to allocate memory";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
    holder->ptr = callback;

    if (!g_idle_add(complete, holder)) {
        LOGE("g_idle addition failed");
    }

    return new_id;
}

Eina_Bool SystemInfo::onCpuChanged(void* _event_ptr)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastCpuChanged();
    return ECORE_CALLBACK_RENEW;
}

void SystemInfo::broadcastCpuChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_cpu_callbacks.begin();

        SystemInfoCpuPtr cpu = createSystemInfoCpu();
        double load = cpu->getLoad();
        if (load == m_cpu_last_load)
            return;

        m_cpu_last_load = load;

        while (itr != m_cpu_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;

            SystemInfoOptions opt = callback->getOptions();
            if (((opt.high_threshold == 0) && (opt.low_threshold == 0))
                    || ((opt.high_threshold > 0) && (load >= opt.high_threshold))
                    || ((opt.low_threshold > 0) && (load <= opt.low_threshold))) {
                callback->setNativeProperty(cpu);

                PropertyCallbackDataHolder* holder =
                        new (std::nothrow) PropertyCallbackDataHolder();
                if (!holder) {
                    std::string log_msg = "Failed to allocate memory";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
                holder->ptr = callback;

                guint id = g_idle_add(getPropertyValueCallback, holder);
                if (!id) {
                    LOGE("g_idle_add fails");
                    delete holder;
                }
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("getCpuValue fails, %s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("getCpuValue fails");
    }
}

void SystemInfo::removePropertyValueChangeListener(JSContextRef context, unsigned long id)
{
    LOGD("Entered");

    JSContextRef g_ctx = NULL;
    bool on_timeout = false;
    if (context != NULL) {
        g_ctx = Common::GlobalContextManager::getInstance()->getGlobalContext(context);
    } else {
        //it is call from onPropertyWatcherTimeout, context will not be checked
        on_timeout = true;
    }

    if (m_battery_callbacks.find(id) != m_battery_callbacks.end()) {
        if (on_timeout || m_battery_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_battery_callbacks[id]->setActive(false);
            m_battery_callbacks.erase(id);
            if (m_battery_callbacks.empty()) {
                unregisterVconfCallback(VCONFKEY_SYSMAN_BATTERY_CAPACITY,
                        SystemInfo::onBatteryChanged);
                unregisterVconfCallback(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW,
                        SystemInfo::onBatteryChanged);
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_storage_callbacks.find(id) != m_storage_callbacks.end()) {
        if (on_timeout || m_storage_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_storage_callbacks[id]->setActive(false);
            m_storage_callbacks.erase(id);
            if (m_storage_callbacks.empty()) {
                unregisterVconfCallback(VCONFKEY_SYSMAN_MMC_STATUS, SystemInfo::mmcStatusChanged);
                stopAndDestroyTimer(&m_storage_timer, std::string("Storage"));
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_cpu_callbacks.find(id) != m_cpu_callbacks.end()) {
        if (on_timeout || m_cpu_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_cpu_callbacks[id]->setActive(false);
            m_cpu_callbacks.erase(id);
            if (m_cpu_callbacks.empty()) {
                stopAndDestroyTimer(&m_cpu_timer, std::string("Cpu"));
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_display_callbacks.find(id) != m_display_callbacks.end()) {
        if (on_timeout || m_display_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_display_callbacks[id]->setActive(false);
            m_display_callbacks.erase(id);
            if (m_display_callbacks.empty()) {
                unregisterVconfCallback(VCONFKEY_SETAPPL_LCD_BRIGHTNESS,
                        SystemInfo::onDisplayChanged);
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_peripheral_callbacks.find(id) != m_peripheral_callbacks.end()) {
        if (on_timeout || m_peripheral_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_peripheral_callbacks[id]->setActive(false);
            m_peripheral_callbacks.erase(id);
            int value = 0;
            if (m_peripheral_callbacks.empty()) {
                if (vconf_get_int(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS, &value) != -1) {
                    unregisterVconfCallback(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS,
                            SystemInfo::onPeripheralChanged);
                }
                if (vconf_get_int(VCONFKEY_SYSMAN_HDMI, &value) != -1) {
                    unregisterVconfCallback(VCONFKEY_SYSMAN_HDMI, SystemInfo::onPeripheralChanged);
                }
                if (vconf_get_int(VCONFKEY_POPSYNC_ACTIVATED_KEY, &value) != -1) {
                    unregisterVconfCallback(VCONFKEY_POPSYNC_ACTIVATED_KEY,
                            SystemInfo::onPeripheralChanged);
                }
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_device_orientation_callbacks.find(id) != m_device_orientation_callbacks.end()) {
        if (on_timeout || m_device_orientation_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_device_orientation_callbacks[id]->setActive(false);
            m_device_orientation_callbacks.erase(id);
            if (m_device_orientation_callbacks.empty()) {
                unregisterVconfCallback(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL,
                        SystemInfo::onDeviceAutoRotationChanged);
                sensord_unregister_event(m_sensor_handle, AUTO_ROTATION_EVENT_CHANGE_STATE);
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_locale_callbacks.find(id) != m_locale_callbacks.end()) {
        if (on_timeout || m_locale_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_locale_callbacks[id]->setActive(false);
            m_locale_callbacks.erase(id);
            if (m_locale_callbacks.empty()) {
                if (SYSTEM_SETTINGS_ERROR_NONE
                        != system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE)) {
                    LOGE("unregistration of language change callback failed");
                }
                if (SYSTEM_SETTINGS_ERROR_NONE
                        != system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY)) {
                    LOGE("unregistration of country change callback failed");
                }
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_network_callbacks.find(id) != m_network_callbacks.end()) {
        if (on_timeout || m_network_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_network_callbacks[id]->setActive(false);
            m_network_callbacks.erase(id);
            if (m_wifi_network_callbacks.empty() && m_cellular_network_callbacks.empty()) {
                //wifi and cellular has common core callback
                unregisterIpChangeCallback();
            }
            if (m_network_callbacks.empty()) {
                LOGD("the native callback for NETWORK listeners will be removed.");
                unregisterNetworkTypeChangeCallback();
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    }
#ifdef FEATURE_OPTIONAL_WI_FI
    else if (m_wifi_network_callbacks.find(id) != m_wifi_network_callbacks.end()) {
        if (on_timeout || m_wifi_network_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_wifi_network_callbacks[id]->setActive(false);
            m_wifi_network_callbacks.erase(id);
            if (m_wifi_network_callbacks.empty()) {
                if (m_cellular_network_callbacks.empty()) {
                    //wifi and cellular has common core callback
                    unregisterIpChangeCallback();
                }
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    }
#endif
    else if (m_is_telephony_enable && (m_cellular_network_callbacks.find(id) != m_cellular_network_callbacks.end())) {
        if (on_timeout || m_cellular_network_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_cellular_network_callbacks[id]->setActive(false);
            m_cellular_network_callbacks.erase(id);
            if (m_cellular_network_callbacks.empty()) {
                if (m_wifi_network_callbacks.empty()) {
                    //wifi and cellular has common core callback
                    unregisterIpChangeCallback();
                }
                unregisterVconfCallback(VCONFKEY_TELEPHONY_FLIGHT_MODE,
                        onConnectionNetworkParamChanged);
                unregisterVconfCallback(VCONFKEY_TELEPHONY_CELLID, onConnectionNetworkParamChanged);
                unregisterVconfCallback(VCONFKEY_TELEPHONY_LAC, onConnectionNetworkParamChanged);
                unregisterVconfCallback(VCONFKEY_TELEPHONY_SVC_ROAM,
                        onConnectionNetworkParamChanged);
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_is_telephony_enable && (m_sim_callbacks.find(id) != m_sim_callbacks.end())) {
        if (on_timeout || m_sim_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_sim_callbacks[id]->setActive(false);
            m_sim_callbacks.erase(id);
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_memory_callbacks.find(id) != m_memory_callbacks.end()) {
        if (on_timeout || m_memory_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_memory_callbacks[id]->setActive(false);
            m_memory_callbacks.erase(id);
            if (m_memory_callbacks.empty()) {
                unregisterVconfCallback(VCONFKEY_SYSMAN_LOW_MEMORY, SystemInfo::onMemoryChanged);
            }
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else if (m_inactive_callbacks.find(id) != m_inactive_callbacks.end()) {
        if (on_timeout || m_inactive_callbacks[id]->getContext() == g_ctx) {
            stopPropertyWatcherTimer(id);
            m_inactive_callbacks[id]->setActive(false);
            m_inactive_callbacks.erase(id);
        } else {
            std::string log_msg = "Removing callbacks from another context is not allowed";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
        }
    } else {
        std::string log_msg = "ListenerId: " + std::to_string(id) + " not found";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException<InvalidValuesException>(0, log_msg);
    }
    return;
}

void SystemInfo::startPropertyWatcherTimer(unsigned long listener_id, unsigned long timeout)
{
    LOGD("Entered");

    if (timeout > 0) {
        double value = static_cast<double>(timeout) / 1000.0;
        unsigned long* id = new (std::nothrow) unsigned long(listener_id);
        if (!id) {
            std::string log_msg = "Failed to allocate memory";
            LOGE("%s", log_msg.c_str());
            SystemInfoUtil::throwSystemInfoException(0, log_msg);
        }
        Ecore_Timer* timer = ecore_timer_add(value, SystemInfo::onPropertyWatcherTimeout,
                (void *) id);
        if (!timer) {
            std::string log_msg = "Failed to add timer for listener id";
            LOGE("%s, %d", log_msg.c_str(), listener_id);
            SystemInfoUtil::throwSystemInfoException(0, log_msg);
        }
        m_watcher_map[listener_id] = timer;
    }
}

void SystemInfo::stopPropertyWatcherTimer(unsigned long listener_id)
{
    LOGD("Entered");

    std::map<unsigned long, Ecore_Timer*>::iterator watcher_it = m_watcher_map.find(listener_id);
    if (watcher_it != m_watcher_map.end()) {
        stopAndDestroyTimer(&watcher_it->second, std::to_string(watcher_it->first));
        m_watcher_map.erase(watcher_it);
    } else {
        LOGW("Timer not set for listener id: %u", listener_id);
    }
}

Eina_Bool SystemInfo::onPropertyWatcherTimeout(void* event_ptr)
{
    LOGD("Entered");

    unsigned long *listener_id = static_cast<unsigned long*>(event_ptr);
    try {
        if (listener_id) {
            SystemInfo::getInstance().removePropertyValueChangeListener(NULL, *listener_id);
        } else {
            LOGE("Listener id is NULL");
        }
    } catch (...) {
        LOGE("Unexpected exception");
    }
    delete listener_id;

    return ECORE_CALLBACK_CANCEL;
}

void SystemInfo::onBatteryChanged(keynode_t *node, void *event_ptr)
{
    LOGD("Entered");

    SystemInfo::getInstance().broadcastBatteryChanged();
}

void SystemInfo::broadcastBatteryChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_battery_callbacks.begin();

        while (itr != m_battery_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            SystemInfoBatteryPtr battery(new SystemInfoBattery());
            SystemInfoOptions opt = callback->getOptions();
            if (((opt.high_threshold == 0) && (opt.low_threshold == 0))
                    || ((opt.high_threshold > 0) && (battery->getLevel() >= opt.high_threshold))
                    || ((opt.low_threshold > 0) && (battery->getLevel() <= opt.low_threshold))) {
                callback->setNativeProperty(battery);

                PropertyCallbackDataHolder* holder =
                        new (std::nothrow) PropertyCallbackDataHolder();
                if (!holder) {
                    std::string log_msg = "Failed to allocate memory";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
                holder->ptr = callback;

                guint id = g_idle_add(getPropertyValueCallback, holder);
                if (!id) {
                    LOGE("g_idle_add fails");
                    delete holder;
                }
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastBatteryChanged fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastBatteryChanged fails");
    }
}

Eina_Bool SystemInfo::onStorageChanged(void *event_ptr)
{
    LOGD("Entered");

    SystemInfo::getInstance().broadcastStorageChanged();
    return ECORE_CALLBACK_RENEW;
}

void SystemInfo::mmcStatusChanged(keynode_t *node, void *event_ptr)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastStorageChanged();
}

void SystemInfo::broadcastStorageChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_storage_callbacks.begin();

        SystemInfoStoragePtr storage(new SystemInfoStorage());
        if (last_storage) {
            if (*last_storage == *storage) {
                LOGD("Storage didn't change. Returning.");
                return;
            }
        }
        last_storage = storage;

        while (itr != m_storage_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(storage);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add failed");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastStorageChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastStorageChanged failed");
    }
}

void SystemInfo::onDisplayChanged(keynode_t *node, void *event_ptr)
{
    LOGD("Entered");

    SystemInfo::getInstance().broadcastDisplayChanged();
}

void SystemInfo::broadcastDisplayChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_display_callbacks.begin();

        while (itr != m_display_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            SystemInfoDisplayPtr display(new SystemInfoDisplay());
            SystemInfoOptions opt = callback->getOptions();
            if (((opt.high_threshold == 0) && (opt.low_threshold == 0))
                    || ((opt.high_threshold > 0) && (display->getBrightness() >= opt.high_threshold))
                    || ((opt.low_threshold > 0) && (display->getBrightness() <= opt.low_threshold))) {
                callback->setNativeProperty(display);

                PropertyCallbackDataHolder* holder =
                        new (std::nothrow) PropertyCallbackDataHolder();
                if (!holder) {
                    std::string log_msg = "Failed to allocate memory";
                    LOGE("%s", log_msg.c_str());
                    SystemInfoUtil::throwSystemInfoException(0, log_msg);
                }
                holder->ptr = callback;

                guint id = g_idle_add(getPropertyValueCallback, holder);
                if (!id) {
                    LOGE("g_idle_add failed");
                    delete holder;
                }
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastDisplayChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastDisplayChanged failed");
    }
}

void SystemInfo::onDeviceAutoRotationChanged(keynode_t *node, void *event_ptr)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastDeviceOrientationChanged();
}

void SystemInfo::onDeviceOrientationChanged(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data)
{
    LOGD("Entered");
    int rotation = 0;

    LOGD("size of the data value array:%d", data->value_count);
    if (data->value_count > 0) {
        rotation = data->values[0];
        LOGD("ratation value : %d", rotation);
        if (rotation != AUTO_ROTATION_DEGREE_UNKNOWN) {
            SystemInfo::getInstance().broadcastDeviceOrientationChanged();
        }
    } else {
        LOGE("failed to get data : the size of array is less than 0");
    }
}

void SystemInfo::broadcastDeviceOrientationChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_device_orientation_callbacks.begin();

        while (itr != m_device_orientation_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            SystemInfoDeviceOrientationPtr dev_orientation(new SystemInfoDeviceOrientation());
            callback->setNativeProperty(dev_orientation);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add failed");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastDeviceOrientationChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastDeviceOrientationChanged failed");
    }
}

void SystemInfo::onLocaleChanged(system_settings_key_e key, void* event_ptr)
{
    LOGD("Entered");

    SystemInfo::getInstance().broadcastLocaleChanged();
}

void SystemInfo::broadcastLocaleChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_locale_callbacks.begin();

        while (itr != m_locale_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            SystemInfoLocalePtr locale(new SystemInfoLocale());
            callback->setNativeProperty(locale);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add failed");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastLocaleChanged failed, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastLocaleChanged failed");
    }
}

SystemInfoCpuPtr SystemInfo::createSystemInfoCpu()
{
    std::lock_guard < std::mutex > lock(m_cpu_info_lock);
    return std::make_shared < SystemInfoCpu > (m_cpu_info);
}

void SystemInfo::stopAndDestroyTimer(Ecore_Timer ** _p_timer, const std::string & _msg)
{
    if (_p_timer && *_p_timer) {
        LOGD("Deleting timer for %s listener", _msg.c_str());
        ecore_timer_del(*_p_timer);
        *_p_timer = NULL;
    } else {
        LOGD("Deleting timer for %s listener failed, timer already NULL", _msg.c_str());
    }
}

void SystemInfo::onPeripheralChanged(keynode_t* /*node*/, void* /*event_ptr*/)
{
    LOGD("Entered");

    SystemInfo::getInstance().broadcastPeripheralChanged();
}

void SystemInfo::broadcastPeripheralChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_peripheral_callbacks.begin();
        SystemInfoPeripheralPtr p_peripheral(new SystemInfoPeripheral());

        while (itr != m_peripheral_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(p_peripheral);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add fails");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastPeripheralChanged fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastPeripheralChanged fails");
    }
}

void SystemInfo::onMemoryChanged(keynode_t* /*node*/, void* /*event_ptr*/)
{
    LOGD("Entered");
    SystemInfo::getInstance().broadcastMemoryChanged();
}

void SystemInfo::broadcastMemoryChanged()
{
    LOGD("Entered");

    try {
        PVCLmap::iterator itr = m_memory_callbacks.begin();
        SystemInfoMemoryPtr p_memory(new SystemInfoMemory());

        while (itr != m_memory_callbacks.end()) {
            PropertyCallbackDataPtr callback = itr->second;
            callback->setNativeProperty(p_memory);

            PropertyCallbackDataHolder* holder = new (std::nothrow) PropertyCallbackDataHolder();
            if (!holder) {
                std::string log_msg = "Failed to allocate memory";
                LOGE("%s", log_msg.c_str());
                SystemInfoUtil::throwSystemInfoException(0, log_msg);
            }
            holder->ptr = callback;

            guint id = g_idle_add(getPropertyValueCallback, holder);
            if (!id) {
                LOGE("g_idle_add fails");
                delete holder;
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("broadcastMemoryChanged fails, %s: %s", err.getName().c_str(),
                err.getMessage().c_str());
    } catch (...) {
        LOGE("broadcastMemoryChanged fails");
    }
}

void SystemInfo::registerVconfCallback(const char *in_key, vconf_callback_fn cb)
{
    if (0 != vconf_notify_key_changed(in_key, cb, NULL)) {
        std::string log_msg = "Failed to register vconf callback";
        LOGE("%s, %s", log_msg.c_str(), in_key);
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
}

void SystemInfo::unregisterVconfCallback(const char *in_key, vconf_callback_fn cb)
{
    if (0 != vconf_ignore_key_changed(in_key, cb)) {
        std::string log_msg = "Failed to unregister vconf callback";
        LOGE("%s, %s", log_msg.c_str(), in_key);
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
}

gboolean SystemInfo::complete(void *data)
{
    LOGD("Entered");

    auto holder = static_cast<PropertyCallbackDataHolder*>(data);
    if (!holder) {
        LOGE("callback holder is null");
        return false;
    }
    auto callback = holder->ptr;
    if (!callback) {
        LOGE("callback is null");
        delete holder;
        holder = NULL;
        return false;
    }

    JSContextRef context = callback->getContext();

    if (callback->isError()) {
        LOGD("ErrorCallback triggered");
        JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(context, callback->getErrorName(),
                callback->getErrorMessage());
        callback->callErrorCallback(error);
    }

    delete holder;
    holder = NULL;

    return false;
}

void SystemInfo::setTapiHandles()
{
    LOGD("Entered");

    char **cp_list = NULL;
    unsigned int modem_num = 0;

    cp_list = tel_get_cp_name_list();

    if (cp_list != NULL) {
        while (cp_list[modem_num]) {
            LOGD("cp_name = %s", cp_list[modem_num]);
            m_tapi_handle[modem_num] = tel_init(cp_list[modem_num]);
            if (m_tapi_handle[modem_num] == NULL) {
                LOGE("Failed to connect with tapi, handle is null");
            }
            modem_num++;
        }
    } else {
        LOGE("Failed to get cp list");
    }
    g_strfreev(cp_list);

    return;
}


} // SystemInfo
} // DeviceAPI
