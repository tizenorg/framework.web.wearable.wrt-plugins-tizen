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


#ifndef TIZENAPIS_PLATFORM_APPLICATION_H_
#define TIZENAPIS_PLATFORM_APPLICATION_H_

#include <map>
#include <set>
#include <list>
#include <mutex>
#include <Commons/Emitters.h>
#include <Logger.h>

#include "IApplicationManager.h"
#include "ApplicationFactory.h"
#include "Application.h"
#include "ApplicationContext.h"
#include "ApplicationInformation.h"
#include "ApplicationCert.h"
#include "ApplicationMetaData.h"

//#include <app_manager.h>
#include <app_control.h>

#include "AppManagerWrapper.h"

namespace DeviceAPI {
namespace Application {

class ApplicationManager: public IApplicationManager, public IAppManagerAppListChangedCallbacks {
    friend class ApplicationFactory;

public:
    ApplicationManager();
    virtual ~ApplicationManager();
    virtual void launch(const EventApplicationLaunchPtr& event);
    virtual void kill(const EventApplicationKillPtr& event);
    virtual void launchAppControl(const EventApplicationLaunchAppControlPtr& event);
    virtual void findAppControl(const EventApplicationFindAppControlPtr& event);
    virtual void getAppsContext(const EventApplicationGetAppsContextPtr& event);
    virtual void getAppsInfo(const EventApplicationGetAppsInfoPtr& event);
    virtual void addAppInfoEventListener(
            const EventApplicationAddAppInfoEventListenerPtr& event);
    virtual void removeAppInfoEventListener(
            const EventApplicationRemoveAppInfoEventListenerPtr& event);

    void invokeManualAnswerLaunchAppControl(app_control_h request,
            app_control_h reply,
            app_control_result_e result,
            EventApplicationLaunchAppControlReplyPtr &event);
    void invokeManualAnswerKill(int pid);

    static bool service_extra_data_callback(app_control_h service,
            const char *key,
            void* user_data);

    static ApplicationPtr getCurrentApplication();
    static ApplicationContextPtr getAppContext(const std::string id);
    static ApplicationInformationPtr getAppInfo(const std::string id);
    static ApplicationCertArrayPtr getAppCerts(const std::string id);
    static std::string getAppSharedURI(const std::string appId);
    static ApplicationMetaDataArrayPtr getAppMetaData(const std::string id);

protected:
    virtual void OnRequestReceived(const EventApplicationLaunchPtr& event);
    virtual void OnRequestReceived(const EventApplicationKillPtr& event);
    virtual void OnRequestReceived(const EventApplicationLaunchAppControlPtr& event);
    virtual void OnRequestReceived(const EventApplicationLaunchAppControlReplyPtr& event);
    virtual void OnRequestReceived(const EventApplicationFindAppControlPtr& event);
    virtual void OnRequestReceived(const EventApplicationGetAppsContextPtr& event);
    virtual void OnRequestReceived(const EventApplicationGetAppsInfoPtr& event);
    virtual void OnRequestReceived(const EventApplicationAddAppInfoEventListenerPtr& event);
    virtual void OnRequestReceived(const EventApplicationRemoveAppInfoEventListenerPtr& event);

    // inherited from IAppManagerAppListChangedCallbacks
    virtual void onAppManagerEventInstalled(const char *appId);
    virtual void onAppManagerEventUninstalled(const char *appId);
    virtual void onAppManagerEventUpdated(const char *appId);

private:
    void initialize();

    std::mutex m_initializationMutex;
    bool m_initialized;

    std::map<int, EventApplicationKillPtr> m_killEventMap;
    mutable std::mutex m_killMapLock;

    typedef WrtDeviceApis::Commons::Emitters<EventApplicationAppInfoEventListenerEmitter> EventApplicationAppInfoEventListenerEmitters;
    typedef std::map<long, EventApplicationAppInfoEventListenerEmitter::IdType> WatchIdMap;

    EventApplicationAppInfoEventListenerEmitters m_installedApplicationsEmitters;
    WatchIdMap      m_watchIdMap;

};

class LaunchAppControlPendingEvent
{
public:
    LaunchAppControlPendingEvent(void *thisObject,
            const EventApplicationLaunchAppControlReplyPtr &event) :
        m_thisObject(thisObject),
        m_event(event)
    {
        LOGD("Entered");
    }

    virtual ~LaunchAppControlPendingEvent()
    {
        LOGD("Entered");
    }

