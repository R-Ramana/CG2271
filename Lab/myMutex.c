/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "LED.h"
 
 
osMutexId_t myMutex;

const osThreadAttr_t thread_attr = {
  .priority = osPriorityNormal1
};
 
/*----------------------------------------------------------------------------
 * Application led_red thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
 
  // ...
  for (;;) {
    osMutexAcquire(myMutex, osWaitForever);
    ledControl(RED, LED_ON);
    osDelay(1000);
    ledControl(RED, LED_OFF);
    osDelay(1000);    
    
    osMutexRelease(myMutex);
  }
}

/*----------------------------------------------------------------------------
 * Application led_green thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
 
  // ...
  for (;;) {
    osMutexAcquire(myMutex, osWaitForever);
    
    ledControl(GREEN, LED_ON);
    osDelay(1000);
    ledControl(GREEN, LED_OFF);
    osDelay(1000);    
    
    osMutexRelease(myMutex);
  }
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  InitLEDGPIO();
  offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myMutex = osMutexNew(NULL);
  osThreadNew(led_red_thread, NULL, &thread_attr);    // Create led_red thread
  osThreadNew(led_green_thread, NULL, NULL);  // Create led_green thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
