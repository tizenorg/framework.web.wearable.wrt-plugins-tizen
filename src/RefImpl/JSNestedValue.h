#ifndef _REFIMPL_JS_NESTEDVALUE_H_
#define _REFIMPL_JS_NESTEDVALUE_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <boost/shared_ptr.hpp>

namespace DeviceAPI {
namespace Reference {

class NestedValue;

struct NestedValueHolder{
    boost::shared_ptr<NestedValue> obj;
};

class JSNestedValue
{
  public:

    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static JSObjectRef makeJSObject(JSContextRef ctx, boost::shared_ptr<NestedValue> native);
    static boost::shared_ptr<NestedValue> getNative(JSContextRef ctx, JSValueRef value);
    static JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
  private:

    static JSClassDefinition m_jsClassInfo;
    static JSClassRef m_jsClassRef;
    static JSStaticFunction m_function[];
    static JSStaticValue m_property[];


    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
	static bool setProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception);

    static JSValueRef print(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

};

}
}

#endif /* _REFIMPL_JS_NESTEDVALUE_H_ */


