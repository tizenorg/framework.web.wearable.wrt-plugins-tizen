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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <iostream>
#include <Commons/Exception.h>
#include <dpl/exception.h>
#include <map>

#include "plugin_config.h"

#define APPLICATION_FEATURE_API_LAUNCH  "http://tizen.org/privilege/application.launch"
#define APPLICATION_FEATURE_API_KILL    "http://tizen.org/privilege/application.kill"
#define APPLICATION_FEATURE_API_KILL2   "http://tizen.org/privilege/appmanager.kill"
#define APPLICATION_FEATURE_API_CERT    "http://tizen.org/privilege/appmanager.certificate"
#define APPLICATION_FEATURE_API_INFO        "http://tizen.org/privilege/application.info"


// for backword compatibility
#define APPLICATION_FEATURE_API_READ    "http://tizen.org/privilege/application.read"
//#define APPLICATION_FEATURE_API_INSTALL   "http://tizen.org/privilege/application.install"

#define APPLICATION_DEVICE_CAP_LAUNCH   "application.launch"
#define APPLICATION_DEVICE_CAP_KILL     "appmanager.kill"
#define APPLICATION_DEVICE_CAP_CERT     "appmanager.certificate"
#define APPLICATION_DEVICE_CAP_INFO     "application.info"

//#define APPLICATION_DEVICE_CAP_READ       "application.read"
//#define APPLICATION_DEVICE_CAP_INSTALL    "application.install"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Application {

static FunctionMapping createApplicationFunctions();

static FunctionMapping ApplicationFunctions =
    createApplicationFunctions();

DEFINE_FUNCTION_GETTER(Application, ApplicationFunctions);

static FunctionMapping createApplicationFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_LAUNCH, APPLICATION_DEVICE_CAP_LAUNCH);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_KILL, APPLICATION_DEVICE_CAP_KILL);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_CERT, APPLICATION_DEVICE_CAP_CERT);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_INFO, APPLICATION_DEVICE_CAP_INFO);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_LAUNCH);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_KILL);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_CERT);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_INFO);

    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_LAUNCH, DEVICE_CAP_APPLICATION_LAUNCH);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_KILL, DEVICE_CAP_APPLICATION_KILL);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_CERT, DEVICE_CAP_APPLICATION_CERT);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_INFO, DEVICE_CAP_APPLICATION_INFO);

#if 0
    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_READ, APPLICATION_DEVICE_CAP_READ);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_APPLICATION_INSTALL, APPLICATION_DEVICE_CAP_INSTALL);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_READ);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_APPLICATION_INSTALL);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_READ, DEVICE_CAP_APPLICATION_READ);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_APPLICATION_INSTALL, DEVICE_CAP_APPLICATION_INSTALL);
#endif

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_LAUNCH, APPLICATION_FEATURE_API_LAUNCH);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_KILL, APPLICATION_FEATURE_API_KILL);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_KILL2, APPLICATION_FEATURE_API_KILL2);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_CERT, APPLICATION_FEATURE_API_CERT);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_READ, APPLICATION_FEATURE_API_READ);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_INFO, APPLICATION_FEATURE_API_INFO);


    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_LAUNCH);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_LAUNCH, FEATURE_APPLICATION_LAUNCH);

    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_KILL);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_KILL, FEATURE_APPLICATION_KILL);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_KILL, FEATURE_APPLICATION_KILL2);

    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_CERT);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_CERT, FEATURE_APPLICATION_CERT);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_CERT, FEATURE_APPLICATION_READ);

    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_INFO);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_INFO, FEATURE_APPLICATION_INFO);


#if 0
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_READ, APPLICATION_FEATURE_API_READ);
    ACE_CREATE_FEATURE(FEATURE_APPLICATION_INSTALL, APPLICATION_FEATURE_API_INSTALL);

    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_READ);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_READ, FEATURE_APPLICATION_READ);

    ACE_CREATE_FEATURE_LIST(APPLICATION_FEATURES_APPLICATION_INSTALL);
    ACE_ADD_API_FEATURE(APPLICATION_FEATURES_APPLICATION_INSTALL, FEATURE_APPLICATION_INSTALL);
