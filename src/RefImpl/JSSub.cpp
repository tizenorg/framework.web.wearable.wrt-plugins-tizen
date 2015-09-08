#include "JSSuper.h"
#include "JSSub.h"

#include <ArgumentValidator.h>
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include "Sub.h"

using namespace DeviceAPI::Common;


namespace DeviceAPI {
namespace Reference {

JSClassRef JSSub::m_jsClassRef = NULL;

JSClassDefinition JSSub::m_jsClassInfo = {
    0, // current (and only) version is 0
    kJSClassAttributeNone, //attributes
    "Sub", //class name
    JSSuper::getClassRef(), // parent class
    NULL, //static values
    JSSub::m_function, // static functions
    JSSub::initialize, // initialize
    JSSub::finalize, //finalize
    NULL, //hasProperty
    NULL, //getProperty
    NULL, //setProperty
    NULL, //deleteProperty
    NULL, //getPropertyNames
    NULL, // callAsFunction
    NULL, // constructor
    NULL,
    NULL // convertToType
};


JSStaticFunction JSSub::m_function[] = {
    { "extend", JSSub::extend, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};


const JSClassRef JSSub::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}
const JSClassDefinition* JSSub::getClassInfo()
{
    return &m_jsClassInfo;
}

JSObjectRef JSSub::constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSSub::constructor()\n");
    ArgumentValidator validator(ctx, argumentCount, arguments);
    try{
        int a = validator.toLong(0);
        if( a == 2)
            throw InvalidValuesException("2 is invalid value");
    }catch(const BasePlatformException& err){
        JSObjectRef error = JSWebAPIErrorFactory::makeErrorObject(ctx, err);
        *exception = error;
        return error;
    }
    Sub *priv = new Sub();
    JSObjectRef obj = JSObjectMake(ctx, getClassRef(), priv);
    JSUtil::setProperty(ctx, obj, "constructor", constructor, kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete|kJSPropertyAttributeDontEnum);
    return obj;
}

void JSSub::initialize(JSContextRef ctx, JSObjectRef object)
{
    printf("JSSub::initialize()\n");
}

void JSSub::finalize(JSObjectRef object)
{
    printf("JSSub::finalize()\n");
    Sub * priv = static_cast<Sub*>(JSObjectGetPrivate(object));
    if( priv ){
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSSub::extend(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    printf("JSSub::extend()\n");
    Sub * priv = static_cast<Sub*>(JSObjectGetPrivate(thisObject));
    if( priv )
        priv->extend();
    return JSValueMakeUndefined(ctx);
}


} // Reference
} // TizenApis

