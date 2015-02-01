#include "Sub.h"
#include <stdio.h>
Sub::Sub(){
    printf("Sub C++ constructor\n");
}

Sub::~Sub(){
    printf("Sub C++ destroy\n");
}

void Sub::extend(){
    printf("Sub::extend()\n");    
}

void Sub::override(){
    printf("Sub::override()\n");
}
