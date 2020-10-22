#ifndef SOUND_H
#define SOUND_H

#include "MKL25Z4.h"                    // Device header
#include "basic.h"
#include "PWM.h"
#include "notes.h"

#define TEMPO 32

void initPWMSound(void);

void setPWMFreq(TPM_Type *timer, uint8_t channel, uint16_t freq);

void playTone(TPM_Type *timer, uint8_t channel, uint16_t tone, uint8_t duration);

void playSong(TPM_Type *, uint8_t, uint16_t, uint8_t[], uint16_t[]);

void playMegalovania(void);

void playWindows(void);

void playCoffin(void);

void offSound(void);

#endif
