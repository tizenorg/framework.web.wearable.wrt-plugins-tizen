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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <vector>
#include <time.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <Logger.h>
#include <app_alarm.h>

#include "AlarmConverter.h"
#include "JSAlarmAbsolute.h"
#include "AlarmAbsolute.h"
#include "JSAlarmRelative.h"
#include "AlarmRelative.h"
#include "AlarmUtil.h"

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

AlarmConverter::AlarmConverter(JSContextRef context) : WrtDeviceApis::CommonsJavaScript::Converter(context)
{
    LOGD("Entered");
}

AlarmConverter::~AlarmConverter()
{
    LOGD("Entered");
}

int AlarmConverter::toNativeAlarmValue(std::vector<std::string> daysOfTheWeek)
{
    LOGD("Entered");

    int nativeValue = 0;

    for (unsigned int i=0; i<daysOfTheWeek.size(); i++ ) {
        if (daysOfTheWeek[i]=="SU")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_SUNDAY;
        else if (daysOfTheWeek[i]=="MO")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_MONDAY ;
        else if (daysOfTheWeek[i]=="TU")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_TUESDAY ;
        else if (daysOfTheWeek[i]=="WE")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_WEDNESDAY;
        else if (daysOfTheWeek[i]=="TH")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_THURSDAY ;
        else if (daysOfTheWeek[i]=="FR")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_FRIDAY ;
        else if (daysOfTheWeek[i]=="SA")
            nativeValue = nativeValue | ALARM_WEEK_FLAG_SATURDAY ;
    }
    return nativeValue;
}

std::vector<std::string> AlarmConverter::convertFlagToDaysOfTheWeek(int byDayValue)
{
    LOGD("Entered");

    std::vector<std::string> daysOfTheWeek;

    if (byDayValue & ALARM_WEEK_FLAG_SUNDAY)
        daysOfTheWeek.push_back("SU");
    if (byDayValue & ALARM_WEEK_FLAG_MONDAY)
        daysOfTheWeek.push_back("MO");
    if (byDayValue & ALARM_WEEK_FLAG_TUESDAY)
        daysOfTheWeek.push_back("TU");
    if (byDayValue & ALARM_WEEK_FLAG_WEDNESDAY)
        daysOfTheWeek.push_back("WE");
    if (byDayValue & ALARM_WEEK_FLAG_THURSDAY)
        daysOfTheWeek.push_back("TH");
    if (byDayValue & ALARM_WEEK_FLAG_FRIDAY)
        daysOfTheWeek.push_back("FR");
    if (byDayValue & ALARM_WEEK_FLAG_SATURDAY)
        daysOfTheWeek.push_back("SA");

    return daysOfTheWeek;
}

app_control_h AlarmConverter::toService(std::string id)
{
    LOGD("Entered");

    app_control_h service;
    int ret = 0;

    ret = app_control_create(&service);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_set_operation(service, APP_CONTROL_OPERATION_DEFAULT);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_set_app_id(service, id.c_str());
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    return service;
}
app_control_h AlarmConverter::toService(std::string id, std::string page)
{
    LOGD("Entered");

    app_control_h service;
    int ret = 0;

    ret = app_control_create(&service);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_set_operation(service, APP_CONTROL_OPERATION_DEFAULT);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_set_app_id(service, id.c_str());
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    return service;
}