#endif

    /**
     * Functions
     */
    FunctionMapping applicationMapping;

    // launch
    AceFunction launchFunc = ACE_CREATE_FUNCTION(
            FUNCTION_LAUNCH,
            APPLICATION_FUNCTION_API_LAUNCH,
            APPLICATION_FEATURES_APPLICATION_LAUNCH,
            DEVICE_LIST_APPLICATION_LAUNCH);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_LAUNCH,
                               launchFunc));

    // launchAppControl
    AceFunction launchAppControlFunc = ACE_CREATE_FUNCTION(
            FUNCTION_LAUNCH_APP_CONTROL,
            APPLICATION_FUNCTION_API_LAUNCH_APP_CONTROL,
            APPLICATION_FEATURES_APPLICATION_LAUNCH,
            DEVICE_LIST_APPLICATION_LAUNCH);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_LAUNCH_APP_CONTROL,
                               launchAppControlFunc));

    // kill
    AceFunction killFunc = ACE_CREATE_FUNCTION(
            FUNCTION_KILL,
            APPLICATION_FUNCTION_API_KILL,
            APPLICATION_FEATURES_APPLICATION_KILL,
            DEVICE_LIST_APPLICATION_KILL);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_KILL,
                               killFunc));


    // getAppCerts
    AceFunction getAppCertsFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_APP_CERTS,
            APPLICATION_FUNCTION_API_GET_APP_CERTS,
            APPLICATION_FEATURES_APPLICATION_CERT,
            DEVICE_LIST_APPLICATION_CERT);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_GET_APP_CERTS,
                               getAppCertsFunc));

        // getAppMetaData
        AceFunction getAppMetaDataFunc = ACE_CREATE_FUNCTION(
                FUNCTION_GET_APP_META_DATA,
                APPLICATION_FUNCTION_API_GET_APP_META_DATA,
                APPLICATION_FEATURES_APPLICATION_INFO,
                DEVICE_LIST_APPLICATION_INFO);

        applicationMapping.insert(std::make_pair(
                                   APPLICATION_FUNCTION_API_GET_APP_META_DATA,
                                   getAppMetaDataFunc));

#if 0

    // setUserAgent
    AceFunction setUserAgentFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_USER_AGENT,
            APPLICATION_FUNCTION_API_SET_USER_AGENT,
            APPLICATION_FEATURES_APPLICATION_READ,
            EMPTY_DEVICE_LIST);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_SET_USER_AGENT,
                               setUserAgentFunc));

    // getAppsInfo
    AceFunction getAppsInfoFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_APPLICATION_INFORMATIONS,
            APPLICATION_FUNCTION_API_GET_APPS_INFO,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_GET_APPS_INFO,
                               getAppsInfoFunc));

    // getAppInfo
    AceFunction getAppInfoFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_APPLICATION_INFORMATION,
            APPLICATION_FUNCTION_API_GET_APP_INFO,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_GET_APP_INFO,
                               getAppInfoFunc));

    // getAppsContext
    AceFunction getAppsContextFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_RUNNING_APPLICATION_CONTEXTS,
            APPLICATION_FUNCTION_API_GET_APPS_CONTEXT,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_GET_APPS_CONTEXT,
                               getAppsContextFunc));

    // getAppContext
    AceFunction getAppContextFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_CURRENT_APPLICATION_CONTEXT,
            APPLICATION_FUNCTION_API_GET_APP_CONTEXT,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_GET_APP_CONTEXT,
                               getAppContextFunc));

    // getAppsContext
    AceFunction findAppControlFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_RUNNING_APPLICATION_CONTEXTS,
            APPLICATION_FUNCTION_API_FIND_APP_CONTROL,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_FIND_APP_CONTROL,
                               findAppControlFunc));

    // addAppInfoEventListener
    AceFunction addAppInfoEventListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_ADD_APPLICATION_INFORMATION_EVENT_LISTENER,
            APPLICATION_FUNCTION_API_ADD_APP_INFO_EVENT_LISTENER,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_ADD_APP_INFO_EVENT_LISTENER,
                               addAppInfoEventListenerFunc));

    // removeAppInfoEventListener
    AceFunction removeAppInfoEventListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE_APPLICATION_INFORMATION_EVENT_LISTENER,
            APPLICATION_FUNCTION_API_REMOVE_APP_INFO_EVENT_LISTENER,
            APPLICATION_FEATURES_APPLICATION_READ,
            DEVICE_LIST_APPLICATION_READ);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_REMOVE_APP_INFO_EVENT_LISTENER,
                               removeAppInfoEventListenerFunc));

    // install
    AceFunction installFunc = ACE_CREATE_FUNCTION(
            FUNCTION_INSTALL_APP,
            APPLICATION_FUNCTION_API_INSTALL_APP,
            APPLICATION_FEATURES_APPLICATION_INSTALL,
            DEVICE_LIST_APPLICATION_INSTALL);
    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_INSTALL_APP,
                               installFunc));

    // uninstall
    AceFunction uninstallFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNINSTALL_APP,
            APPLICATION_FUNCTION_API_UNINSTALL_APP,
            APPLICATION_FEATURES_APPLICATION_INSTALL,
            DEVICE_LIST_APPLICATION_INSTALL);
    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_UNINSTALL_APP,
                               uninstallFunc));

    // update
    AceFunction updateFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UPDATE_APP,
            APPLICATION_FUNCTION_API_UPDATE_APP,
            APPLICATION_FEATURES_APPLICATION_INSTALL,
            DEVICE_LIST_APPLICATION_INSTALL);
    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_UPDATE_APP,
                               updateFunc));
#endif
    // size
    AceFunction sizeAttr = ACE_CREATE_FUNCTION(
            ATTRIBUTE_SIZE,
            APPLICATION_FUNCTION_API_SIZE,
            APPLICATION_FEATURES_APPLICATION_INFO,
            DEVICE_LIST_APPLICATION_INFO);

    applicationMapping.insert(std::make_pair(
                               APPLICATION_FUNCTION_API_SIZE,
                               sizeAttr));

    return applicationMapping;
}

}
}
