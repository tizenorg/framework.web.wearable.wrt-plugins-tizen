#include "SystemInfoPeripheral.h"
#include <PlatformException.h>
#include <Logger.h>
#include <vconf.h>

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoPeripheral::SystemInfoPeripheral() :
                m_is_video_output_on(fetchIsVideoOutputOn())
{
    LOGD("Entered");
}

SystemInfoPeripheral::~SystemInfoPeripheral()
{
    LOGD("Entered");
}

bool SystemInfoPeripheral::isVideoOutputOn() const
{
    return m_is_video_output_on;
}

bool SystemInfoPeripheral::fetchIsVideoOutputOn()
{
    LOGD("Entered");
    int hdmi_status = 0, wireless_display_status = 0, popsync_status = 0;
    int ret = 0;

    ret = vconf_get_int(VCONFKEY_MIRACAST_WFD_SOURCE_STATUS, &wireless_display_status);
    if (ret == VCONF_OK) {
        if (VCONFKEY_MIRACAST_WFD_SOURCE_ON == wireless_display_status) {
            return true;
        }
    } else {
        LOGE("Failed to get wireless display status");
    }

    ret = vconf_get_int(VCONFKEY_SYSMAN_HDMI, &hdmi_status);
    if (ret == VCONF_OK) {
        if (VCONFKEY_SYSMAN_HDMI_CONNECTED == hdmi_status) {
            return true;
        }
    } else {
        LOGE("Failed to get hdmi status");
    }

    ret = vconf_get_int(VCONFKEY_POPSYNC_ACTIVATED_KEY, &popsync_status);
    if (ret == VCONF_OK) {
        if (1 == popsync_status) {
            return true;
        }
    } else {
        LOGE("Failed to get popsync status");
    }

    return false;
}

} // SystemInfo
} // DeviceAPI
