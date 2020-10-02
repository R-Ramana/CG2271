#include "MKL25Z4.h"                    // Device header
#include "GPIO.h"




void initGPIOPort(PORT_Type *port, uint8_t pin) {
  // Configure MUX settings to make all 3 pins GPIO
  // MUX_MASK = 011100000000, first step is to clear MUX
  // MUX(1) = 1 << 8, then set as GPIO
  // Configure Mode 1 for GPIO pin operation
  port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
  port->PCR[pin] |= PORT_PCR_MUX(1);  
}

GPIO_Type* getGPIO(PORT_Type* port) {
  if (port == PORTA)
    return PTA;
  else if (port == PORTB)
    return PTB;
  else if (port == PORTC)
    return PTC;
  else if (port == PORTD)
    return PTD;  
  else
    return PTE;
}

void InitGPIOAll(PORT_Type *port, uint8_t pins[], uint8_t numPins)
{
  // Used when setting DDR I/O
  uint32_t mask = 0;
  
  // Enable Clock to port
  // Controls the clock gate to the PORT modules
  SIM->SCGC5 |= getMaskPort(port);
  
  // Set all pins of port MUX to GPIO
  // Gets mask for all pins for DDR
  for (uint8_t i = 0; i < numPins; ++i) {
    initGPIOPort(port, pins[i]);
    mask |= MASK(pins[i]);
  }
  
  // Set Data Direction Registers for PortB and PortD
  // Configures pins as Input and Output
  getGPIO(port)->PDDR |= mask;
  
}

void setPin(PORT_Type *port, uint8_t pin, value_t value) {
  GPIO_Type *gpio = getGPIO(port);
  if (value == HIGH)
    gpio->PSOR |= MASK(pin);
  else
    gpio->PCOR |= MASK(pin);
}
