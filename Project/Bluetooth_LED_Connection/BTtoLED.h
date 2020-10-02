#ifndef BTTOLED_H
#define BTTOLED_H

#include "MKL25Z4.h"
/* Default Core Clk Freq is 20.97152MHz */
// Current code will run at 48 MHz core clk freq and 24MHz Bus clk freq

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1

#define LED_RED 2 // 0b00000010
#define LED_MASK(x) (x & 0x06)
#define BIT0_MASK(x) (x & 0x01)

#define MASK(x) (1 << x)

typedef enum {RED, GREEN, BLUE} color_t;

// Init UART2
void initUART2(uint32_t baud_rate);

void offRGB();

// UART2 Transmit Poll
void UART2_Transmit_Poll(uint8_t data);

// UART2 Receive Poll
uint8_t UART2_Receive_Poll(void);

/* GPIO Initialization */
void InitGPIO(void);

void ledControl(color_t);

#endif