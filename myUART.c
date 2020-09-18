#include "MKL25Z4.h"
/* Default Core Clk Freq is 20.97152MHz */
// Current code will run at 48 MHz core clk freq and 24MHz Bus clk freq

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128

// Init UART2
void initUART2(uint32_t baud_rate) {
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

// Delay function
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

int main(void) {
	uint8_t rx_data = 0x69;
	
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	
	while(1) {
		// RX and TX
		UART2_Transmit_Poll(rx_data);
		delay(0x80);
	}
}	