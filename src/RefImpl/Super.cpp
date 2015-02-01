#include "Super.h"
#include <stdio.h>


Super::Super(){
    printf("Super c++ constructor\n");
}
Super::~Super(){
    printf("Super c++ destroy\n");
}

void Super::base(){
    printf("Super::base()\n");
}

void Super::override(){
    printf("Super::override()\n");
}
