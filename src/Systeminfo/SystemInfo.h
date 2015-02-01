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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_H__

#include "SystemInfoPropertySuccessCallbackData.h"
#include "SystemInfoDeviceCapability.h"
#include "SystemInfoStorage.h"
#include "SystemInfoDeviceOrientation.h"
#include "SystemInfoLocale.h"

#include <glib.h>
#include <Ecore.h>
#include <vconf.h>
#include <memory>
#include <mutex>
#include <vector>
#include <map>
#include <net_connection.h>
#include <system_settings.h>

#ifdef FEATURE_OPTIONAL_TELEPHONY
#include <tapi_common.h>
#endif

#define TAPI_HANDLE_MAX 2
#define DEFAULT_PROPERTY_COUNT 1

namespace DeviceAPI {
namespace SystemInfo {

//! Map that stores PropertyValueChangeListeners
typedef std::map<unsigned long, PropertyCallbackDataPtr> PVCLmap;

class SystemInfoCpu;

class SystemInfo {
public:
    static SystemInfo& getInstance();
    virtual ~SystemInfo();

    long long getTotalMemory();
    long long getAvailableMemory();

    SystemInfoDeviceCapabilityPtr getCapabilities();
    void getPropertyValue(PropertyCallbackDataPtr callback);

    unsigned long getCount(SystemInfoPropertyId propertyId);

    //! Interface for listener's manipulation (registration and removal)
    unsigned long addPropertyValueChangeListener(PropertyCallbackDataPtr callback);
    void removePropertyValueChangeListener(JSContextRef context, unsigned long id);

    /**
     * Notify that one of processed callbacks is ready to be called
     */
    void notifyGetPropertyValueReady();

private:
    SystemInfo();
    SystemInfo(const SystemInfo&);
    SystemInfo& operator=(const SystemInfo&);

    static gboolean getPropertyValueCallback(void *data);
    static void getPropertyValueThread(gpointer data, gpointer user_data);
    void startPropertyWatcherTimer(unsigned long listener_id, unsigned long timeout);
    void stopPropertyWatcherTimer(unsigned long listener_id);
    static Eina_Bool onPropertyWatcherTimeout(void* event_ptr);

    static gboolean complete(void *data);

    static void onBatteryChanged(keynode_t *node, void *event_ptr);
    void broadcastBatteryChanged();

    static Eina_Bool onStorageChanged(void *event_ptr);
    static void mmcStatusChanged(keynode_t *node, void *event_ptr);
    void broadcastStorageChanged();

    static void onDisplayChanged(keynode_t *node, void *event_ptr);
    void broadcastDisplayChanged();

    static void onDeviceAutoRotationChanged(keynode_t *node, void *event_ptr);
    static void onDeviceOrientationChanged(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data);

    void broadcastDeviceOrientationChanged();

    static Eina_Bool onCpuChanged(void* event_ptr);
    void broadcastCpuChanged();

    void stopAndDestroyTimer(Ecore_Timer ** _p_timer, const std::string & _msg);

    static void onPeripheralChanged(keynode_t *node, void *event_ptr);
    void broadcastPeripheralChanged();

    static void onMemoryChanged(keynode_t *node, void *event_ptr);
    void broadcastMemoryChanged();

    static void onLocaleChanged(system_settings_key_e key, void* event_ptr);
    void broadcastLocaleChanged();

    void addProcessingProperty(PropertyCallbackDataHolder* holder);

    static void registerVconfCallback(const char *in_key, vconf_callback_fn cb);
    static void unregisterVconfCallback(const char *in_key, vconf_callback_fn cb);
    connection_h getConnectionHandle();

    void registerIpChangeCallback();
    void unregisterIpChangeCallback();
    static void onConnectionIpAddressChanged(const char* ipv4_address, const char* ipv6_address,
            void* user_data);
    static void onConnectionNetworkParamChanged(keynode_t *node, void *user_data);
    void broadcastCellularNetworkChanged();

    void broadcastWifiNetworkChanged();

    //! Handler for thread pool
    GThreadPool* m_pool;

    //! Highest used id (in most cases id of last registered listener)
    unsigned long m_current_id;

    //! Mutex for m_processing_properties
    std::mutex m_processing_properties_lock;

    //! Callbacks for each property is stored in separate
    //! collections to simplify adding and searching on call
    //! mutex is not used because core API callback is called by main thread
    PVCLmap m_battery_callbacks;
    PVCLmap m_cpu_callbacks;
    PVCLmap m_storage_callbacks;
    PVCLmap m_display_callbacks;
    PVCLmap m_device_orientation_callbacks;
    PVCLmap m_locale_callbacks;
    PVCLmap m_network_callbacks;

    PVCLmap m_wifi_network_callbacks;

    PVCLmap m_cellular_network_callbacks;
    PVCLmap m_sim_callbacks;

    PVCLmap m_peripheral_callbacks;
    PVCLmap m_memory_callbacks;
    PVCLmap m_inactive_callbacks;

    //! Map for timeout functionality
    std::map<unsigned long, Ecore_Timer*> m_watcher_map;

    //! Timer for a storage listener
    Ecore_Timer *m_storage_timer;
    std::shared_ptr<SystemInfoStorage> last_storage;

    static CpuInfo m_cpu_info;
    static std::shared_ptr<SystemInfoCpu> createSystemInfoCpu();
    static std::mutex m_cpu_info_lock;

    //! Timer for a cpu load listener
    Ecore_Timer *m_cpu_timer;
    double m_cpu_last_load;
    int m_sensor_handle;

#ifdef FEATURE_OPTIONAL_TELEPHONY
    //! Handle for tapi communication
    TapiHandle *m_tapi_handle[TAPI_HANDLE_MAX+1];
#else
    void *m_tapi_handle;
#endif

#ifdef FEATURE_OPTIONAL_WI_FI
    //! Handle for net connection
    connection_h m_connection_handle;
#else
    void *m_connection_handle;
#endif
    //! A list of properties being processed asynchronously
    std::vector<PropertyCallbackDataHolder*> m_processing_properties;
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_H__
