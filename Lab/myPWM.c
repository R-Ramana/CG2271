#include "MKL25Z4.h"
/* Default Core Clk Freq is 20.97152MHz */

#define PTB0_Pin 0
#define PTB1_Pin 1

#define SW_POS 6

#define MASK(x) (1 << (x))

#define MUSICAL_NOTE_CNT 7
#define FREQ_2_MOD(x) (375000 / x)

// float musical_notes[MUSICAL_NOTE_CNT] = {1318.50, 1174.70, 739.99, 830.61, 1108.70, 987.77, 587.33, 659.26, 987.77, 880.00, 554.37, 659.26, 880.00};
int musical_notes[] = {262, 294, 330, 349, 392, 440, 494};
	
unsigned int counter = 0;

/* Initialize PWM */
void initPWM(void) {
	// Enable Clock to PORTB
	// Need to enable port power because need to configure the registers for the MUX
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Configure MUX settings to make pins GPIO
	// Configure Mode 3 for the new PWM pin operation
	// Pin 0
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	// Alternative 3 (chip specific) Timer Module Channel 0
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	// Pin 1
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	// Alternative 3 (chip specific) Timer Module Channel 1
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	// Enable Clock for Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	// Select Clock for TPM module
	// TPMSRC = Clk source [using MCGFLLCLK]
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	// Set Modulo Value 20971520 / 128 = 163840
	// 163840 / 3276 = 50 Hz
	// TPM1->MOD = 3276;
	
	// Set Modulo value to 48000000 (48 Mhz) / 128 (Prescalaer) = 375000 
	// 375000 / 7500 = 50 Hz [basically when reach 7500 (max count) will flip over to 0 and start again]
	TPM1->MOD = 7500;

	// Edge-Aligned PWM
	// Update SnC register: CMOD = 01, PS = 111 (128)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	// Set CMOD to 1 == LPTPM counter increments on every LPTPM counter clock
	// PS 7 is 128 prescaler
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	// Enable PWM on TPM1 Channel 0 -> PTB0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Enable PWM on TPM1 Channel 1 -> PTB1
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void setFreq(int freq) {
	int newMod = 375000 / freq;
	TPM1->MOD = newMod;
	
	// Edge-Aligned PWM
	// Update SnC register: CMOD = 01, PS = 111 (128)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	// Set CMOD to 1 == LPTPM counter increments on every LPTPM counter clock
	// PS 7 is 128 prescaler
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	
	TPM1_C0V = newMod/2;
	
}

// Delay function
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

static void delay2(volatile uint32_t nof) {
	for(int i = 0; i < 100; i++) {
		delay(nof);
	}
}

int main(void) {
	char i = 0;
	SystemCoreClockUpdate();
	initPWM();
	
	//TPM1_C0V = 0x0666; //0x0666 = 1638 (half of 3276) -> 50% duty cycle
	TPM1_C0V = 0x0EA6; // 0xEA6 = 3750, basically half od 7500 for 50% duty cycle
	TPM1_C1V = 0x753; // 0x0EA6
	
	while(1) {
		/*if(counter >= 12) {
			counter = 0;
		}
		else {
			setFreq(musical_notes[counter]);
			//delay(0x7D);
			delay(0xFA);
			//delay(0x2EE);
			counter++;
		}*/
		 for(i = 0; i < MUSICAL_NOTE_CNT; i++) {
			TPM1->MOD = FREQ_2_MOD(musical_notes[i]);
			TPM1_C0V = (FREQ_2_MOD(musical_notes[i])) / 2;
			delay2(0xFFFF);
		} 
	}
}