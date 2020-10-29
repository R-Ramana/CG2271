#include "PWM.h"

void initPWMPort(PORT_Type *port, uint8_t pin, uint8_t alt) {
  // Configure Mode 3 for PWM pin operation
  // Alternative 3
  port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
  port->PCR[pin] |= PORT_PCR_MUX(alt);  
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
  // Set CMOD to 1 == LPTPM counter increments on every LPTPM counter clock
	// PS 7 is 128 prescaler
  timer->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
  timer->SC &= ~(TPM_SC_CPWMS_MASK);
}

void initPWMChannel(TPM_Type *timer, uint8_t channel) {
  // Enable PWM on TPM1 Channel 0 -> PTB0
  TPM_CnSC_REG(timer,channel) &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
  TPM_CnSC_REG(timer,channel) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

uint32_t getMaskTimer(TPM_Type *timer) {
  if (timer == TPM0)
    return SIM_SCGC6_TPM0_MASK;
  else if (timer == TPM1)
    return SIM_SCGC6_TPM1_MASK;
  else
    return SIM_SCGC6_TPM2_MASK;
}

void initPWMAll(PORT_Type *port, uint8_t alt, uint8_t pins[], uint8_t numPins, 
    TPM_Type *timer, uint8_t channels[], uint8_t numChannels) {
  // Enable Clock Gating for PORTB
  SIM_SCGC5 |= getMaskPort(port);
  
  for (uint8_t i = 0; i < numPins; ++i)
    initPWMPort(port, pins[i], alt);
  
  // Enable Clock Gating for Timer1
  SIM->SCGC6 |= getMaskTimer(timer);
  
  // Set Modulo Value 20971520 / 128 = 163840 / 327 = 50 Hz
  //initPWMModule)TPM1, 3276);
  // Set Modulo Value 48000000 / 128 = 375000 / 7500 = 50 Hz
  initPWMModule(timer, 7500);
  
  for (uint8_t i = 0; i < numChannels; ++i)
    initPWMChannel(timer, channels[i]);
}

void setFreq(TPM_Type *timer, uint8_t channel, uint16_t freq) {
	timer->MOD = FREQ_2_MOD(freq);	
	TPM_CnV_REG(timer,channel) = FREQ_2_MOD(freq)/2; // 50% duty cycle
}

void setDuty(TPM_Type *timer, uint8_t channel, uint8_t duty) {
  TPM_CnV_REG(timer,channel) = ((uint32_t)duty * DEF_MOD) / 100;
}
