#ifndef GPIO_H
#define GPIO_H

#include "MKL25Z4.h"                    // Device header
#include "basic.h"

void initGPIOPort(PORT_Type *port, uint8_t pin);

GPIO_Type* getGPIO(PORT_Type* port);

void InitGPIOAll(PORT_Type *port, uint8_t pins[], uint8_t numPins);

#endif
