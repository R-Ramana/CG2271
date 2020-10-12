# CG2271 2021 S1

## Features
A BT connectivity
- [x] Flash Green LED to indicate connection
- [x] Receive specific data from BT06 then start LED and play tone
- [ ] Create function to receive data from app
- [ ] Call Flash green LED
- [ ] Call play tune

B Motor Control
- [x] Move in all 4 directions
- [x] Have function for each direction
- [x] Control all 8 pins for each direction
- [x] TPM/PWM initialize all 8 pins using library functions
- [ ] Choose which pins belong to which wheel
- [ ] Create function to configure each wheel
- [ ] Create function to configure movement using function for wheels
- [ ] Create function to receive data from app and choose direction

C LED Control
- [x] Green LEDs running mode when moving
- [x] Green LED all lit when stationary
- [x] RED LED flashing (1 Hz) when moving
- [x] RED LED flashing (2 Hz) when stationary
- [x] GPIO initialize all 11 pins using library functions
- [ ] Abstract GPIO initialization (Wira)
- [ ] Create function to control GPIO pins
- [ ] Create function for running mode vs all lit mode
- [ ] Create function for 1 Hz and 2 Hz

D Audio Control
- [x] Continuously play song from start to end
- [x] Play unique tone at the end
- [x] TPM/PWM initialize 1 pin using library functions
- [ ] Create function to play tune running vs tune end