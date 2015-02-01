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

/**
 * @file        FilesystemUtils.h
 */

#ifndef TIZENAPIS_FILESYSTEM_UTILS_H_
#define TIZENAPIS_FILESYSTEM_UTILS_H_

#include <string>
#include <memory>
#include <vector>
#include <Logger.h>
#include <JavaScriptCore/JavaScript.h>
#include <CallbackUserData.h>
#include <JSWebAPIErrorFactory.h>

#include <glib.h>

#include "PlatformException.h"
#include "Commons/Exception.h"


namespace DeviceAPI {
namespace Filesystem {
namespace Utils{

void toUTF8String(const std::string& fromEncoding,
                  const char* from,
                  const size_t fromLength,
                  std::string &outputString);

class ScopedExecution
{
public:
    ScopedExecution(std::function<void()> aRoutine):
        mRountine(aRoutine)
    {}
    ~ScopedExecution() {mRountine();}
private:
    std::function<void()> mRountine;
};

class FilesystemEvent : public Common::CallbackUserData
{
public:
    FilesystemEvent(JSContextRef aGlobalCtx):
        CallbackUserData(aGlobalCtx),
        mCode(WrtDeviceApis::Commons::ExceptionCodes::None),
        m_synchronous(false),
        m_is_error(false)
    {}
    void setException(std::string aName, std::string aMessage = "")
    {
        mExceptionName = aName;
        mExceptionMessage = aMessage;
    }
    WrtDeviceApis::Commons::ExceptionCodes::Enumeration getExceptionCode()
    {
        return mCode;
    }
    void setExceptionCode(WrtDeviceApis::Commons::ExceptionCodes::Enumeration
                          aCode)
    {
        mCode = aCode;
    }
    std::string getExceptionName()
    {
        return mExceptionName;
    }
    std::string getExceptionMessage()
    {
        return mExceptionMessage;
    }
    void setJSError(JSValueRef aJSError)
    {
        m_JSError = aJSError;
    }
    void internalCallErrorCallback()
    {
        LOGE("enter");
        callErrorCallback(
                DeviceAPI::Common::JSWebAPIErrorFactory::makeErrorObject(
                        getContext(),
                        getExceptionName(),
                        getExceptionMessage()));
        LOGE("enter");
    }
    void setForSynchronousCall()
    {
        m_synchronous = true;
    }
    void setForAsynchronousCall()
    {
        m_synchronous = false;
    }
    bool isSynchronous()
    {
        return m_synchronous;
    }

    void setIsError(bool val){
        m_is_error = val;
    }

