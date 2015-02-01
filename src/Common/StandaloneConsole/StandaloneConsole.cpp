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

#include <stdio.h>
#include <dlog.h>
#include <JavaScriptCore/JavaScript.h>
#include "StandaloneConsole.h"
#include "JSConsole.h"
#include <Ecore.h>
#include <GlobalContextManager.h>
#include <string>
#include <vector>
#include <iostream>
#include <termios.h>
#include <JSUtil.h>

#undef LOG_TAG
#define LOG_TAG "TIZEN_DEVICEAPI"

using namespace std;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Test {

struct _Command{
    char * mLine;
    StandaloneConsole *mConsole;
    pthread_mutex_t *mLock;
    _Command(const char * cmd, StandaloneConsole *console, pthread_mutex_t* lock){
        mLine = strdup(cmd);
        mConsole = console;
        mLock = lock;
    }
    ~_Command(){
        free(mLine);
    }
    void run(){
        mConsole->RunLine(mLine);
        pthread_mutex_unlock(mLock);
    }
};

struct CallbackData{
    JSObjectRef callback;
    int id;
    StandaloneConsole *console;
};

static Eina_Bool tick(void *data){
    return true;
}

static Eina_Bool commandDispath(void *data){
    _Command *cmd = (_Command*)data;
    cmd->run();
    delete cmd;
    return false;
}

Eina_Bool StandaloneConsole::timerCb(void *data){
    CallbackData *callback = (CallbackData*)data;
    StandaloneConsole *console = callback->console;
    map<int,int>::iterator itr;
    itr = console->mTimerMap.find(callback->id);
    if( itr == console->mTimerMap.end() ){
        JSValueUnprotect(console->getGlobalContext(), callback->callback);
        delete callback;
        return false;
    }
    if( itr->second == 0){
        console->mTimerMap.erase(itr);
        JSValueUnprotect(console->getGlobalContext(), callback->callback);
        delete callback;
        return false;
    }
    if( callback->callback != NULL){
        JSObjectCallAsFunction(console->getGlobalContext(), callback->callback, NULL, 0, 0, NULL);
    }

    if( itr->second == 2 ){
        console->mTimerMap.erase(itr);
        JSValueUnprotect(console->getGlobalContext(), callback->callback);
        delete callback;
        return false;
    }

    return true;
}


JSValueRef StandaloneConsole::alert(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    if( argumentCount < 1 )
        return JSValueMakeUndefined(ctx);


    //JSContextRef globalCtx = GlobalContextManager::getInstance()->getGlobalContext(ctx);
    //printf(" local : %p, global : %p \n", ctx, globalCtx);

    JSStringRef str = JSValueToStringCopy(ctx, arguments[0], NULL);
    if(str == NULL){
        return JSValueMakeUndefined(ctx);
    }
    int n = JSStringGetLength(str);
    {
        char cstr[n+1];
        JSStringGetUTF8CString(str, cstr,n+1);
        printf("<alert>%s\n", cstr);
    }
    return JSValueMakeUndefined(ctx);
}

JSValueRef StandaloneConsole::setInterval(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    static int id = 0;
    StandaloneConsole *console = static_cast<StandaloneConsole*>(JSObjectGetPrivate(thisObject));
    if( argumentCount < 2 ){
        if( exception != NULL){

        }
        return JSValueMakeUndefined(ctx);
    }
    int handleid = id++;
    double interval = JSValueToNumber(ctx, arguments[1], NULL);
    interval = interval/1000;

    console->mTimerMap.insert(pair<int,int>(handleid, 1));
    CallbackData *data = new CallbackData();
    JSValueProtect(console->getGlobalContext(), arguments[0]);
    data->callback = JSValueToObject(ctx, arguments[0], NULL);
    data->id = handleid;
    data->console = console;

    ecore_timer_add( interval, StandaloneConsole::timerCb , data);
    return JSValueMakeNumber(ctx, handleid);

}

JSValueRef StandaloneConsole::setTimeout(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    static int id = 0;
    StandaloneConsole *console = static_cast<StandaloneConsole*>(JSObjectGetPrivate(thisObject));

    if( argumentCount < 2 ){
        if( exception != NULL){

        }
        return JSValueMakeUndefined(ctx);
    }
    int handleid = id++;
    double interval = JSValueToNumber(ctx, arguments[1], NULL);
    interval = interval/1000;

    console->mTimerMap.insert(pair<int,int>(handleid, 2));
    CallbackData *data = new CallbackData();
    JSValueProtect(console->getGlobalContext(), arguments[0]);
    data->callback = JSValueToObject(ctx, arguments[0], NULL);
    data->id = handleid;
    data->console = console;

    ecore_timer_add( interval, StandaloneConsole::timerCb , data);
    return JSValueMakeNumber(ctx, handleid);

}


JSValueRef StandaloneConsole::clearInterval(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    StandaloneConsole *console = static_cast<StandaloneConsole*>(JSObjectGetPrivate(thisObject));
    if(console == NULL) return JSValueMakeUndefined(ctx);
    if( argumentCount < 1 ){
        printf("error clearInterval\n");
        if( exception != NULL){

        }
        return JSValueMakeUndefined(ctx);
    }

    int handleid = JSValueToNumber(ctx, arguments[0], NULL);
    map<int,int>::iterator it;
    it = console->mTimerMap.find(handleid);
    if( it != console->mTimerMap.end())
        console->mTimerMap[handleid] = 0;
    return JSValueMakeUndefined(ctx);
}


static JSValueRef test(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    return JSValueMakeUndefined(ctx);
}


static void setProperty(JSContextRef ctx , JSObjectRef object, const char *name, JSValueRef value, JSPropertyAttributes attributes)
{
	JSStringRef propertyName = JSStringCreateWithUTF8CString(name);
	JSObjectSetProperty(ctx, object, propertyName, value,attributes, NULL );
	JSStringRelease(propertyName);
}

static JSValueRef getProperty(JSContextRef ctx , JSObjectRef object, const char *name){
	JSValueRef value;
	JSStringRef propertyName = JSStringCreateWithUTF8CString(name);
	value = JSObjectGetProperty(ctx, object, propertyName, NULL);
	JSStringRelease(propertyName);
	return value;
}

static char * toString(JSContextRef ctx , JSValueRef jsV){
    JSValueRef exception = NULL;
    JSStringRef jsStr = JSValueToStringCopy(ctx, jsV, &exception);
    if( exception != NULL )
        return NULL;
    int n = JSStringGetMaximumUTF8CStringSize(jsStr);
    char *buf = new char[n+1];
    JSStringGetUTF8CString(jsStr, buf, n+1);
    JSStringRelease(jsStr);
    return buf;
}

StandaloneConsole::StandaloneConsole():mGlobalContext(NULL),mGlobalObject(NULL){
}

StandaloneConsole::~StandaloneConsole(){
}

void StandaloneConsole::initialize(){
    // Function table
    JSStaticFunction functions[] = {
        { "alert", StandaloneConsole::alert , kJSPropertyAttributeNone },
        { "setInterval", StandaloneConsole::setInterval , kJSPropertyAttributeNone },
        { "setTimeout", StandaloneConsole::setTimeout , kJSPropertyAttributeNone },
        { "clearInterval", StandaloneConsole::clearInterval , kJSPropertyAttributeNone },
        { "clearTimeout", StandaloneConsole::clearInterval , kJSPropertyAttributeNone },
        { "test", test, kJSPropertyAttributeNone },
        { 0, 0, 0 }
    };

    // Global class
    JSClassDefinition def = {
        0,                                 // current (and only) version is 0
        kJSClassAttributeNone,   //attributes
        "global",                        //class name
        NULL,                            // parent class
        NULL,                            //static values
        functions,                      // static functions
        NULL,                            // initialize
        NULL,                            //finalize
        NULL,                            //hasProperty
        NULL,                            //getProperty
        NULL,                            //setProperty
        NULL,                            //deleteProperty
        NULL,                            //getPropertyNames
        NULL,                            // callAsConstructor
        NULL,                            // constructor
        NULL,
        NULL                             // convertToType
    };

    JSClassRef globalClass = JSClassCreate(&def);

    mGlobalContext = JSGlobalContextCreate(globalClass);
    mGlobalObject = JSContextGetGlobalObject(mGlobalContext);
    JSObjectSetPrivate(mGlobalObject, this);
    JSObjectRef console = JSConsole::createJSObject(mGlobalContext);
    setProperty(mGlobalContext, mGlobalObject, "console", console, kJSPropertyAttributeReadOnly);


    //is it ecore bug? event was not invoke, it was added in another thread
    ecore_timer_add(0.001, tick, NULL);
}


JSObjectRef StandaloneConsole::getGlobalObject(){
    return mGlobalObject;
}

JSContextRef StandaloneConsole::getGlobalContext(){
    return mGlobalContext;
}


JSValueRef StandaloneConsole::RunLineEx(const char* line, JSValueRef *exception){
    JSStringRef jsScript = JSStringCreateWithUTF8CString(line);
    int size = strlen(line);
    if( size != static_cast <int>(JSStringGetLength(jsScript))){
        cout <<"error - fail to converting JSStringRef"<<endl;
    }
    JSValueRef ret = JSEvaluateScript(mGlobalContext, jsScript, NULL, NULL, 0, exception);
    JSStringRelease(jsScript);
    return ret;
}

JSValueRef StandaloneConsole::RunScriptEx(const char* path, JSValueRef *exception){

    FILE* f = fopen(path, "r");
    if( f == NULL ){
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);

    if( length > 0 )
    {
        char buff[length+1];
        memset(buff, '\0', length+1);
        int r = fread(buff, 1, length, f);
        fclose(f);

        if( r != length ){
            printf("error read\n");
            return JSValueMakeUndefined(mGlobalContext);
        }
        return RunLineEx(buff, exception);
    }
    fclose(f);
    return JSValueMakeUndefined(mGlobalContext);
}

void StandaloneConsole::RunLine(const char * line){
    JSValueRef exception = NULL;
    JSValueRef v = RunLineEx(line, &exception);
    reportingResult(v,exception);
}

void StandaloneConsole::RunScript(const char * path){
    JSValueRef exception = NULL;
    JSValueRef v = RunScriptEx(path, &exception);
    reportingResult(v,exception);
}

void StandaloneConsole::GarbageCollect(){
    printf("GarbageCollect\n");
    JSGarbageCollect(mGlobalContext);
}
void StandaloneConsole::reportingResult(JSValueRef v, JSValueRef exception){
    if( exception != NULL ){
        char *errStr = toString(mGlobalContext, exception);
        if( errStr != NULL ){
            printf("< error - %s\n", errStr);
            delete[] errStr;
        }
        JSObjectRef errObj = JSValueToObject(mGlobalContext, exception, NULL);
        if( errObj != NULL ){
            JSValueRef stack = getProperty(mGlobalContext, errObj, "stack");
            char *stackStr = NULL;
            if( !JSValueIsUndefined(mGlobalContext, stack) && (stackStr = toString(mGlobalContext, stack )) != NULL){
                printf("stack:%s\n", stackStr);
                delete[] stackStr;
            }
        }
    }else{
        char *resultStr = toString(mGlobalContext, v);
        if( resultStr != NULL ){
            printf("< %s\n", resultStr);
            delete[] resultStr;
        }
    }
}

JSObjectRef StandaloneConsole::registModule(const char * name, JSClassRef module, void * priv){
    JSObjectRef obj = JSObjectMake(mGlobalContext, module, priv);
    setProperty(mGlobalContext, mGlobalObject, name, obj, kJSPropertyAttributeReadOnly);
    return obj;
}

void StandaloneConsole::appendModule(const char * name, JSObjectRef module){
    setProperty(mGlobalContext, mGlobalObject, name, module, kJSPropertyAttributeReadOnly);
}


int getch(void)
{
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

struct termios gSave;

void onExit(void)
{
    tcsetattr(0, TCSAFLUSH, &gSave);
}

class LineBuffer{
    vector<string> mHistory;
    string mLine;
    int mHistoryIndex;
    unsigned int mCurrentPos;
    unsigned int mCurrentPosTmp;
    int mLineLength;
public:
    LineBuffer():mHistoryIndex(0), mCurrentPos(0){
        tcgetattr(0, &gSave);
        atexit(onExit);
    }
    ~LineBuffer(){
        tcsetattr(0, TCSAFLUSH, &gSave);
    }

    void backSpace( int length ){
        for( int i =0 ; i < length ; i++){
            putchar('\b');
            putchar(' ');
            putchar('\b');
        }
    }

    void cleanLine(){
        int diff = mLineLength - mCurrentPosTmp;
        while( diff-- > 0 ){
            moveCursor(false);
        }
        backSpace(mLineLength);
    }

    void applyHistory( unsigned int index ){
        if( mHistory.size() > index ){
            mLine = mHistory[index];
            mCurrentPos = mLine.size();
        }
    }

    void moveCursor( bool Left ){
        putchar(27);putchar(91);
        if( Left )
            putchar(68);
        else
            putchar(67);
    }

    void moveCurrentCursorPosition(){
        int diff = mLine.size() - mCurrentPos;

        while( diff-- > 0 ){
            moveCursor(true);
        }
    }

    bool checkSpecialKeys(int a){
        if( a == 8 ){
            if( mLine.size() != 0 && mCurrentPos != 0){
                mCurrentPos--;
                mLine.erase(mCurrentPos,1);
            }
            return true;
        }
        if( a == 27 ){
            a = getch(); // 91
            a = getch();
            switch( a ){
                case 65:
                    //UP
                    if( mHistoryIndex > 0 ){
                        applyHistory(--mHistoryIndex);
                    }
                    break;
                case 66:
                    //DOWN
                    if( (unsigned)mHistoryIndex < mHistory.size() ){
                        applyHistory(++mHistoryIndex);
                    }
                    break;
                case 67:
                    //RIGHT
                    if( mCurrentPos < mLine.size())
                        mCurrentPos++;
                    break;
                case 68:
                    //LEFT
                    if( mCurrentPos > 0 )
                        mCurrentPos--;
                    break;
                case 51:
                    //delete
                    getch();
                    if( mCurrentPos < mLine.size())
                        mLine.erase(mCurrentPos,1);
                    break;
                case 52:
                    //end
                    getch();
                    mCurrentPos = mLine.size();
                    break;
                case 49:
                    //home
                    mCurrentPos = 0;
                    a = getch();
                    break;
                default:
                    a = getch();
            }

            return true;
        }
        return false;
    }

    string Prompt(const char * prompt){
        printf("%s", prompt);
        mCurrentPos = 0;
        mLine.clear();
        mLineLength = mLine.size();
        mCurrentPosTmp = mCurrentPos;
        while(1){
            int a = getch();
            cleanLine();
            if( a == 10 )
                break;

            if(!checkSpecialKeys(a)){
                mLine.insert(mCurrentPos,1, a);
                mCurrentPos++;
            }
            cout << mLine;
            moveCurrentCursorPosition();
            mLineLength = mLine.size();
            mCurrentPosTmp = mCurrentPos;
        }
        cout << mLine;
        if( mLine.size() > 0 ){
            mHistory.push_back(mLine);
            mHistoryIndex = mHistory.size();
        }
        return mLine;
    }

};



void StandaloneConsole::commandline(StandaloneConsole* console){
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    printf("command line mode ( \"quit\" for exit  )\n");
    LineBuffer linebuff;
    while(1){
        pthread_mutex_lock(&lock);
        string line = linebuff.Prompt(">");
        printf("\n");

        if( line == "quit" )
            break;
        if( line == "gc" ){
            console->GarbageCollect();
            continue;
        }
        if( line.size() == 0 )
            continue;
        _Command *cmd = new _Command(line.c_str(), console, &lock);
        // for thread safety
        ecore_idler_add(commandDispath, cmd);
    }
}


}
}

