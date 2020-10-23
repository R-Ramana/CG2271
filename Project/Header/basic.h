#ifndef BASIC_H
#define BASIC_H

#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define MASK(x) (1 << (x))

extern osSemaphoreId_t endSem;
extern osSemaphoreId_t brainSem;
extern osSemaphoreId_t musicSem;
extern osSemaphoreId_t moveSem;

uint32_t getMaskPort(PORT_Type *port);

/* Delay Function */
void delay(volatile uint32_t nof);

void delay2(volatile uint32_t nof);

#endif