bool AlarmConverter::toAlarmAbsolutePtr(int id, app_control_h handle, AlarmAbsolutePtr privateData)
{
    LOGD("Entered");

    char* dateString = NULL;
    char* alarmType = NULL;
    struct tm date;
    memset(&date, 0, sizeof(tm));
    int ret = ALARM_ERROR_NONE;

    ret = app_control_get_extra_data(handle, ALARM_ALSOLUTE_DATE_KEY, &dateString);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to get AlarmDelay %d, %s", ret, AlarmUtil::getAlarmErrorMessage(ret).c_str());
        return false;
    }
    LOGI("Date Strng = %s", dateString);
    sscanf(dateString, "%d %d %d %d %d %d %d", &date.tm_year, &date.tm_mon,
        &date.tm_mday,  &date.tm_hour, &date.tm_min,  &date.tm_sec, &date.tm_isdst);
    mktime(&date);

    LOGI("Converter AlarmAbsolute Date = Sec : %d, Min : %d, Hour : %d, Day : %d, MON : %d, Year : %d, Is DST ? %d",
            date.tm_sec, date.tm_min, date.tm_hour, date.tm_mday, date.tm_mon, date.tm_year, date.tm_isdst);

    ret = app_control_get_extra_data(handle, ALARM_ALSOLUTE_RECURRENCE_TYPE_KEY, &alarmType);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to get AlarmType %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());

        free(dateString);
        return false;
    }

    if (!strcmp(alarmType, ALARM_ALSOLUTE_RECURRENCE_TYPE_INTERVAL)) {
        int interval = 0;
        ret = alarm_get_scheduled_period(id, &interval);
        if (ret != ALARM_ERROR_NONE) {
            LOGE("Fail to get scheduled period, %d, %s", ret, AlarmUtil::getAlarmErrorMessage(ret).c_str());
        }

        LOGI("interval type alarm: %d", interval);
        privateData->setInterval(interval);
    } else if (!strcmp(alarmType, ALARM_ALSOLUTE_RECURRENCE_TYPE_BYDAYVALUE)) {
        int byDayValue =0;
        ret = alarm_get_scheduled_recurrence_week_flag(id, &byDayValue);
        LOGI("daysOfWeek type alarm: %d, error: %d", byDayValue, ret);
        if (ret==ALARM_ERROR_NONE && byDayValue>0) {
            std::vector<std::string> result;
            result = convertFlagToDaysOfTheWeek(byDayValue);
            privateData->setByDayRecurrence(result);
        } else {
            LOGE("Can't get the recurrence week flag.");
        }
    }

    privateData->setId(id);
    privateData->setDate(date);

    free(dateString);
    free(alarmType);

    return true;
}

bool AlarmConverter::toAlarmRelativePtr(int id, app_control_h handle, AlarmRelativePtr privateData)
{
    LOGD("Entered");

    int interval = 0;
    char* delayString;
    int delay;

    int ret = ALARM_ERROR_NONE;
    ret = alarm_get_scheduled_period(id, &interval);
    if (ret != ALARM_ERROR_NONE) {
        interval = 0;
        LOGE("Failed to get scheduled period, %d, %s", ret, AlarmUtil::getAlarmErrorMessage(ret).c_str());
    }

    ret = app_control_get_extra_data(handle, ALARM_RELATIVE_DELAY_KEY, &delayString);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to get AlarmDelay, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
        return false;
    }
    delay = atoi(delayString);
    free(delayString);

    privateData->setId(id);
    privateData->setDelay(delay);
    privateData->setPeriod(interval);

    return true;
}

int AlarmConverter::toNativeInterval(std::string freq, std::string interval)
{
    LOGD("Entered");
    int freqInSecond = 0;
    int intervalValue = toInt(interval);

    if (!freq.compare(ALARM_PROPERTY_MINUTELY_RECURRENCE))
        freqInSecond = 60;
    else if (!freq.compare(ALARM_PROPERTY_HOURLY_RECURRENCE))
        freqInSecond = 3600;
    else if (!freq.compare(ALARM_PROPERTY_DAILY_RECURRENCE))
        freqInSecond = 3600*24;
    else if (!freq.compare(ALARM_PROPERTY_WEEKLY_RECURRENCE))
        freqInSecond = 3600 * 24 * 7;

    return freqInSecond * intervalValue;
}

bool AlarmConverter::toAlarmService(app_control_h service, DeviceAPI::Application::ApplicationControlPtr appservice)
{
    LOGD("Entered");

    int ret = 0;

    if (appservice->getOperation().compare("") != 0) {
        ret = app_control_set_operation(service, appservice->getOperation().c_str());
        LOGE("Fail to set operation, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    } else {
        LOGD("Error. operation is madatory field. cannot be null");
        return false;
    }

    if (appservice->getUri().compare("") != 0) {
        ret = app_control_set_uri(service, appservice->getUri().c_str() );
        LOGE("Fail to set uri, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    if (appservice->getMime().compare("") != 0) {
        ret = app_control_set_mime(service, appservice->getMime().c_str() );
        LOGE("Fail to set mime, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    std::vector<DeviceAPI::Application::ApplicationControlDataPtr> appControlDataArray = appservice->getAppControlDataArray();

    if (!appControlDataArray.empty()) {
        std::string key;
        const char** arr = NULL;

        for (size_t i = 0; i < appControlDataArray.size(); ++i) {
            key = appControlDataArray.at(i)->getKey();
            if (key.empty()) {
                    return false;
            }
            std::vector<std::string> valueArray = appControlDataArray.at(i)->getValue();
            size_t size = valueArray.size();

            arr = (const char**)calloc(sizeof(char*), size);

            for (size_t j = 0; j < size; j++) {
                arr[j] = valueArray.at(j).c_str();
            }

            app_control_add_extra_data_array(service, key.c_str(), arr, size);

            if (arr)
                free(arr);
        }
    }
    return true;
}

} //Alarm
} //DeviceAPI
