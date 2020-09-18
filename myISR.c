#include "MKL25Z4.h"                    // Device header

#define SW_POS 6

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
 
#define MASK(x) (1 << (x))

unsigned int counter = 0;
char led_mapping[3][2] = {{0, RED_LED}, {1, GREEN_LED}, {2, BLUE_LED}};
// char led_control = 1;
// char int_count = 0;

/* GPIO Initialization Function */
void initGPIO() {
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	// RED LED
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	// GREEN LED
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	// BLUE LED
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	// Set as output
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}	

void offRGB() {
	/* Active low, so set bits to 1 to switch off */
	PTB->PSOR = (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PSOR = MASK(BLUE_LED);
}

// Manage behaviour of LED
void ledControl(colour) {
	offRGB();
	switch(colour) {
		case RED_LED:
			PTB->PCOR |= MASK(RED_LED);
			break;
		case GREEN_LED:
			PTB->PCOR |= MASK(GREEN_LED);
			break;
		case BLUE_LED:
			PTD->PCOR |= MASK(BLUE_LED);
			break;
		default:
			offRGB();
	}
}

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
	counter++;
	
	//Clear INT Flag
	PORTD->ISFR |= MASK(SW_POS); // ISFR = Interrupt Status Flag Register. Important to clear it (1 to clear the flag to stop sending interrupt requests)
}


static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

int main(void) {
	/* Updates the variable SystemCoreClock and must be called whenever the core clock is changed during program execution. 
	   The function evaluates the clock register settings and calculates the current core clock. */
	SystemCoreClockUpdate();
	initSwitch();
	initGPIO();
	offRGB();
	
	while(1){
		ledControl(led_mapping[counter][1]);
		if(counter > 0x02) {
			counter = 0;
		}
		delay(0xFFFFF);
	}
}