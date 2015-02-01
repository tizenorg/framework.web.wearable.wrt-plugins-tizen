#ifndef _JS_SUB_H_
#define _JS_SUB_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Reference {

class JSSub
{
  public:

    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

  private:

    static JSClassDefinition m_jsClassInfo;
    static JSClassRef m_jsClassRef;
    static JSStaticFunction m_function[];


    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);

    static JSValueRef extend(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

};

}
}

#endif /* _JS_SUB_H_ */



