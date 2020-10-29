#ifndef PWM_H_
#define PWM_H_                        /**< Symbol preventing repeated inclusion */

#include "basic.h"

#define DEF_FREQ      50
#define FREQ_2_MOD(x) (375000 / x)
#define DEF_MOD       FREQ_2_MOD(DEF_FREQ)

void initPWMPort(PORT_Type *port, uint8_t pin, uint8_t alt);

void initPWMModule(TPM_Type *timer, uint32_t mod);

void initPWMChannel(TPM_Type *timer, uint8_t channel);

uint32_t getMaskTimer(TPM_Type *timer);

void initPWMAll(PORT_Type *port, uint8_t alt, uint8_t pins[], uint8_t numPins, 
    TPM_Type *timer, uint8_t channels[], uint8_t numChannels);
    
void setFreq(TPM_Type *timer, uint8_t channel, uint16_t freq);

void setDuty(TPM_Type *timer, uint8_t channel, uint8_t duty);

#endif
