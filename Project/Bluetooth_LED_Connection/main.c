/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "BTtoLED.h"
#include "GPIO.h"
 
 void pinControl(int val) {
	if(val) {
		PTE->PSOR |= MASK(20);
	} else {
		PTE->PCOR |= MASK(20);
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
			ledControl(RED);
			pinControl(1);
			osDelay(500);
			offRGB();
			pinControl(0);
			osDelay(500);
			ledControl(RED);
			pinControl(1);
			osDelay(500);
			offRGB();
			pinControl(0);
			osDelay(500);
			
		}	
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	InitGPIO();
	
	// Configure MUX settings to make all 3 pins GPIO
	// PORT_PCR_MUX_MASK = 0x700u. Need to clear it first 
	PORTE->PCR[20] &= ~PORT_PCR_MUX_MASK;
	// Set PCR[COLOR_LED] to 1 because GPIO (pg 184 datasheet)
	PORTE->PCR[20] |= PORT_PCR_MUX(1);
	PTE->PDDR |= MASK(20);
	PTE->PCOR |= MASK(20);
	//pinControl(1);
	//uint8_t pin[] = {20};
	//InitGPIOAll(PORTE, pin, 1);
	//PTE->PSOR |= MASK(20);
	//PTE->PCOR |= MASK(20);
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
