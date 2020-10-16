/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "LEDModule.h"
#include "UART.h"
#include "sound.h"

#define STOP  83
#define UP    85
#define DOWN  68
#define LEFT  76
#define RIGHT 82
#define AUDIO 65
#define BT    5


osMutexId_t myBTMutex, myConnMutex;

char move = 0;
char isConnected = 0;

const osThreadAttr_t thread_attr = {
  .priority = osPriorityAboveNormal
};

char isMoving() {
  if (bleNum == STOP)
    move = 0;
  else if (bleNum == UP || bleNum == DOWN || bleNum == LEFT || bleNum == RIGHT)
    move = 1;
  return move;
}

void bluetooth_connect_thread (void *argument) {
  for (;;) {
    
    osMutexAcquire(myBTMutex, osWaitForever);
    if (bleNum != BT) {
      osMutexRelease(myBTMutex);
      continue;
    }
    
    osMutexAcquire(myConnMutex, osWaitForever);
    flash2GreenLED();
    bleNum = STOP;
    playWindows();
    osMutexRelease(myConnMutex);
  }
}

/*----------------------------------------------------------------------------
 * Application led_green_thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
  // ...
  for (;;) {
    osMutexAcquire(myConnMutex, osWaitForever);
    osMutexRelease(myConnMutex);
    if (isMoving())
      runningLED();
    else
      allLitLED();
     
    
  }
}


/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
  // ...
  for (;;) {    
    osMutexAcquire(myConnMutex, osWaitForever);
    osMutexRelease(myConnMutex);
    if (isMoving())
      flashRedLED_1();
    else      
      flashRedLED_2();
  }
}

/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void sound_thread (void *argument) {
  // ...
  for (;;) {    
    osMutexAcquire(myConnMutex, osWaitForever);
    osMutexRelease(myConnMutex);
    if (isMoving())
      playMegalovania();
    else      
      playCoffin();
  }
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  initUART2(BAUD_RATE);
  initLEDModules();
  offLEDModules();
  initPWMSound();
  offSound();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myBTMutex = osMutexNew(NULL);
  myConnMutex = osMutexNew(NULL);
  osThreadNew(bluetooth_connect_thread, NULL, &thread_attr);
  osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
  osThreadNew(sound_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