    void* getThisObject() const
    {
        LOGD("Entered");
        return m_thisObject;
    }
    EventApplicationLaunchAppControlReplyPtr getEvent() const
    {
        LOGD("Entered");
        return m_event;
    }

private:
    void *m_thisObject;
    EventApplicationLaunchAppControlReplyPtr m_event;
};


template <class KeyType, class DataType>
class KeyMultiMap
{
public:
    typedef unsigned int                                DataKeyType;
    typedef DataType *                                  DataPtrType;
    typedef std::pair<KeyType, DataPtrType>             KeyDataPairType;
    typedef std::map<DataKeyType, KeyDataPairType>      DataMapType;
    typedef std::set<DataKeyType>                       DataKeySetType;
    typedef std::map<KeyType, DataKeySetType>           KeyMapType;
    typedef std::list<DataPtrType>                      DataPtrListType;

    KeyMultiMap() :
        m_keyAcc(0)
    {
        LOGD("Entered");
    }

    DataKeyType insert(const KeyType &key, const DataPtrType &dataPtr)
    {
        LOGD("Entered");
        DataKeyType dataKey = ++m_keyAcc;

        KeyDataPairType pair(key, dataPtr);
        m_dataMap.insert(std::pair<DataKeyType, KeyDataPairType>(dataKey, pair));

        typename KeyMapType::iterator keyMapIter = m_keyMap.find(key);
        if (keyMapIter == m_keyMap.end())
        {
            DataKeySetType newKeySet;
            m_keyMap.insert(std::pair<KeyType, DataKeySetType>(key, newKeySet));
            keyMapIter = m_keyMap.find(key);
        }

        DataKeySetType &dataKeySet = keyMapIter->second;

        dataKeySet.insert(dataKey);

        return dataKey;
    }

    DataPtrType getData(const DataKeyType &dataKey) const
    {
        LOGD("Entered");
        typename DataMapType::const_iterator dataMapIter = m_dataMap.find(dataKey);

        if (dataMapIter == m_dataMap.end())
            return static_cast<DataPtrType>(NULL);

        return dataMapIter->second.second;
    }

    DataPtrListType getDataPtrList(const KeyType &key)
    {
        LOGD("Entered");
        DataPtrListType dataPtrList;

        typename KeyMapType::const_iterator keyMapIter = m_keyMap.find(key);
        if (keyMapIter == m_keyMap.end())
            return dataPtrList;

        DataKeySetType keySet = keyMapIter->second;
        DataKeySetType::iterator keySetIter = keySet.begin();
        for (; keySetIter != keySet.end(); keySetIter++)
        {
            DataPtrType dataPtr = getData(*keySetIter);
            if (dataPtr == NULL)
            {
                LOGD("No data for %u", *keySetIter);
                break;
            }

            dataPtrList.push_back(dataPtr);
        }

        return dataPtrList;
    }

    void eraseData(const DataKeyType &dataKey)
    {
        LOGD("Entered");
        typename DataMapType::iterator dataKeyIter = m_dataMap.find(dataKey);

        if (dataKeyIter == m_dataMap.end())
        {
            LOGD("No data for %u", dataKey);
            return;
        }

        KeyType key = dataKeyIter->second.first;

        m_dataMap.erase(dataKeyIter);

        typename KeyMapType::iterator keyMapIter = m_keyMap.find(key);
        if (keyMapIter == m_keyMap.end())
        {
            LOGD("No data for Key");
            return;
        }

        DataKeySetType &keySet = keyMapIter->second;
        DataKeySetType::iterator keySetIter = keySet.find(dataKey);
        if (keySetIter == keySet.end())
        {
            LOGD("No data for %u", dataKey);
            return;
        }

        keySet.erase(keySetIter);
    }

    void eraseKey(const KeyType &key)
    {
        LOGD("Entered");
        typename KeyMapType::iterator keyMapIter = m_keyMap.find(key);
        if (keyMapIter == m_keyMap.end())
        {
            LOGD("No data to erase.");
            return;
        }

        DataKeySetType &keySet = keyMapIter->second;
        DataKeySetType::iterator keySetIter = keySet.begin();
        for (; keySetIter != keySet.end(); keySetIter++)
        {
            typename DataMapType::iterator dataKeyIter = m_dataMap.find(*keySetIter);
            if (dataKeyIter == m_dataMap.end())
            {
                LOGD("No data to erase.");
                break;
            }

            m_dataMap.erase(dataKeyIter);
        }

        m_keyMap.erase(keyMapIter);
    }

private:
    DataKeyType m_keyAcc;

    DataMapType m_dataMap;
    KeyMapType m_keyMap;
};

}
}

#endif
