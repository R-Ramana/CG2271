#include "MKL25Z4.h"                    // Device header
#include "basic.h"

osSemaphoreId_t endSem, brainSem, musicSem, moveSem;


uint32_t getMaskPort(PORT_Type *port) {
  if (port == PORTA)
    return SIM_SCGC5_PORTA_MASK;
  else if (port == PORTB)
    return SIM_SCGC5_PORTB_MASK;
  else if (port == PORTC)
    return SIM_SCGC5_PORTC_MASK;
  else if (port == PORTD)
    return SIM_SCGC5_PORTD_MASK;
  else
    return SIM_SCGC5_PORTE_MASK;
}

/* Delay Function */
void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void delay2(volatile uint32_t nof) {
  for(int i = 0; i < 100; i++) {
    delay(nof);
  }
}
