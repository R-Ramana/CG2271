# CG2271 2021 S1

This repository contains all the code used for this iteration of CG2271. Lab and Project code used are located in their respective folders. 

## Project

[The project specifications can be found here.](https://github.com/R-Ramana/CG2271/blob/master/Project/Documents/Project%20Specifications_AY2021SEM1.pdf)

Click the image below for a short video on our project!

[![Project Video](https://img.youtube.com/vi/o-zYY2m2vNM/0.jpg)](https://www.youtube.com/watch?v=o-zYY2m2vNM)

The aim of the project is to design a Real-Time Operating System (RTOS) based robotic car that will be controlled through an Android App. The robotic car must be able to fulfil the following features:

1. Establish a BT connection with the Android App
2. Receive commands from the Android App and execute the correct response
3. Move the car in multiple directions.
4. Control the various LED’s according to the car’s status
5. Play different sounds/tunes according to the cars status.

## Features
### BT connectivity
- [x] Flash Green LED to indicate connection
- [x] Receive specific data from BT06 then start LED and play tone
- [X] Create function to receive data from app
- [X] Call Flash green LED
- [X] Call play tune

### Motor Control
- [x] Move in all 4 directions
- [x] Have function for each direction
- [x] Control all 8 pins for each direction
- [x] TPM/PWM initialize all 8 pins using library functions
- [X] Choose which pins belong to which wheel
- [X] Create function to configure each wheel
- [X] Create function to configure movement using function for wheels
- [X] Create function to receive data from app and choose direction

### LED Control
- [x] Green LEDs running mode when moving
- [x] Green LED all lit when stationary
- [x] RED LED flashing (1 Hz) when moving
- [x] RED LED flashing (2 Hz) when stationary
- [x] GPIO initialize all 11 pins using library functions
- [X] Abstract GPIO initialization
- [X] Create function to control GPIO pins
- [X] Create function for running mode vs all lit mode
- [X] Create function for 1 Hz and 2 Hz

### Audio Control
- [x] Continuously play song from start to end
- [x] Play unique tone at the end
- [x] TPM/PWM initialize 1 pin using library functions
- [X] Create function to play tune running vs tune end
