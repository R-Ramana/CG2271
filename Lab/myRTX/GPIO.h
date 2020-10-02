#ifndef GPIO_H
#define GPIO_H

#include "MKL25Z4.h"                    // Device header
#include "basic.h"

typedef enum {LOW, HIGH} value_t;

void initGPIOPort(PORT_Type *port, uint8_t pin);

GPIO_Type* getGPIO(PORT_Type* port);

void InitGPIOAll(PORT_Type *port, uint8_t pins[], uint8_t numPins);

void setPin(PORT_Type *port, uint8_t pin, value_t value);

#endif
