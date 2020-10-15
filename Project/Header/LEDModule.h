#ifndef LEDMODULE_H
#define LEDMODULE_H

#include "GPIO.h"

#define NUM_LED_B 4
#define NUM_LED_E 6
#define NUM_LED_C 1

#define DELAY_LED_RUN   100
#define DELAY_LED_FLASH 500
#define DELAY_LED_1HZ   500
#define DELAY_LED_2HZ   250


void initLEDModules(void);

void offRedModule(void);

void offGreenModule(void);

void offLEDModules(void);

void onOffLED(PORT_Type *port, uint8_t pins);

void runningLED(void);

void allLitLED(void);

void flashGreenLED(uint8_t);

void flash2GreenLED(void);

void flashRedLED_1(void);

void flashRedLED_2(void);

#endif
