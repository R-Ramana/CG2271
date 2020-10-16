#ifndef UART_H
#define UART_H

#include "MKL25Z4.h"
/* Default Core Clk Freq is 20.97152MHz */
// Current code will run at 48 MHz core clk freq and 24MHz Bus clk freq

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128

#define LED_RED 2 // 0b00000010
#define LED_MASK(x) (x & 0x06)
#define BIT0_MASK(x) (x & 0x01)

#define Q_SIZE (32)

extern volatile uint8_t bleNum;

typedef struct {
  unsigned char Data[Q_SIZE];
  unsigned int Head;
  unsigned int Tail; // points to next free space
  unsigned int Size; // quantity of elements in queue
} Q_T;

// Init UART2
void initUART2(uint32_t);

void UART2_IRQHandler(void);

#endif
