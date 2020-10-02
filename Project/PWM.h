#ifndef PWM_H_
#define PWM_H_                        /**< Symbol preventing repeated inclusion */

#include "MKL25Z4.h"                    // Device header

#define FREQ_2_MOD(x) (375000 / x)

void initPWMPort(PORT_Type *port, uint8_t pin);

void initPWMModule(TPM_Type *timer, uint32_t mod);

void initPWMChannel(TPM_Type *timer, uint8_t channel);

uint32_t getMaskPort(PORT_Type *port);

uint32_t getMaskTimer(TPM_Type *timer);

void initPWMAll(PORT_Type *port, uint8_t pins[], uint8_t numPins, 
    TPM_Type *timer, uint8_t channels[], uint8_t numChannels);

#endif
