#ifndef SOUND_H
#define SOUND_H

#include "PWM.h"
#include "notes.h"

#define TEMPO 32

#define AUDIO 65

void initPWMSound(void);

void setPWMFreq(TPM_Type *timer, uint8_t channel, uint32_t freq);

void playTone(TPM_Type *timer, uint8_t channel, uint32_t tone, uint8_t duration);

void playToneDelay2(TPM_Type *timer, uint8_t channel, uint32_t tone, uint8_t duration);

void playSong(TPM_Type *, uint8_t, uint32_t, uint8_t[], uint32_t[]);

void playSongDelay2(TPM_Type *, uint8_t, uint32_t, uint8_t[], uint32_t[]);

void playSongNoSem(TPM_Type *, uint8_t, uint32_t, uint8_t[], uint32_t[]);

void playMegalovania(void);

void playWindows(void);

void playWindowsDelay2(void);

void playCoffin(void);

void offSound(void);

#endif
