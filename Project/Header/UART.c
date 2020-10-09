#include "MKL25Z4.h"
#include "UART.h"
/* Default Core Clk Freq is 20.97152MHz */
// Current code will run at 48 MHz core clk freq and 24MHz Bus clk freq


// Init UART2
void initUART2(uint32_t baud_rate, Q_T *q) {
  uint32_t divisor, bus_clock;

  // Enable clocking to the two different peripheral block
  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  // Page 162 datasheet (Alt 4 GPIO config) to transmit
  PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4); 

  // Page 162 datasheet (Alt 4 GPIO config) to receive
  PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);

  // TE and RE is to enable both transmitter and receiver
  // Clearing because we are setting up the module (setting to 0)
  UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));

  // Default System Clock is 48MHz [system clock clocks to CPU]
  // Bus clock is to clock some subsystem, just how some system is designed
  // UART runs at half the system clock rate
  bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
  divisor = bus_clock / (baud_rate * 16); // [* 16 because oversampling internally to account for some noise -> unique to this controller]
  UART2->BDH = UART_BDH_SBR(divisor >> 8); // BDH = Buad Rate High
  UART2->BDL = UART_BDL_SBR(divisor); // BDL = Baud Rate low

  UART2->C1 = 0; // No parity so set to 0
  UART2->S2 = 0; // Not configuring anything
  UART2->C3 = 0; // In case you want to send a 9th bit data then can use this. Can enable interrupts if you want to handle errors

  // Turn on transmit and receive
  UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));

  // Initialize Peripheral
  
  NVIC_SetPriority(UART2_IRQn, 128);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);

  UART2->C2 |= UART_C2_TIE_MASK | UART_C2_RIE_MASK;
  UART2->C2 |= UART_C2_RIE_MASK;
  Q_Init(q);  
}	



// UART2 Transmit Poll
void UART2_Transmit_Poll(uint8_t data) {
  // check if transmit data register is empty
  // when empty you put a new data in
  while(!(UART2->S1 & UART_S1_TDRE_MASK));
  UART2->D = data;
}

// UART2 Receive Poll
uint8_t UART2_Receive_Poll(void) {
  // while not empty can read the data
  while(!(UART2->S1 & UART_S1_RDRF_MASK));
  return (UART2->D);
}


void Q_Init(Q_T * q) {
  unsigned int i;
  for (i=0; i<Q_SIZE; i++)
    q->Data[i] = 0; // to simplify our lives when debugging
  q->Head = 0;
  q->Tail = 0;
  q->Size = 0;
}
int Q_Empty(Q_T * q) {
  return q->Size == 0;
}

int Q_Full(Q_T * q) {
  return q->Size == Q_SIZE;
}

int Q_Enqueue(Q_T * q, unsigned char d) {
  // What if queue is full?
  if (Q_Full(q))
    return 0; // failure
  q->Data[q->Tail++] = d;
  q->Tail %= Q_SIZE;
  q->Size++;
  return 1; // success
}

unsigned char Q_Dequeue(Q_T * q) {
  // Must check to see if queue is empty before dequeueing
  if (Q_Empty(q))
    return 0;
  
  unsigned char t = 0;
  t = q->Data[q->Head];
  q->Data[q->Head++] = 0; // to simplify debugging
  q->Head %= Q_SIZE;
  q->Size--;
  return t;
}


void UART2_IRQHandler(Q_T *q) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
  
  /*
  // Transmitter Interrupt Handler
  if(UART2->S1 & UART_S1_TDRE_MASK) {
    // send a character
    if(!Q_Empty(&TxQ)) {
      UART2->D = Q_Dequeue(&TxQ);

    } else {
      // queue is empty so disable tx
      UART2->C2 &= ~UART_C2_TIE_MASK ;
    }	
  }
  */

  
  // Receiver Interrupt Handler
  if(UART2->S1 & UART_S1_RDRF_MASK) {
    // received a character
    if (!Q_Full(q)) {
      Q_Enqueue(q, UART2->D);
    } else {
      // error -queue full.
      while (1)
          ;
    }
  }
  

  // Error Cases
  if (UART2->S1 & (UART_S1_OR_MASK 
                  | UART_S1_NF_MASK
                  | UART_S1_FE_MASK 
                  | UART_S1_PF_MASK)) {
    // handle the error
    
    // clear the flag
  }
}
