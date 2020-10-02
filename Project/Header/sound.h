#ifndef SOUND_H
#define SOUND_H

#include "MKL25Z4.h"                    // Device header
#include "PWM.h"
#include "notes.h"

#define TEMPO 32

void setPWMFreq(TPM_Type *timer, uint8_t channel, uint16_t freq);

void playTone(TPM_Type *timer, uint8_t channel, uint16_t tone, uint8_t duration);

void playNote(TPM_Type *timer, uint8_t channel, char note, uint8_t duration);

void playSong(TPM_Type *timer, uint8_t channel);

#endif