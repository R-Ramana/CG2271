#include "motors.h"

const int16_t BASE_SPEED = 50;

/* Initialize PWM */
void initPWMMotors(void) {
  uint8_t pinsD[NUM_PINS_D], pinsA[] = {PTA_Pin(1), PTA_Pin(2)};
  uint8_t channelsA[] = {0, 1};
  
  for (uint8_t i = 0; i < NUM_PINS_D; ++i)
    pinsD[i] = PTB_Pin(i);
  
  // Initialize PORTD0-5, TPM0_CH0-5
  // Initialize PORTA1-2, TPM2-CH1-2
  initPWMAll(PORTD, ALT_D, pinsD, NUM_PINS_D, TPM0, pinsD, NUM_PINS_D);
  initPWMAll(PORTA, ALT_A, pinsA, NUM_PINS_A, TPM2, channelsA, NUM_PINS_A);
}
 

void move(uint8_t rx_data) {
  // 0b[Go/Stop][U/D][Turn/Straight][L/R]
  int isStop = (rx_data & 0b1000) == 0;
  int isStraight = (rx_data & 0b0010) == 0;
  
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
  uint16_t delayTime = 100;
  int left, right;
  switch (rx_data) {
  case 0b0011:
  case 0b0111: // L
    left = -(BASE_SPEED << 1); right = BASE_SPEED << 1;
    delayTime = 500;
    break;
  case 0b1111: // UL
    left = BASE_SPEED >> 1; right = BASE_SPEED << 1;
    break;
  case 0b1100:
  case 0b1101: // U
    left = BASE_SPEED << 1; right = BASE_SPEED << 1;
    break;
  case 0b1110: // UR
    left = BASE_SPEED << 1; right = BASE_SPEED >> 1;
    break;
  case 0b0010:
  case 0b0110: // R
    left = BASE_SPEED << 1; right = -(BASE_SPEED << 1);
    delayTime = 500;
    break;
  case 0b1010: // DR
    //left = -(BASE_SPEED << 1); right = -BASE_SPEED;
    left = BASE_SPEED << 1; right = -(BASE_SPEED << 1);
    delayTime = 100;
    break;
  case 0b1000:
  case 0b1001: // D
    left = -(BASE_SPEED << 1); right = -(BASE_SPEED << 1);
    break;
  case 0b1011: // DL
    //left = -BASE_SPEED; right = -(BASE_SPEED << 1);
    left = -(BASE_SPEED << 1); right = BASE_SPEED << 1;
    delayTime = 100;
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

  osDelay(delayTime);
  //delay(0xF);
  if (rx_data == 0b1100 || rx_data == 0b1010 || rx_data == 0b1011)
    stop();
  //stop();
}

// Need a mapping of wheel to port

void controlWheel(wheel_t wheel, uint16_t speed) {
  uint16_t val = speed * 75;
  switch (wheel) {
  case LFF:
    TPM2_C0V = val; // 0xEA6 = 3750, basically half of 7500 for 50% duty cycle
    break;
  case LBF:
    TPM0_C4V = val;
    break;
  case RFF:
    TPM0_C1V = val;
    break;
  case RBF:
    TPM0_C2V = val;
    break;
  case LFR:
    TPM2_C1V = val;
    break;
  case LBR:
    TPM0_C5V = val;
    break;
  case RFR:
    TPM0_C3V = val;    
    break;
  case RBR:
    TPM0_C0V = val;
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
