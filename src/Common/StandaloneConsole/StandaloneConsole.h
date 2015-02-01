//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#ifndef _TIZEN_COMMON_STANDALONECONSOLE_
#define _TIZEN_COMMON_STANDALONECONSOLE_

#include <Ecore.h>
#include <map>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Test{

class StandaloneConsole{
    public :
        StandaloneConsole();
        virtual ~StandaloneConsole();
        void initialize();
        JSObjectRef getGlobalObject();
        JSContextRef getGlobalContext();

        void RunLine(const char *line);
        void RunScript(const char * path);

        void GarbageCollect();


        JSValueRef RunLineEx( const char *line , JSValueRef *exception);
        JSValueRef RunScriptEx( const char *path , JSValueRef *exception);

        JSObjectRef registModule( const char *name, JSClassRef module, void *priv);
        void appendModule( const char* name, JSObjectRef module );

        static JSValueRef alert(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
        static JSValueRef setInterval(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
        static JSValueRef clearInterval(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

        static JSValueRef setTimeout(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

        static void commandline(StandaloneConsole *console);

    private:
        static Eina_Bool timerCb(void *data);
        void reportingResult(JSValueRef v, JSValueRef exception);

        JSContextRef mGlobalContext;
        JSObjectRef mGlobalObject;
        std::map<int,int> mTimerMap;
};

}
}

#endif //_TIZEN_COMMON_STANDALONECONSOLE_
