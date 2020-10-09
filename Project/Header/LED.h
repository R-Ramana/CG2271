#ifndef LED_H
#define LED_H

#include "MKL25Z4.h"                    // Device header
#include "GPIO.h"

#define RED_LED 18    // PortB Pin 18
#define GREEN_LED 19  // PortB Pin 19
#define BLUE_LED 1    // PortD Pin 1

typedef enum {RED, GREEN, BLUE} color_t;
typedef enum {LED_OFF, LED_ON} led_t;

void InitLEDGPIO(void);

void offRGB(void);

void ledControl(color_t color, led_t sw);

#endif
