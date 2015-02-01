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

#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

#include <string>
#include <list>
#include <CallbackUserData.h>
#include <JavaScriptCore/JavaScript.h>
#include <device/callback.h>

namespace DeviceAPI {
namespace Power {

class PowerResource{
public:
    enum type { SCREEN=0, CPU=1 };
    PowerResource( const char * str );
    operator type() const {return mType;}
    const std::string toString();
    type mType;
};

class PowerState{
public:
    enum state { SCREENOFF, SCREENDIM, SCREENNORMAL, SCREENBRIGHT, CPUAWAKE, NONE };
    PowerState( const char * str );
    PowerState( state in);
    operator state() const {return mState;}
    const std::string toString();
    state mState;
};



class PowerManager
{
public:
    void request( PowerResource resource, PowerState state );
    void release( PowerResource resource );
    double getScreenBrightness();
    void setScreenBrightness(double brightness);
    void restoreScreenBrightness();
    bool isScreenOn();
    void setScreenState( bool onoff );

    void addScreenStateChangedCallback(Common::CallbackUserData *callback);
    void removeScreenStateChangedCallback(Common::CallbackUserData *callback);

    static PowerManager* getInstance();
private:
    int getPlatformBrightness();
    void setPlatformBrightness(int i);
    void restoreSettedBrightness();
    static void onPlatformStateChangedCB(device_callback_e type, void *value, void *user_data);
    PowerManager();
    virtual ~PowerManager();
    void broadcastScreenState(PowerState current);
    std::list<Common::CallbackUserData*> mListener;
    PowerState mCurrentState;
    bool mBrightStateEnable;
    int mCurrentBrightness;
    bool mShouldBeReadFromCache;
    bool mSetCustomBrightness;
    PowerState mCurrentRequestedState;

};

}
}

#endif /* _POWER_MANAGER_H_ */

