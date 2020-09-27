#include "MKL25Z4.h"                    // Device header

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))

typedef enum {RED, GREEN, BLUE} color_t;
color_t currentColor = RED;

unsigned int counter = 0;

/* GPIO Initialization */
void InitGPIO(void){
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	// Configure MUX settings to make all 3 pins GPIO
	// PORT_PCR_MUX_MASK = 0x700u. Need to clear it first 
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	// Set PCR[COLOR_LED] to 1 because GPIO (pg 184 datasheet)
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}

void turnOffRGB() {
	/* Active low, so set bits to 1 to switch off */
	PTB->PSOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PSOR = MASK(BLUE_LED);
}

void led_control(color_t color) {
    turnOffRGB();
		/* Turn on specific LED colour, active low so set bits to 0 */
    switch(color) {
		case RED:
			PTB->PCOR |= MASK(RED_LED);
			break;
		case GREEN:
			PTB->PCOR |= MASK(GREEN_LED);
			break;
		case BLUE:
			PTD->PCOR |= MASK(BLUE_LED);
			break;
		}
}

static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

/* MAIN function */
int main(void) {
	SystemCoreClockUpdate();
	InitGPIO();
	delay(0x001AAAA);
	led_control(currentColor);
	while(1) {
		delay(0x001AAAAA);
		if (currentColor == BLUE) {
			currentColor = RED;
		} else {
			currentColor++;
		}
		led_control(currentColor);
	}
}
