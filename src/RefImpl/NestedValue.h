#ifndef _REFIMPL_NESTEDVALUE_H_
#define _REFIMPL_NESTEDVALUE_H_

#include <string>

namespace DeviceAPI {
namespace Reference {

class NestedValue{
public:
    NestedValue();
    NestedValue(long number, const std::string &message);
    virtual ~NestedValue();
    long getNumber() const;
    const std::string& getMessage() const;

    void setNumber(long v);
    void setMessage(const std::string &msg);
    void print();

private:
    long mNumber;
    std::string mMessage;
    
};

}
}

#endif //_REFIMPL_NESTEDVALUE_H_
