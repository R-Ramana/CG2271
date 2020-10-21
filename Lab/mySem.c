/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "LED.h"

#define SW_POS 6

osSemaphoreId_t mySem;

// Switch connected to
void initSwitch(void) {
	// enable clock for PortD
	// To supply power to the port
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	/* Select GPIO and enable pull-up resistors and interrupts on
	   falling edges of pin connected to switch
		 If no pull-up resistors, the value will float between 0 & 1....
	*/
	PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) |
											 PORT_PCR_PS_MASK | // Pull select
											 PORT_PCR_PE_MASK | // Pull enable
											 PORT_PCR_IRQC(0x0a)); //0x0a means look at IRQC config [1010 - interrupt on falling edge]

	// Set PORT D Switch bit to input
	PTD->PDDR &= ~(MASK(SW_POS));
	
	//Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn, 2); // Set priority for the entire port, doesn't matter here because only single interrupt
	/* If got interrupt triggered, NVIC will capture. If interrupt is enabled and halfway got disabled and want to re-enable. 
	   Any left over IRQ pending before it was disabled, you want to clear it and not carry it out when it is re-enabled. */
	NVIC_ClearPendingIRQ(PORTD_IRQn); 
	NVIC_EnableIRQ(PORTD_IRQn); // Capture the interrupts
}


void PORTD_IRQHandler() {
	// Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn); // Not necessary, but for legacy reasons
	
	// Updating some variable / flag
  osDelay(500); // debounce
	osSemaphoreRelease(mySem);
	
	//Clear INT Flag
	PORTD->ISFR |= MASK(SW_POS); // ISFR = Interrupt Status Flag Register. Important to clear it (1 to clear the flag to stop sending interrupt requests)
}
 
/*----------------------------------------------------------------------------
 * Application led_red thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
 
  // ...
  for (;;) {
    osSemaphoreAcquire(mySem, osWaitForever);
    
    ledControl(RED, LED_ON);
    osDelay(1000);
    ledControl(RED, LED_OFF);
    osDelay(1000);    
    
    //osSemaphoreRelease(mySem);
  }
}

/*----------------------------------------------------------------------------
 * Application led_green thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
 
  // ...
  for (;;) {
    osSemaphoreAcquire(mySem, osWaitForever);
    
    ledControl(GREEN, LED_ON);
    osDelay(1000);
    ledControl(GREEN, LED_OFF);
    osDelay(1000);    
    
    //osSemaphoreRelease(mySem);
  }
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  InitLEDGPIO();
  offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  mySem = osSemaphoreNew(1,0,NULL);
  osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
  osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
