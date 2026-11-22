#ifndef LAMP_ANIMATION_API_H
#define LAMP_ANIMATION_API_H

#include <stdint.h>

// This provides a way for other files besdes the main .ino file to call the ShowLampAnimation function, which is defined in LampAnimations.h
void ShowLampAnimation(uint8_t animationNum,
                       uint32_t divisor,
                       uint32_t baseTime,
                       uint8_t subOffset,
                       bool dim,
                       bool reverse = false,
                       uint8_t keepLampOn = 99);

void ShowLampAnimationSingleStep(uint8_t animationNum,
                                 uint8_t currentStep,
                                 uint8_t *lampsToAvoid = NULL);

#endif
