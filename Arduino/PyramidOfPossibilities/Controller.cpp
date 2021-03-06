//
//  Controller.cpp
//  PlatformData
//
//  Created by Bill on 7/18/14.
//  Copyright (c) 2014 RN. All rights reserved.
//

#include "Controller.h"
#include "RNInfo.h"
#include "Arduino.h"
#include "Animations.h"
#include <malloc.h>
#include "Constants.h"



static int heapSize(){
    return mallinfo().uordblks;
}

void RNController::switchToAnimation(AnimationEnum nextAnimation) {
    if (currentAnimation) {
        info.printf("Deleting old animation\n");
        int before = heapSize();
        delete currentAnimation;
        int after = heapSize();
        info.printf("%d bytes freed\n", before-after);
    }
    currentAnimationEnum = nextAnimation;
    if (currentAnimationEnum >= e_AnimationCount)
        currentAnimationEnum = (AnimationEnum) 0;
    info.printf("Switching to animation %d\n", currentAnimationEnum);
    unsigned long start = millis();
    int before = heapSize();
    currentAnimation = getAnimation(currentAnimationEnum, info, start);
    int after = heapSize();
    info.printf("%d bytes allocated\n",after-before);

    // See Constants.h
    animationExpires = start + constants.animationDuration;

    if (currentAnimation)
      info.printf("Created %s\n", currentAnimation->name());
}

void RNController::nextAnimation() {
  switchToAnimation((AnimationEnum) (1+((int)currentAnimationEnum)));
}

void RNController::paint(RNLights & lights) {
    info.update();
    if (millis() > animationExpires || !currentAnimation) {
        nextAnimation();
    }
    
#ifndef POP_SIMULATOR
    if (Serial2.available() > 1) {
        int a = Serial2.read();
        if (a == 'a') {
            int c = Serial2.read();
            info.printf("Got %d from serial2\n", c);
            if (c >= 0 && c < e_AnimationCount)
                switchToAnimation((AnimationEnum) c);
        }
    }
#endif
    
    if (currentAnimation)
        currentAnimation->paint(lights);
    
    
}




