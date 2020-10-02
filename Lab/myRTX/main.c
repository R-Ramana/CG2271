/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "GPIO.h"

#define RED_LED 18      // PortB Pin 18
#define GREEN_LED 19    // PortB Pin 19
#define BLUE_LED 1      // PortD Pin 1
#define PTE20 20

typedef enum {LED_OFF, LED_ON} led_t;
typedef enum {RED, GREEN, BLUE} color_t;

/* Delay function */
static void delay(volatile uint32_t nof) {
  while (nof!=0) {
    __asm("NOP");
    nof--;
  }
}


void InitGPIO(void)
{
  uint8_t pinsB[] = {RED_LED, GREEN_LED}, numPinsB = 2;
  uint8_t pinsD[] = {BLUE_LED}, numPinsD = 1;
  uint8_t pin[] = {20};
  
  InitGPIOAll(PORTB, pinsB, numPinsB);
  InitGPIOAll(PORTD, pinsD, numPinsD);
  InitGPIOAll(PORTE,pin , 1);
}

void offRGB() {
  // Set all (active low)
  PTB->PSOR |= MASK(RED_LED) | MASK(GREEN_LED);
  PTD->PSOR |= MASK(BLUE_LED);
}

void led_control(color_t color, led_t sw) {
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
 
/*----------------------------------------------------------------------------
 * Application led_red thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
 
  // ...
  for (;;) {
    setPin(PORTE, PTE20, HIGH);
    led_control(RED, LED_ON);
    osDelay(1000);
    setPin(PORTE, PTE20, LOW);
    led_control(RED, LED_OFF);
    osDelay(1000);    
  }
}

/*----------------------------------------------------------------------------
 * Application led_green thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
 
  // ...
  for (;;) {
    
    led_control(GREEN, LED_ON);
    osDelay(1000);
    led_control(GREEN, LED_OFF);
    osDelay(1000);    
  }
}
 
int main (void) {
  
  // System Initialization
  SystemCoreClockUpdate();
  // ...
  InitGPIO();
  offRGB();
  setPin(PORTE, 20, LOW);
  
  
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(led_red_thread, NULL, NULL);    // Create application led_red thread
  osThreadNew(led_green_thread, NULL, NULL);    // Create application led_red thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
