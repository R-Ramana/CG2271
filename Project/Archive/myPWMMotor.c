#include "MKL25Z4.h"                    // Device header
#include "PWM.h"

#define NUM_PINS_D  6
#define PTB_Pin(x)  x
#define ALT_D       4

#define NUM_PINS_A  2
#define PTA_Pin(x)  x
#define ALT_A       3

#define SW_POS      6

typedef enum {LFF = 0, LFR, RFF, RFR, LBF, LBR, RBF, RBR} wheel_t;
typedef enum {DR = 0, DL, } dir_t;
const int16_t BASE_SPEED = 3750;

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

// Wheel pwm
void PORTD_IRQHandler() {
  // Clear Pending IRQ
  NVIC_ClearPendingIRQ(PORTD_IRQn); // Not necessary, but for legacy reasons
  
  // Updating some variable / flag
  if (PORTD->ISFR & MASK(SW_POS)) {
    // which wheel?
  }
  
  //Clear INT Flag
  PORTD->ISFR |= MASK(SW_POS); // ISFR = Interrupt Status Flag Register. Important to clear it (1 to clear the flag to stop sending interrupt requests)
}
 

void move(uint8_t rx_data) {
  // 0b[Go/Stop][U/D][Turn/Straight][L/R]
  bool isStop = (rx_data & 0b1000) == 0;
  bool isStraight = (rx_data & 0b0010) == 0;
  if (isStop && isStraight) return;

  // input --> movement
  // (don't bother finding MSOP)
  // L  0x11 --> 90 deg left forward  --> 0L, 2R
  // UL 1111 --> 45 deg left forward --> 1L, 2R
  // U  110x --> forward              --> 2L, 2R
  // UR 1110 --> 45 deg right forward--> 2L, 1R
  // R  0x10 --> 90 deg right forward --> 2L, 0R
  // DR 1010 --> 45 deg right back   --> -2L, -1R
  // D  100x --> backward             --> -2L, -2R
  // DL 1011 --> 45 deg left back    --> -1L, -2R
  int16_t left, right;
  switch (rx_data) {
  case 0b0011:
  case 0b0111:
    left = 0; right = BASE_SPEED << 1;
    break;
  case 0b1111:
    left = BASE_SPEED; right = BASE_SPEED << 1;
    break;
  case 0b1100:
  case 0b1101:
    left = BASE_SPEED << 1; right = BASE_SPEED << 1;
    break;
  case 0b1110:
    left = BASE_SPEED << 1; right = BASE_SPEED;
    break;
  case 0b0010:
  case 0b0110:
    left = BASE_SPEED << 1; right = 0;
    break;
  case 0b1010:
    left = -(BASE_SPEED << 1); right = -BASE_SPEED;
    break;
  case 0b1000:
  case 0b1001:
    left = -(BASE_SPEED << 1); right = -(BASE_SPEED << 1);
    break;
  case 0b1011
    left = -BASE_SPEED; right = -(BASE_SPEED << 1);
    break;
  }

  if (left >= 0) {
    controlWheel(LFF, left);
    controlWheel(LBF, left);
  } else {
    controlWheel(LFR, -left);
    controlWheel(LBR, -left);
  }

  if (right >= 0) {
    controlWheel(RFF, right);
    controlWheel(RBF, right);
  } else {
    controlWheel(RFR, -right);
    controlWheel(RBR, -right);
  }

  osDelay(10);
  stop();
}

// Need a mapping of wheel to port

void controlWheel(wheel_t wheel, uint16_t speed) {
  uint16_t val = speed * 75;
  switch (wheel) {
  case LFF:
    TPM1_C0V = val; // 0xEA6 = 3750, basically half of 7500 for 50% duty cycle
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
  // TODO: reverse
}

void stop() {
  controlWheel(LFF,0);
  controlWheel(LFR,0);
  controlWheel(RFF,0);
  controlWheel(RFR,0);
  controlWheel(LBF,0);
  controlWheel(LBR,0);
  controlWheel(RBF,0);
  controlWheel(RBR,0);
}


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
