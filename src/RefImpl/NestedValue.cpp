#include "NestedValue.h"

using namespace std;

namespace DeviceAPI {
namespace Reference {

NestedValue::NestedValue():mNumber(0){
}

NestedValue::NestedValue(long number, const string &message ):mNumber(number), mMessage(message){
}

NestedValue::~NestedValue(){
}

long NestedValue::getNumber() const{
    return mNumber;
}

const string& NestedValue::getMessage() const{
    return mMessage;
}

void NestedValue::setNumber(long v){
    mNumber = v;
}

void NestedValue::setMessage(const  string & msg){
    mMessage = msg;
}

void NestedValue::print(){
    printf("%d)%s\n", mNumber, mMessage.c_str());
}

}
}
