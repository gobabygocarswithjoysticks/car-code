#### https://github.com/gobabygocarswithjoysticks/car-code

[![Compile](https://github.com/gobabygocarswithjoysticks/car-code/actions/workflows/compile.yml/badge.svg)](https://github.com/gobabygocarswithjoysticks/car-code/actions/workflows/compile.yml)

## The website for uploading these programs to a car and adjusting settings is here: [https://gobabygocarswithjoysticks.github.io/programmer/](https://gobabygocarswithjoysticks.github.io/programmer/)

#### Github for the website code: https://github.com/gobabygocarswithjoysticks/programmer

#### The new standard code is in this directory: https://github.com/gobabygocarswithjoysticks/car-code/tree/main/gbg_program

#### Instructions for modifying go baby go cars to use this code can be found here: https://gobabygocarswithjoysticks.github.io/index/ 

### Remote Control
[notes and instructions for remote control](https://github.com/gobabygocarswithjoysticks/car-code/blob/main/rcdocs/remote_control.md)

### LED blink codes
* Off: the car is not moving
* On: the car is moving
* Blinking: the car is being told to move but the input hasn't been held at zero for a couple seconds yet

### default pins

#### nano/uno
JOY_X_PIN = A4

JOY_Y_PIN = A1

LEFT_MOTOR_CONTROLLER_PIN = 3

RIGHT_MOTOR_CONTROLLER_PIN = 6

SPEED_KNOB_PIN = A3

BUTTON_MODE_PIN = 2

STEERING_OFF_SWITCH_PIN = 4

BUTTON PINS: 7, 8, 9, 10, 11, 12 are forwards, left, right, left+forwards, right+forwards, backwards

RC PINS: 5, 6, 7, 8 are speed, turn, control, stop

#### rpi pico
JOY_X_PIN = 26

JOY_Y_PIN = 27

LEFT_MOTOR_CONTROLLER_PIN = 19

RIGHT_MOTOR_CONTROLLER_PIN = 21

SPEED_KNOB_PIN = 28

BUTTON_MODE_PIN = 5

STEERING_OFF_SWITCH_PIN = 9

BUTTON PINS: 7, 8, 9, 10, 11, 12 are forwards, left, right, left+forwards, right+forwards, backwards

RC PINS: 5, 6, 7, 8 are speed, turn, control, stop

#### esp32
JOY_X_PIN = 39

JOY_Y_PIN = 25

LEFT_MOTOR_CONTROLLER_PIN = 18

RIGHT_MOTOR_CONTROLLER_PIN = 21

SPEED_KNOB_PIN = 33

BUTTON_MODE_PIN = 23

STEERING_OFF_SWITCH_PIN=4

BUTTON PINS: 16, 17, 19, 26, 22, 25 are forwards, left, right, left+forwards, right+forwards, backwards

RC PINS: 16, 17, 19, 22 are speed, turn, control, stop

### Acknowledgements
* Arduino
* https://github.com/earlephilhower/arduino-pico
