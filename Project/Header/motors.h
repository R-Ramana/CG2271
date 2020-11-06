#ifndef MOTORS_H
#define MOTORS_H
#include "PWM.h"

#define NUM_PINS_D  6
#define PTB_Pin(x)  x
#define ALT_D       4

#define NUM_PINS_A  2
#define PTA_Pin(x)  x
#define ALT_A       3

#define SW_POS      6

typedef enum {LFF = 0, LFR, RFF, RFR, LBF, LBR, RBF, RBR} wheel_t;


/* Initialize PWM */
void initPWMMotors(void);

void controlWheel(wheel_t, uint16_t);

void stop(void);

void move(uint8_t);

#endif
