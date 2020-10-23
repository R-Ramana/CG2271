/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "UART.h"
#include "GPIO.h"
#include "LED.h"

#define PTE20 20

volatile uint8_t var;

/* GPIO Initialization */
void InitGPIO() {
  InitLEDGPIO();
  
  uint8_t pinsE[] = {PTE20}, numPinsE = 1;
  InitGPIOAll(PORTE, pin, numPinsE);
}

/* UART2 Interrupt Handler */
void UART2_IRQHandler() {
  NVIC_ClearPendingIRQ(UART2_IRQn);
  
  if (UART2->S1 & UART_S1_RDRF_MASK)
    var = UART2->D;

  // Error Cases
  if (UART2->S1 & (UART_S1_OR_MASK 
                  | UART_S1_NF_MASK
                  | UART_S1_FE_MASK 
                  | UART_S1_PF_MASK)) {
    // handle the error
    
    // clear the flag
  }
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
	uint8_t rx_data = 0x01;
	
  // ...
  for (;;) {
		// RX
		rx_data = UART2_Receive_Poll();
		if (rx_data == 0x05) {
      for (uint8_t i = 0; i < 2; ++i) {
        ledControl(RED, LED_ON);
        setPin(PORTE, PTE20, HIGH)
        osDelay(500);
        offRGB();
        setPin(PORTE, PTE20, LOW);
        osDelay(500);
      }			
		}	
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  initUART2(BAUD_RATE);
  InitGPIO();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
