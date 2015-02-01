//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include <Commons/Exception.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include "ConverterSystemSetting.h"
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <Logger.h>
//#include "JSMessageAttachment.h"
//#include "JSConversation.h"
//#include "JSMessageFolder.h"

using namespace std;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace DeviceAPI {
namespace SystemSetting {

ConverterSystemSetting::ConverterSystemSetting(JSContextRef context) :
    Converter(context)
{
}

ConverterSystemSetting::~ConverterSystemSetting()
{
}

SettingType ConverterSystemSetting::toSettingType(JSValueRef arg)
{
    LOGD("Enter");
    return toSettingType(toJSObjectRef(arg));
}

SettingType ConverterSystemSetting::toSettingType(JSObjectRef arg)
{
    // convert JSvalue to message type enum
    std::string strSettingType = toString(arg);
    LOGD("Setting Type : %s", strSettingType.c_str());

    if (strSettingType.compare("HOME_SCREEN") == 0)
    {
        return HOME_SCREEN;
    }
    else if (strSettingType.compare("LOCK_SCREEN") == 0)
    {
        return LOCK_SCREEN;
    }
    else if (strSettingType.compare("INCOMING_CALL") == 0)
    {
        return INCOMMING_CALL;
    }
    else if (strSettingType.compare("NOTIFICATION_EMAIL") == 0)
    {
        return NOTIFICATION_EMAIL;
    }
    else
    {
        ThrowMsg(WrtDeviceApis::Commons::ConversionException, "Setting type not supported");
    }
}

/*
std::string ConverterMessage::toMessageType(long msgtype)
{
    LOGD("Messasge Type : %ld", msgtype);

    switch (msgtype)
    {
        case Api::Messaging::SMS:
            return "messaging.sms";
        case Api::Messaging::MMS:
            return "messaging.mms";
        case Api::Messaging::EMAIL:
            return "messaging.email";
        default :
            ThrowMsg(WrtDeviceApis::Commons::ConversionException, "message type not supported");
    }
}

std::string ConverterMessage::toMessageStatusType(long msgstatus)
{
    LOGD("Messasge Status : %ld", msgstatus);

    switch (msgstatus)
    {
        case Api::Messaging::MESSAGE_STATUS_SENT:
            return "SENT";
        case Api::Messaging::MESSAGE_STATUS_SENDING:
            return "SENDING";
        case Api::Messaging::MESSAGE_STATUS_FAILED:
            return "FAILED";
        case Api::Messaging::MESSAGE_STATUS_DRAFT:
            return "DRAFT";
        case Api::Messaging::MESSAGE_STATUS_CREATED:
        case Api::Messaging::MESSAGE_STATUS_LOADED:
            return "";
        default :
            ThrowMsg(WrtDeviceApis::Commons::ConversionException, "message status not supported");
    }
}
*/

}
}
