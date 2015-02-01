#include "JSSuper.h"
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include "Super.h"

using namespace DeviceAPI::Common;



namespace DeviceAPI {
namespace Reference {

JSClassRef JSSuper::m_jsClassRef = NULL;

JSClassDefinition JSSuper::m_jsClassInfo = {
    0, // current (and only) version is 0
    kJSClassAttributeNone, //attributes
    "Super", //class name
    NULL, // parent class
    NULL, //static values
    JSSuper::m_function, // static functions
    JSSuper::initialize, // initialize
    JSSuper::finalize, //finalize
    NULL, //hasProperty
    NULL, //getProperty
    NULL, //setProperty
    NULL, //deleteProperty
    NULL, //getPropertyNames
    NULL, // callAsFunction
    NULL, // constructor
    NULL, //hasInstance
    NULL // convertToType
};


JSStaticFunction JSSuper::m_function[] = {
    { "base", JSSuper::base, kJSPropertyAttributeNone },
    { "override", JSSuper::override, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSSuper::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSuper::getClassInfo()
{
    return &m_jsClassInfo;
}

void JSSuper::initialize(JSContextRef ctx, JSObjectRef object)
{
    printf("JSSuper::initialize()\n");
}

void JSSuper::finalize(JSObjectRef object)
{
    printf("JSSuper::finalize()\n");
    Super * priv = static_cast<Super*>(JSObjectGetPrivate(object));
    if( priv ){
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSObjectRef JSSuper::constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSSuper::constructor()\n");
    Super * priv = new Super();
    JSObjectRef obj = JSObjectMake(ctx, getClassRef(), priv);
    JSUtil::setProperty(ctx, obj, "constructor", constructor, kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete|kJSPropertyAttributeDontEnum);
    return obj;
}

JSValueRef JSSuper::base(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSSuper::base()\n");
    Super * priv = static_cast<Super*>(JSObjectGetPrivate(thisObject));
    if( priv )
        priv->base();
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSSuper::override(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSSuper::override()\n");
    Super * priv = static_cast<Super*>(JSObjectGetPrivate(thisObject));
    priv->override();
    return JSValueMakeUndefined(ctx);
}



} // Reference
} // TizenApis

