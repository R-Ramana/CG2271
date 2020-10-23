#include "MKL25Z4.h"                    // Device header
#include "PWM.h"
#include "sound.h"
/* 
Default Core Clk Frequency is 20.97152MHz
#define CLOCK_SETUP 1 in <system_MKL25Z4.h> to change Core Clk Freq to 48MHz 
*/

/*
PTB18 -> TM2 CH0
PTB19 -> TM2 CH1
*/

#define PTB0_Pin 0
#define PTB1_Pin 1
#define PTE29_Pin 29
#define PTE_Pin(x) x
#define MUSICAL_NOTE_CNT 7
#define FREQ_2_MOD(x) (375000 / x)

int musical_notes[] = {262, 294, 330, 349, 392, 440, 494};

void initPWM() {
  
  uint8_t pins[] = {PTB0_Pin, PTB1_Pin};
  uint8_t channels[] = {0, 1};
  
  initPWMAll(PORTB, pins, 2, TPM1, channels, 2);
  /*
  uint8_t pinsE[] = {29, 30, 31};
  uint8_t channelsE[] = {2, 3, 4};
  
  initPWMAll(PORTE, pinsE, 3, TPM0, channelsE, 3);
  */
}

int main() {
  char i = 0;
  SystemCoreClockUpdate();
  initPWM();
  
  //TPM1_C0V = 0x0666; // 0x666 = 1638 (half of 3276) -> 50% Duty Cycle
  TPM1_C0V = 0x0EA6; // 0xEA6 = 3750 (half of 7500) -> 50% Duty Cycle
  TPM1_C1V = 0x753; // 0x0EA6;
  
  
  while (1) {
    playSong(TPM1, 0);
    /*
    for (i = 0; i < MUSICAL_NOTE_CNT; i++) {
      // Changing MOD changes frequency
      TPM1->MOD = FREQ_2_MOD(musical_notes[i]);
      // Channel 0 duty cycle always 50%
      TPM1_C0V = FREQ_2_MOD(musical_notes[i]) / 2;
      delay2(0xFFFF);
    }
    */
  }
}
