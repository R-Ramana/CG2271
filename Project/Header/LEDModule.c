#include "LEDModule.h"


uint8_t greenPins[] = {8, 9, 10, 11, 2, 3, 4, 5, 20, 21};

uint8_t pinsB[] = {8, 9, 10, 11};
uint8_t pinsE[] = {2, 3, 4, 5, 20, 21};
uint8_t pinsC[] = {9};

void initLEDModules() {
  
  InitGPIOAll(PORTB, pinsB, NUM_LED_B);
  InitGPIOAll(PORTE, pinsE, NUM_LED_E);
  InitGPIOAll(PORTC, pinsC, NUM_LED_C);
}

void offRedModule() {
  setPin(PORTC, pinsC[0], LOW);
}

void offGreenModule() {
  uint8_t i = 0;
  for (; i < 4; ++i)
    setPin(PORTB, greenPins[i], LOW);
  for (; i < 10; ++i)
    setPin(PORTE, greenPins[i], LOW);
}

void offLEDModules() {
  offRedModule();
  offGreenModule();
}

void onOffLED(PORT_Type *port, uint8_t pin) {
  setPin(port, pin, HIGH);
  osDelay(DELAY_LED_RUN);
  setPin(port, pin, LOW);
}

void runningLED() {
  uint8_t i = 0;
  for (; i < 4; ++i)
    onOffLED(PORTB, greenPins[i]);
  
  for (; i < 10; ++i)
    onOffLED(PORTE, greenPins[i]);
}

void allLitLED() {
  uint8_t i = 0;
  for (; i < 4; ++i)
    setPin(PORTB, greenPins[i], HIGH);
  for (; i < 10; ++i)
    setPin(PORTE, greenPins[i], HIGH);
}

void flash2GreenLED() {
  for (uint8_t i = 0; i < 2; ++i)
    flashGreenLED(i);
}

void flashGreenLED(uint8_t in) {
  uint8_t i = in;  
  for (; i < 4; i += 2)
    setPin(PORTB, greenPins[i], HIGH);
  for (; i < 10; i += 2)
    setPin(PORTE, greenPins[i], HIGH);
  osDelay(DELAY_LED_FLASH);
  

  i = in;
  for (; i < 4; i += 2)
    setPin(PORTB, greenPins[i], LOW);
  for (; i < 10; i += 2)
    setPin(PORTE, greenPins[i], LOW);
  
  osDelay(DELAY_LED_FLASH);
}

void flashRedLED_1() {
  setPin(PORTC, 9, HIGH);
  osDelay(DELAY_LED_1HZ);
  setPin(PORTC, 9, LOW);
  osDelay(DELAY_LED_1HZ);
}

void flashRedLED_2() {
  setPin(PORTC, 9, HIGH);
  osDelay(DELAY_LED_2HZ);
  setPin(PORTC, 9, LOW);
  osDelay(DELAY_LED_2HZ);
}

  

