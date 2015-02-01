#include <system_settings.h>
#include <system_info.h>
#include <net_connection.h>

#ifdef FEATURE_OPTIONAL_WI_FI
#include <wifi.h>
#endif

#ifdef FEATURE_OPTIONAL_TELEPHONY
#include <tapi_common.h>
#endif

#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

std::string SystemInfoUtil::getSystemInfoErrorMessage(const int errorCode)
{
    switch (errorCode) {
        case SYSTEM_INFO_ERROR_NONE:
            return "";
        case SYSTEM_INFO_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case SYSTEM_INFO_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case SYSTEM_INFO_ERROR_IO_ERROR:
            return "IO error";
        case SYSTEM_INFO_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}

std::string SystemInfoUtil::getConnectionErrorMessage(const int errorCode)
{
    switch (errorCode) {
        case CONNECTION_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case CONNECTION_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case CONNECTION_ERROR_INVALID_OPERATION:
            return "Invaild operation";
        case CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
            return "Address family not supported";
        case CONNECTION_ERROR_OPERATION_FAILED:
            return "Operation failed";
        case CONNECTION_ERROR_ITERATOR_END:
            return "Iterator end";
        case CONNECTION_ERROR_NO_CONNECTION:
            return "No connection";
        case CONNECTION_ERROR_ALREADY_EXISTS:
            return "Already exists";
        case CONNECTION_ERROR_OPERATION_ABORTED:
            return "Operation aborted";
        case CONNECTION_ERROR_DHCP_FAILED:
            return "DHCP failed";
        case CONNECTION_ERROR_INVALID_KEY:
            return "Invalid key";
        case CONNECTION_ERROR_NO_REPLY:
            return "No reply";
        case CONNECTION_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case CONNECTION_ERROR_NOT_SUPPORTED:
            return "Not supported";
        default:
            return "Unknown Error";
    }
}

#ifdef FEATURE_OPTIONAL_TELEPHONY
std::string SystemInfoUtil::getTapiErrorMessage(const int errorCode)
{
    switch (errorCode) {
        case TAPI_API_INVALID_INPUT:
            return "Invaild input";
        case TAPI_API_INVALID_PTR:
            return "Invalid pointer";
        case TAPI_API_NOT_SUPPORTED:
            return "Not supported";
        case TAPI_API_DEPRICATED:
            return "Depricated";
        case TAPI_API_SYSTEM_OUT_OF_MEM:
            return "Out of memory";
        case TAPI_API_SYSTEM_RPC_LINK_DOWN:
            return "RPC link down";
        case TAPI_API_SERVICE_NOT_READY:
            return "Service not ready";
        case TAPI_API_SERVER_FAILURE:
            return "Service failure";
        case TAPI_API_OEM_PLUGIN_FAILURE:
            return "Plug-in layer failure";
        case TAPI_API_TRANSPORT_LAYER_FAILURE:
            return "Trasport layer failure";
        case TAPI_API_INVALID_DATA_LEN:
            return "Invalid data length";
        case TAPI_API_REQUEST_MAX_IN_PROGRESS:
            return "Maximum number of requests in progress";
        case TAPI_API_OFFLINE_MODE_ERROR:
            return "Offline mode error";
        case TAPI_EVENT_CLASS_UNKNOWN:
            return "Unknown event class";
        case TAPI_EVENT_UNKNOWN:
            return "Unknown event";
        case TAPI_REGISTRATION_OP_FAILED:
            return "Callback registration failed";
        case TAPI_API_OPERATION_FAILED:
            return "Operation failed";
        case TAPI_API_INVALID_OPERATION:
            return "Invalid operation";
        case TAPI_API_ACCESS_DENIED:
            return "Access denied";
        case TAPI_API_SYSTEM_RPC_LINK_NOT_EST:
            return "RPC link down";
        case TAPI_API_API_NOT_SUPPORTED:
            return "API not supported";
        case TAPI_API_SERVER_LAYER_FAILURE:
            return "Server layer failure";
        case TAPI_API_INVALID_CALL_ID:
            return "Invalid call id";
        case TAPI_API_CALL_CTXT_OVERFLOW:
            return "Call context overflow";
        case TAPI_API_COULD_NOT_GET_CALL_CTXT:
            return "Could not get the call context";
        case TAPI_API_CTXT_SEARCH_RET_NON_CALL_CTXT:
            return "Context search returned the non-call context ";
        case TAPI_API_COULD_NOT_DESTROY_CTXT:
            return "Could not destroy the context ";
        case TAPI_API_INVALID_LINE_ID:
            return "Invalid line ID";
        case TAPI_API_INVALID_CALL_HANDLE:
            return "Invalid call handle";
        case TAPI_API_INVALID_CALL_STATE:
            return "Invalid call state";
        case TAPI_API_CALL_PRE_COND_FAILED:
            return "Pre condition like MO call cannot be established now ";
        case TAPI_API_CALL_SAME_REQ_PENDING:
            return "Cannot accept the same request multiple times ";
        case TAPI_API_MODEM_POWERED_OFF:
            return "Modem is powered off ";
        case TAPI_API_MODEM_ALREADY_ON:
            return "Modem is already on ";
        case TAPI_API_MODEM_ALREADY_OFF:
            return "Modem is already off ";
        case TAPI_API_NETTEXT_DEVICE_NOT_READY:
            return "Nettext device is not ready ";
        case TAPI_API_NETTEXT_SCADDR_NOT_SET:
            return "Nettext SCA address is not set ";
        case TAPI_API_NETTEXT_INVALID_DATA_LEN:
            return "Nettext Invalid data length ";
        case TAPI_NETTEXT_SCADDRESS_NOT_SET:
            return "Nettext SCA address is not set ";
        case TAPI_API_SIM_CARD_ERROR:
            return "SIM general error";
        case TAPI_API_SIM_NOT_FOUND:
            return "SIM is not present";
        case TAPI_API_SIM_NOT_INITIALIZED:
            return "SIM has not initialized yet";
        case TAPI_API_SIM_LOCKED:
            return "PIN/PUK/NCK/NSCK/SPCK/CCK required state";
        case TAPI_API_SIM_PERM_BLOCKED:
            return "SIM Permanently blocked state";
        case TAPI_API_SIM_SERVICE_IS_DISABLED:
            return "Service is disabled in EF-SST due to a given operation";
        case TAPI_API_SAT_INVALID_COMMAND_ID:
            return "Command Number Invalid";
        case TAPI_API_SAT_COMMAND_TYPE_MISMATCH:
            return "Command Type Mismatch";
        case TAPI_API_SAT_EVENT_NOT_REQUIRED_BY_USIM:
            return "Event Not Requested by the USIM";
        case TAPI_API_NETWORK_INVALID_CTXT:
            return "Network invalid context";
        case TAPI_API_NETWORK_ROAMING_NOT_ALLOWED:
            return "3GPP Attache reject cause 11";
        case TAPI_API_MISC_RETURN_NULL:
            return "3GPP Attache reject cause 13 MISC return NULL";
        case TAPI_API_MISC_VALIDITY_ERROR:
            return "MISC validity error";
        case TAPI_API_MISC_INPUTPARM_ERROR:
            return "MISC input parameter error";
        case TAPI_API_MISC_OUTPARAM_NULL:
            return "MISC output parameter NULL";
        default:
            return "Unknown Error";
    }
}
#endif

std::string SystemInfoUtil::getSettingErrorMessage(const int errorCode)
{
    switch (errorCode) {
        case SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case SYSTEM_SETTINGS_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case SYSTEM_SETTINGS_ERROR_IO_ERROR:
            return "IO error";
        case SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case SYSTEM_SETTINGS_ERROR_LOCKSCREEN_APP_PASSWORD_MODE:
            return "Current lock screen app set password type";
        default:
            return "Unknown Error";
    }
}

#ifdef FEATURE_OPTIONAL_WI_FI
std::string SystemInfoUtil::getWIFIErrorMessage(const int errorCode)
{
    switch (errorCode) {
        case WIFI_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case WIFI_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case WIFI_ERROR_INVALID_OPERATION:
            return "Invalid operation";
        case WIFI_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
            return "Address family not supported";
        case WIFI_ERROR_OPERATION_FAILED:
            return "Operation failed";
        case WIFI_ERROR_NO_CONNECTION:
            return "No connection";
        case WIFI_ERROR_NOW_IN_PROGRESS:
            return "Now in progress";
        case WIFI_ERROR_ALREADY_EXISTS:
            return "Already exists";
        case WIFI_ERROR_OPERATION_ABORTED:
            return "Operation aborted";
        case WIFI_ERROR_DHCP_FAILED:
            return "DHCP failed";
        case WIFI_ERROR_INVALID_KEY:
            return "Invalid key";
        case WIFI_ERROR_NO_REPLY:
            return "No reply";
        case WIFI_ERROR_SECURITY_RESTRICTED:
            return "Security restricted";
        case WIFI_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}
#endif

}
//SystemInfo
}//DeviceAPI
