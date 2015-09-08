#include <stdio.h>
#include <string.h>
#include <JavaScriptCore/JavaScript.h>
#include "StandaloneConsole.h"
#include <Ecore.h>
#include <GlobalContextManager.h>

#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <pthread.h>
#include <JSRefImplManager.h>
#include <JSSub.h>
#include <JSSuper.h>
#include <JSNestedValue.h>

using namespace std;
using namespace DeviceAPI::Test;
using namespace DeviceAPI::Common;
using namespace DeviceAPI::Reference;
using namespace WrtDeviceApis::Commons;


void fun1(void *data, Ecore_Thread *thread){
    printf("fun1 %p\n", pthread_self());
}
void fun2(void *data, Ecore_Thread *thread){
    printf("fun2 %p\n", pthread_self());
}

static Eina_Bool fun3(void *data){
    ecore_thread_run(fun1, fun2, NULL, NULL);
    return false;
}


void loopQuit(void *data, Ecore_Thread *thread){
    ecore_main_loop_quit();
}

void threadRun(void *data, Ecore_Thread *thread){
    StandaloneConsole *console = (StandaloneConsole*)data;
    StandaloneConsole::commandline(console);

}

int main(int argc, char** argv){

    if (!ecore_init())
    {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
    }
    ecore_main_loop_glib_integrate();

    DPL::Log::LogSystemSingleton::Instance().SetTag("STANDALONE");
    //DPL::Event::GetMainEventDispatcherInstance().ResetCrossEventCallHandler();
    WrtAccessSingleton::Instance().initialize(0);


    StandaloneConsole console;
    console.initialize();
    GlobalContextManager::getInstance()->addGlobalContext(console.getGlobalContext());
    console.registModule("ref", JSRefImplManager::getClassRef(), NULL);
    JSObjectRef Sub = JSObjectMakeConstructor(console.getGlobalContext(), JSSub::getClassRef(), JSSub::constructor);
    JSObjectRef Super = JSObjectMakeConstructor(console.getGlobalContext(), JSSuper::getClassRef(), JSSuper::constructor);
    JSObjectRef Nested = JSObjectMakeConstructor(console.getGlobalContext(), JSNestedValue::getClassRef(), JSNestedValue::constructor);
    console.appendModule("Sub", Sub);
    console.appendModule("Super", Super);
    console.appendModule("NestedValue", Nested);

    if( argc > 1 ){
        console.RunScript(argv[1]);
    }
    printf("main thread = %p\n", pthread_self());
    ecore_thread_run(threadRun, loopQuit, NULL, &console);

    ecore_main_loop_begin();
    ecore_shutdown();
    return 0;
}
