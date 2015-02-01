//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#ifndef _BADGE_MANAGER_H_
#define _BADGE_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include <CallbackUserData.h>
#include <JavaScriptCore/JavaScript.h>
#include <badge.h>
#include "BadgeUtil.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Badge {

typedef std::map<std::string, CallbackUserData*> BadgeCallbackMapT;

class BadgeManager
{
    public:
        long getBadgeCount(const char* appId);
        void addChangeListener(BadgeCallbackMapT& badgeCallback);
        void removeChangeListener(std::vector<std::string>& appIdList);
        bool checkPermisionForCreatingBadge(const char* appId);

        static BadgeManager* getInstance();

        // internal function
        void setCallbackToMap(std::string appId, CallbackUserData *callback);
        CallbackUserData* getCallbackFromMap(std::string appId);
        void removeCallbackFromMap(std::string appId);

    private:
        BadgeManager();
        virtual ~BadgeManager();

        char* _badge_get_pkgname_by_appid(const char* appId);
        int _badge_is_same_certinfo(const char *caller, const char *pkgname);
        char* _badge_get_pkgname_by_pid();

        BadgeCallbackMapT mBadgeCallbacks;

};

}
}

#endif


