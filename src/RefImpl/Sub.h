#ifndef _SUB_H_
#define _SUB_H_

#include "Super.h"

class Sub : public Super {
public:
    Sub();
    virtual ~Sub();
    void extend();
    virtual void override();
};

#endif //_SUB_H_

