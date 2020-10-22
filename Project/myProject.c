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


osMutexId_t myConnMutex;

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

void bluetooth_connect() {
  flash2GreenLED();
  bleNum = STOP;
  playWindows();
}

/*----------------------------------------------------------------------------
 * Application led_green_thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
  // ...
  for (;;) {
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
  myConnMutex = osMutexNew(NULL);
  while (bleNum == 0); // Wait for connection
  bluetooth_connect();
  osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
  osThreadNew(sound_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
