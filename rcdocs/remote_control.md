At the request of many families and physical therapists, we added features to this code to allow remote control of the go baby go car.

* on/off switch
* on/off buttons
* joystick/button toggling
* RC receiver input

Please talk to a physical therapist about the best way to use a remote control to benefit the kid, and take a look at these resources: (remote control user manual coming soon)

## Instructions for adding a keyfob on/off toggle
### Description: 
Press one button on a keyfob remote to stop the car, and another button to start it again.

### Advantages: 
simple, cheap

### Disadvantages: 
car won't turn off if the transmitter is out of range

### Instructions:

## Instructions for adding a keyfob switch
Description: Modify a keyfob remote with a switch that stops the car or allows it to drive.

Advantages: cheap, car will turn off if the transmitter is out of range

Disadvantages: modifying the keyfob makes it a bit more complicated

### Instructions:

## Instructions for adding a keyfob that can drive the car
### Description:
Use a keyfob remote to override and drive the car. One button will toggle between car control and remote control, and three buttons make the car drive forward, left, and right.
### Advantages: 
simple, cheap

## Disadvantages: 
car won't turn off if the transmitter is out of range, challenging to drive precisely, prevents the car from being driven by buttons on the car

### Instructions:
Get a keyfob remote and receiver module with four buttons. (search for 2262/2272 4CH key remote control).

Connect the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively. Connect the D0, D1, D2, and D3 pins of the receiver module to 4 pins of the Arduino.

check "enable button ctrl"

check "use button mode pin"

set "button mode pin" to the pin connected to the receiver module D0 pin

check "button mode toggle"

check "buttons active high"

set "num drive buttons" to 3

set "drive button 1" "pin" to the pin connected to the receiver module D1 pin

set "drive button 1" "speed" to 0

set "drive button 1" "turn" to 1

set "drive button 2" "pin" to the pin connected to the receiver module D2 pin

set "drive button 2" "speed" to 1

set "drive button 2" "turn" to 0

set "drive button 3" "pin" to the pin connected to the receiver module D3 pin

set "drive button 3" "speed" to 0

set "drive button 3" "turn" to -1

### How to use:
Press button A on the keyfob to toggle between car control and remote control. When in remote control mode, press button B to turn right, button C to drive forward, and button D to turn left.


## Instructions for adding an RC receiver
### Advantages: 
can drive the car precisely, car will turn off if the transmitter is out of range

###Disadvantages: 
more expensive, more complicated

