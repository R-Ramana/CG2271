#include "MKL25Z4.h"                    // Device header
#include "PWM.h"

#define NUM_PINS_D  6
#define PTB_Pin(x)  x
#define ALT_D       4

#define NUM_PINS_A  2
#define PTA_Pin(x)  x
#define ALT_A       3

#define SW_POS      6

typedef enum {LFF, LFR, RFF, RFR, LBF, LBR, RBF, RBR} wheel_t;

/* Initialize PWM */
void initPWM(void) {
  uint8_t pinsD[NUM_PINS_D], pinsA[] = {PTA_Pin(1), PTA_Pin(2)};
  uint8_t channelsA[] = {0, 1};
  
  for (uint8_t i = 0; i < NUM_PINS_D; ++i)
    pinsD[i] = PTB_Pin(i);
  
  // Initialize PORTD0-5, TPM0_CH0-5
  // Initialize PORTA1-2, TPM2-CH1-2
  initPWMAll(PORTD, ALT_D, pinsD, NUM_PINS_D, TPM0, pinsD, NUM_PINS_D);
  initPWMAll(PORTA, ALT_A, pinsA, NUM_PINS_A, TPM2, channelsA, NUM_PINS_A);
}


// Need a mapping of wheel to port
/*
void controlWheel(wheel_t wheel, uint16_t speed) {
  uint16_t val = speed * 75;
  switch (wheel) {
  case LFF:
    TPM1_C0V = val; // 0xEA6 = 3750, basically half od 7500 for 50% duty cycle
    break;
  case LBF:
    TPM1_C1V = val;
    break;
  case RFF:
    TPM2_C0V = val;
    break;
  case RBF:
    TPM2_C1V = val;
    break;
  }
}
*/

/*
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
*/

int main(void) {
	SystemCoreClockUpdate();
	initPWM();
	
  for (uint8_t i = 1; i < NUM_PINS_D; ++i)
    setDuty(TPM0, i, 20*i);
  
  TPM2_C0V = 0x0EA6;  // 0xEA6 = 3750, basically half of 7500 for 50% duty cycle
  TPM2_C1V = 0x753;   // 25%
	
	while(1) {
	}
}
