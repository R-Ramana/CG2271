#include "LED.h"

void InitLEDGPIO() {
  uint8_t pinsB[] = {RED_LED, GREEN_LED}, numPinsB = 2;
  uint8_t pinsD[] = {BLUE_LED}, numPinsD = 1;
  
  InitGPIOAll(PORTB, pinsB, numPinsB);
  InitGPIOAll(PORTD, pinsD, numPinsD);
}

void offRGB() {
	/* Active low, so set bits to 1 to switch off */
	PTB->PSOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PSOR = MASK(BLUE_LED);
}

void ledControl(color_t color, led_t sw) {
  // Turn on specific colour
  // Clear bit to turn on, since active low
  switch (color) {
  case RED:
    if (sw)
      PTB->PCOR |= MASK(RED_LED);
    else
      PTB->PSOR |= MASK(RED_LED);
    break;
  case GREEN:
    if (sw)
      PTB->PCOR |= MASK(GREEN_LED);
    else
      PTB->PSOR |= MASK(GREEN_LED);
    break;
  case BLUE:
    if (sw)
      PTD->PCOR |= MASK(BLUE_LED);
    else
      PTD->PSOR |= MASK(BLUE_LED);
    break;
  }
}
