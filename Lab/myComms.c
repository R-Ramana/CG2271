/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "LED.h"

#define MSG_COUNT 1

typedef struct {
  uint8_t cmd;
  uint8_t data;
} myDataPkt;

osMessageQueueId_t redMsg, greenMsg, blueMsg;
osThreadId_t redLED_Id, greenLED_Id, blueLED_Id, control_Id;
 
/*----------------------------------------------------------------------------
 * Application led_red thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
  
  myDataPkt myRxData;
  // ...
  for (;;) {
    // osThreadFlagsWait(0x1, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(redMsg, &myRxData, NULL, osWaitForever);
    if (myRxData.cmd == 0x01 && myRxData.data == 0x01) {
    ledControl(RED, LED_ON);
    osDelay(1000);
    ledControl(RED, LED_OFF);
    osDelay(1000);
    }
  }
}

/*----------------------------------------------------------------------------
 * Application led_green thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
  
  myDataPkt myRxData;
  // ...
  for (;;) {
    // osThreadFlagsWait(0x1, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(redMsg, &myRxData, NULL, osWaitForever);
    if (myRxData.cmd == 0x01 && myRxData.data == 0x02) {
    ledControl(GREEN, LED_ON);
    osDelay(1000);
    ledControl(GREEN, LED_OFF);
    osDelay(1000);
    }
  }
}

/*----------------------------------------------------------------------------
 * Application led_green thread
 *---------------------------------------------------------------------------*/
void led_blue_thread (void *argument) {
 
  myDataPkt myRxData;
  // ...
  for (;;) {
    // osThreadFlagsWait(0x1, osFlagsWaitAny, osWaitForever);
    osMessageQueueGet(redMsg, &myRxData, NULL, osWaitForever);
    if (myRxData.cmd == 0x01 && myRxData.data == 0x03) {
    ledControl(BLUE, LED_ON);
    osDelay(1000);
    ledControl(BLUE, LED_OFF);
    osDelay(1000);    
    }
  }
}

/*----------------------------------------------------------------------------
 * Application control thread
 *---------------------------------------------------------------------------*/
void control_thread (void *argument) {
  
  myDataPkt myData;
  
  myData.cmd = 0x01;
  myData.data = 0x01;
  // ...
  for (;;) {
    // Thread Flags
    /*
    osThreadFlagsSet(redLED_Id, 0x1);
    osDelay(1000);
    osThreadFlagsSet(greenLED_Id, 0x1);
    osDelay(1000);    
    osThreadFlagsSet(blueLED_Id, 0x1);
    osDelay(1000);
    */
        
    
    // Message Queues
    myData.data = 0x01;
    osMessageQueuePut(redMsg, &myData, NULL, 0);
    osDelay(2000);
    
    myData.data = 0x02;
    osMessageQueuePut(greenMsg, &myData, NULL, 0);
    osDelay(2000);
    
    myData.data = 0x03;
    osMessageQueuePut(blueMsg, &myData, NULL, 0);
    osDelay(2000);
    
    myData.data = 0x04;
    osMessageQueuePut(redMsg, &myData, NULL, 0);
    osMessageQueuePut(greenMsg, &myData, NULL, 0);
    osMessageQueuePut(blueMsg, &myData, NULL, 0);
    osDelay(2000);
  }
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  InitLEDGPIO();
  offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  redLED_Id = osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
  greenLED_Id = osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  blueLED_Id = osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
  control_Id = osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  redMsg = osMessageQueueNew(MSG_COUNT, sizeof(myDataPkt), NULL);
  greenMsg = osMessageQueueNew(MSG_COUNT, sizeof(myDataPkt), NULL);
  blueMsg = osMessageQueueNew(MSG_COUNT, sizeof(myDataPkt), NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
