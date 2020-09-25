#include "PWM.h"

void initPWMPort(PORT_Type *port, uint8_t pin) {
  // Configure Mode 3 for PWM pin operation
  // Alternative 3
  port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
  port->PCR[pin] |= PORT_PCR_MUX(3);  
}

void initPWMModule(TPM_Type *timer, uint32_t mod) {
  // Select clock for TPM module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGFLLCLK or MCGPLLCLK/2
  
  // Set Modulo Value 48000000 / 128 = 375000 / mod Hz
  timer->MOD = mod;
  
  // Edge-Aligned PWM
  // Update SnC register: CMOD = 01, PS=111 (128)
  timer->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  timer->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
  timer->SC &= ~(TPM_SC_CPWMS_MASK);
}

void initPWMChannel(TPM_Type *timer, uint8_t channel) {
  // Enable PWM on TPM1 Channel 0 -> PTB0
  TPM_CnSC_REG(timer,channel) &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM_CnSC_REG(timer,channel) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

uint32_t getMaskPort(PORT_Type *port) {
  if (port == PORTA)
    return SIM_SCGC5_PORTA_MASK;
  else if (port == PORTB)
    return SIM_SCGC5_PORTB_MASK;
  else if (port == PORTC)
    return SIM_SCGC5_PORTC_MASK;
  else if (port == PORTD)
    return SIM_SCGC5_PORTD_MASK;
  else
    return SIM_SCGC5_PORTE_MASK;
    
}

uint32_t getMaskTimer(TPM_Type *timer) {
  if (timer == TPM0)
    return SIM_SCGC6_TPM0_MASK;
  else if (timer == TPM1)
    return SIM_SCGC6_TPM1_MASK;
  else
    return SIM_SCGC6_TPM2_MASK;
}

void initPWMAll(PORT_Type *port, uint8_t pins[], uint8_t numPins, 
    TPM_Type *timer, uint8_t channels[], uint8_t numChannels) {
  // Enable Clock Gating for PORTB
  SIM_SCGC5 |= getMaskPort(port);
  
  for (uint8_t i = 0; i < numPins; ++i)
    initPWMPort(port, pins[i]);
  
  // Enable Clock Gating for Timer1
  SIM->SCGC6 |= getMaskTimer(timer);
  
  // Set Modulo Value 20971520 / 128 = 163840 / 327 = 50 Hz
  //initPWMModule)TPM1, 3276);
  // Set Modulo Value 48000000 / 128 = 375000 / 7500 = 50 Hz
  initPWMModule(timer, 7500);
  
  for (uint8_t i = 0; i < numChannels; ++i)
    initPWMChannel(timer, channels[i]);
}
    
/* Delay Function */
void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void delay2(volatile uint32_t nof) {
  for(int i = 0; i < 100; i++) {
    delay(nof);
  }
}  
