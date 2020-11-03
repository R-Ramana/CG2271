/*-------------------------------------`---------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include "LEDModule.h"
#include "UART.h"
#include "sound.h"
#include "motors.h"

#define STOP  83
#define UP    85
#define DOWN  68
#define LEFT  76
#define RIGHT 82
#define NW    57
#define NE    45
#define SE    53
#define SW    55
#define AUDIO 65
#define BT    5

char isMove = 0;

const osThreadAttr_t priorityAbNormal = {
  .priority = osPriorityAboveNormal
};
const osThreadAttr_t priorityHigh = {
  .priority = osPriorityHigh
};
const osThreadAttr_t priorityMax = {
  .priority = osPriorityRealtime
};

char isMoving() {
  if (bleNum == STOP)
    isMove = 0;
  else if (bleNum == UP || bleNum == DOWN || bleNum == LEFT || bleNum == RIGHT)
    isMove = 1;
  return isMove;
}

void bluetooth_connect() {
  flash2GreenLED();
  bleNum = STOP;
  playWindowsDelay2();
  offSound();
  delay2(0xFFF);
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
 * Application motor_thread
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
  // ...
  for (;;) {
    osSemaphoreAcquire(moveSem, osWaitForever);
    switch (bleNum) {
    case UP:
      move(0b1100);
      break;
    case DOWN:
      move(0b1000);
      break;
    case LEFT:
      move(0b1111);
      break;
    case RIGHT:
      move(0b1110);
      break;
    case NW:
      move(0b1111);
      break;
    case NE:
      move(0b1110);
      break;
    case SE:
      move(0b1010);
      break;
    case SW:
      move(0b1011);
      break;
    default:
      break;
    }
    bleNum = 0;
  }
}


/*----------------------------------------------------------------------------
 * Application sound_thread
 *---------------------------------------------------------------------------*/
void sound_thread (void *argument) {
  // ...
  for (;;) {
    playMegalovania();
  }
}

/*----------------------------------------------------------------------------
 * Application brain_thread
 *---------------------------------------------------------------------------*/
void brain_thread (void *argument) {
  // ...
  for (;;) {
    osSemaphoreAcquire(brainSem, osWaitForever);
    switch (bleNum) {
    case AUDIO:
      playCoffin();
      break;
    case UP:
    case DOWN:
    case LEFT:
    case RIGHT:
    case NW:
    case NE:
    case SE:
    case SW:
      osSemaphoreRelease(moveSem);
      break;
    default:
      break;
    }
  }
}

 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  
  initUART2(BAUD_RATE);
  initLEDModules();
  offLEDModules();
  initPWMSound();
  
  initPWMMotors();
  offSound();
  stop();
  //playWindowsDelay2();
  //offSound();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  
  // Semaphores for threads to acquire before running
  // max_count = 1 -> only 1 thread can acquire
  // initial_count = 1 -> can acquire immediately
  // initial_count = 0 -> wait for release
  // brainSem only released by UART2_IRQ Receive Interrupt
  // other Sem only released by brain_thread
  brainSem = osSemaphoreNew(1, 0, NULL);
  musicSem = osSemaphoreNew(1, 1, NULL);
  moveSem = osSemaphoreNew(1, 0, NULL);
  
  // Wait for connection before entering multithreaded environment
  while (bleNum != BT); 
  bluetooth_connect();
  
  // Thread inception
  osThreadNew(brain_thread, NULL, &priorityMax); // Create brain thread
  osThreadNew(sound_thread, NULL, &priorityHigh);
  osThreadNew(led_green_thread, NULL, NULL);    
  osThreadNew(led_red_thread, NULL, NULL);    
  osThreadNew(motor_thread, NULL, &priorityAbNormal);
  
  osKernelStart();                      // Start thread execution
  
  for (;;) {}
  
}
