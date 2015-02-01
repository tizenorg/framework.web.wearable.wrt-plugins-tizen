/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#ifndef _REFIMPLMANAGER_
#define _REFIMPLMANAGER_

#include <string>
#include <stdio.h>
#include <CallbackUserData.h>
#include <MultiCallbackUserData.h>
#include <boost/shared_ptr.hpp>
#include <map>
#include <PropertyBag.h>

namespace DeviceAPI {
namespace Reference{

class RefCallbackUserData : public Common::CallbackUserData{
    public:
        RefCallbackUserData(JSContextRef globalCtx, int aa, int bb):CallbackUserData(globalCtx), a(aa), b(bb), result(0){}
        int a;
        int b;
        int result;
};

class RefImplManager{
    public:
        int syncToSync(int a, int b);
        void syncToAsync(RefCallbackUserData *callback);
        void asyncToAsync(RefCallbackUserData *callback);

        //for multiple callback
        int addListener(boost::shared_ptr<Common::CallbackUserData> callback);
        void removeListener(int id);
        void fireListener();

        // for single callbcak object per one global context
        void setCallbackObject( JSContextRef context, boost::shared_ptr<Common::MultiCallbackUserData> callback);
        void unsetCallbackObject( JSContextRef context );
        void namedCallbackFire(JSContextRef context, const char *callbackName);
        
        static RefImplManager* getInstance();
        Common::PropertyBag mLocalProperty;

    private:
        int mCounter;
        typedef std::pair<int, boost::shared_ptr<Common::CallbackUserData>> listenerT;
        std::map<int, boost::shared_ptr<Common::CallbackUserData>> mListenerList;
        typedef std::pair<JSContextRef, boost::shared_ptr<Common::MultiCallbackUserData>> callbackObjectT;
        std::map<JSContextRef, boost::shared_ptr<Common::MultiCallbackUserData>> mCallbackList;

        RefImplManager();
        ~RefImplManager();
        static int onFire(void* user_data);
        static int onNamedFire(void* user_data);
        void broadCast();
        void namedBroadCast(JSContextRef context, const char *callbackName);
        
};
    


}
}

#endif //_REFIMPLMANAGER_

