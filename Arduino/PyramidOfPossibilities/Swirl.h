//
//  Swirl.h
//
//  Created by Bill on 7/18/14.
//  Copyright (c) 2014 RN. All rights reserved.
//

#ifndef __Swirl__
#define __Swirl__

#include "RNAnimation.h"
#include "RNGradient.h"

struct SwirlParameters {
    RNGradient headGradient = RNGradient(1, RNGradientBounce,
                            0x90ffff, 0xffffff);
    RNGradient tailGradient = RNGradient(1, RNGradientBounce,
                            0x90ff00, 0xffff00);
    
    float lengthFraction = 0.7;
    
    uint16_t rpm = 30;
    uint16_t gpm = 10;
    
    bool global = 0;
};


class Swirl : public RNAnimation {
public:
    Swirl(RNInfo & info, unsigned long animationStartMillis)
    : RNAnimation(info, animationStartMillis, sizeof(SwirlParameters), &parameters) {
      
       };
    virtual void paint(RNLights & lights);
    
    virtual const char * name();
    
    SwirlParameters parameters;
   


};

#endif /* defined(__Swirl__) */