    bool isError(){
        return m_is_error;
    }

private:
    std::string mExceptionName;
    std::string mExceptionMessage;
    WrtDeviceApis::Commons::ExceptionCodes::Enumeration mCode;
    JSValueRef m_JSError;
    bool m_synchronous;
    bool m_is_error;
};


template <class EventObjectPtr>
class MainLoop
{
public:
    static JSValueRef passErrorLater(std::string errorName,
                                     std::string errorMessage,
                                     JSValueRef result,
                                     EventObjectPtr event)
    {
        auto eventData = new EventObjectPtr(event);
        event->setException(errorName, errorMessage);
        if (!g_idle_add(errorWorker, static_cast<gpointer>(eventData))) {
            LOGE("g_idle addition failed");
        }
        return result;
    }
private:
    static gboolean errorWorker(gpointer aData)
    {
        try {
            auto event = static_cast<EventObjectPtr*>(aData);
            (*event)->internalCallErrorCallback();
            delete event;
        } catch (...) {
            LOGE("Unknown exception caught");
        }
        return false;
    }
};

enum NoFunction {
    ENoFunction
};

template <class EventPtr>
class MultiThreadHandler
{
public:
    typedef std::function<void(EventPtr)> Routine;
    static void execute(Routine aBeginFunction,
                        Routine aThreadFunction,
                        Routine aEndFunction,
                        EventPtr aPassedEvent,
                        bool aEmptyWorkerThread = false)
    {
        try {
            aBeginFunction(aPassedEvent);
            if (aPassedEvent->isSynchronous()) {
                if (!aEmptyWorkerThread) {
                    aThreadFunction(aPassedEvent);
                }
                aEndFunction(aPassedEvent);
            }
        } catch (const WrtDeviceApis::Commons::Exception& ex) {
            LOGW("Exception caught:%s:%s",
                 ex.GetClassName().c_str(),
                 ex.GetMessage().c_str());
            aPassedEvent->setException(ex.GetClassName(), ex.GetMessage());
        } catch (const DeviceAPI::Common::BasePlatformException& ex) {
            LOGW("Exception caught %s:%s",
                 ex.getName().c_str(),
                 ex.getMessage().c_str());
            aPassedEvent->setException(ex.getName(), ex.getMessage());
        }
        if (!aPassedEvent->isSynchronous()) {
            auto handler = new MultiThreadHandler<EventPtr>(aThreadFunction,
                                                            aEndFunction,
                                                            aPassedEvent);
            if (aPassedEvent->getExceptionName().empty() && !aEmptyWorkerThread) {
                pthread_t thread;
                //Handler ownership is passed to threadWorker
                if(!pthread_create(
                        &thread, NULL, MultiThreadHandler<EventPtr>::threadWorker, handler)) {
                    if(pthread_detach(thread)) {
                        LOGE("Pthread detach error");
                    }
                } else {
                    LOGE("Pthread create error");
                }
            } else {
                //aData ownership is passed to mainWorker
                g_idle_add(mainWorker, handler);
            }
        }
    }
    static void noOperation(EventPtr /*aEvent*/){ }
private:
    Routine m_threadFunction;
    Routine m_endFunction;
    EventPtr m_event;
    MultiThreadHandler(Routine aThreadFunction,
                       Routine aEndFunction,
                       EventPtr aPassedEvent):
        m_threadFunction(aThreadFunction),
        m_endFunction(aEndFunction),
        m_event(aPassedEvent)
    {}
    static void* threadWorker(void* aEvent)
    {
        try {
            if (!aEvent) {
               LOGE("NULL pointer received as event data");
               return NULL;
            }
            auto handler = static_cast<MultiThreadHandler<EventPtr>*>(aEvent);
            auto event = handler->m_event;
            if (event->getExceptionName().empty()) {
                try {
                    handler->m_threadFunction(event);
                } catch (const WrtDeviceApis::Commons::Exception& ex) {
                    LOGW("Exception caught: %s", ex.GetMessage().c_str());
                    event->setException(ex.GetClassName(), ex.GetMessage());
                } catch (const DeviceAPI::Common::BasePlatformException& ex) {
                    LOGW("Exception caught: %s", ex.getMessage().c_str());
                    event->setException(ex.getName(), ex.getMessage());
                } catch (...) {
                    LOGW("Unknown exception caught and passed to JS layer");
                    DeviceAPI::Common::IOException ex("Unknown exception caught");
                    event->setException(ex.getName(), ex.getMessage());
                }
            }
            //aData ownership is passed to mainWorker
            g_idle_add(mainWorker, handler);
        } catch (...) {
            LOGW("Unknown exception caught and ignored in Filesystem worker thread");
        }
        return NULL;
    }
    static gboolean mainWorker(gpointer aData)
    {
        try
        {
            auto handler = static_cast<MultiThreadHandler<EventPtr>*>(aData);
            auto event = handler->m_event;
            if (!handler) {
                LOGE("NULL pointer passed");
                return false;
            }
            if (!event->getExceptionName().empty()) {
                LOGW("IO error reported to JavaScript Error:[%s] Message:[%s]",
                      event->getExceptionName().c_str(),
                      event->getExceptionMessage().c_str());
                event->internalCallErrorCallback();
                LOGE("enter");
            } else {
                try {
                    handler->m_endFunction(event);
                } catch (const WrtDeviceApis::Commons::Exception& ex) {
                    LOGW("Exception caught: %s", ex.GetMessage().c_str());
                    event->setException(ex.GetClassName(), ex.GetMessage());
                    event->internalCallErrorCallback();
                } catch (const DeviceAPI::Common::BasePlatformException& ex) {
                    LOGW("Exception caught: %s", ex.getMessage().c_str());
                    event->setException(ex.getName(), ex.getMessage());
                    event->internalCallErrorCallback();
                } catch (...) {
                    LOGW("Unknown exception caught and passed to JS layer");
                    DeviceAPI::Common::IOException ex("Unknown exception caught");
                    event->setException(ex.getName(), ex.getMessage());
                    event->internalCallErrorCallback();
                }
            }
            delete handler;
        } catch (...) {
            LOGW("Unknown error ignored in Filesystem Main Thread");
        }
        return false;
    }
};

template <class EventPtr>
void executeThreadAndMainFunctions(
        EventPtr aPassedEvent,
        typename MultiThreadHandler<EventPtr>::Routine aBeginFunction,
        typename MultiThreadHandler<EventPtr>::Routine aThreadFunction,
        typename MultiThreadHandler<EventPtr>::Routine aEndFunction)
{
    MultiThreadHandler<EventPtr>::execute(aBeginFunction,
                                          aThreadFunction,
                                          aEndFunction,
                                          aPassedEvent,
                                          false);
}

template <class EventPtr>
void executeThreadAndMainFunctions(
        EventPtr aPassedEvent,
        NoFunction /*aBeginFunction*/,
        typename MultiThreadHandler<EventPtr>::Routine aThreadFunction,
        typename MultiThreadHandler<EventPtr>::Routine aEndFunction)
{
    MultiThreadHandler<EventPtr>::execute(
            MultiThreadHandler<EventPtr>::noOperation,
            aThreadFunction,
            aEndFunction,
            aPassedEvent,
            false);
}

template <class EventPtr>
void executeThreadAndMainFunctions(
        EventPtr aPassedEvent,
        NoFunction /*aBeginFunction*/,
        NoFunction /*aThreadFunction*/,
        typename MultiThreadHandler<EventPtr>::Routine aEndFunction)
{
    MultiThreadHandler<EventPtr>::execute(
            MultiThreadHandler<EventPtr>::noOperation,
            MultiThreadHandler<EventPtr>::noOperation,
            aEndFunction,
            aPassedEvent,
            true);
}

template <class EventPtr>
void executeThreadAndMainFunctions(
        EventPtr aPassedEvent,
        typename MultiThreadHandler<EventPtr>::Routine aBeginFunction,
        NoFunction /*aThreadFunction*/,
        typename MultiThreadHandler<EventPtr>::Routine aEndFunction)
{
    MultiThreadHandler<EventPtr>::execute(
            aBeginFunction,
            MultiThreadHandler<EventPtr>::noOperation,
            aEndFunction,
            aPassedEvent,
            true);
}

} //Utils namespace
} //Filesystem namespace
} //DeviceAPI namespace

#endif  //ifndef TIZENAPIS_FILESYSTEM_UTILS_H_
