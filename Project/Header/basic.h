#ifndef BASIC_H
#define BASIC_H

#include "MKL25Z4.h"                    // Device header

#define MASK(x) (1 << (x))

uint32_t getMaskPort(PORT_Type *port);

/* Delay Function */
void delay(volatile uint32_t nof);

void delay2(volatile uint32_t nof);

#endif
