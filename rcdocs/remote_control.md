At the request of many families and physical therapists, we added features to this code to allow remote control of the go baby go car.

* on/off switch
* on/off buttons
* joystick/button toggling
* RC receiver input

Please talk to a physical therapist about the best way to use a remote control to benefit the kid, and take a look at these resources: (remote control user manual coming soon)

# TODO: RC control is still in development and has not yet been fully tested.

---
## keyfob on/off toggle
### Description: 
Press one button on a keyfob remote to stop the car, and another button to start it again.

### Advantages: 
simple, cheap

### Disadvantages: 
car won't turn off if the transmitter is out of range

### Instructions:
Get a keyfob remote and receiver module with at least 2 (probably 4) buttons. (search for 2262/2272 4CH key remote control).

Connect the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively. 

Connect the D0 and D3 pins of the receiver module to 2 pins of the Arduino.

check "use on off buttons"

set "on button" to the pin connected to the receiver module D3 pin

set "off button" to the pin connected to the receiver module D0 pin

check "on off buttons active high"

### How to Use:
Press button A on the keyfob to turn the car off, and button D to turn it back on.

---
## keyfob on/off switch
### Description: 
Modify a keyfob remote with a switch that stops the car or allows it to drive.

### Advantages: 
cheap, car will turn off if the transmitter is out of range

### Disadvantages: 
modifying the keyfob makes it a bit more complicated

### Instructions:
Get a keyfob remote and receiver module with at least 1 button. (search for 2262/2272 4CH key remote control).

Open the keyfob remote and wire a toggle switch parallel to one of the buttons. When the switch is on the remote should transmit continuously. Check that it is legal to transmit for minutes at a time on the frequency of the remote in your country. Optionally, replace the battery with a larger one.

Connect the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively.

Connect an output pin of the receiver module to a pin of the Arduino.

check "use stop switch"

set "stop pin" to the pin connected to the receiver module output pin

uncheck "stop pin high"

If you want the car to be able to drive without the remote, check "no stop until start". If "no stop until start" is checked, the car will be able to drive until the switch on the remote is turned on then off again.

If "no stop until start" is not checked, the car will not be able to drive until the switch on the remote is turned on. This means the remote is required for using the car.

### How to use:
Turn on the switch on the remote to allow the car to drive. Turn it off to stop the car. If the car goes out of range of the remote, it will stop driving.

---
## RC receiver
### Advantages: 
can drive the car precisely, car will turn off if the transmitter is out of range

### Disadvantages: 
more expensive, more complicated

### Instructions:
Get an RC transmitter with at least 4 channels, 1 joystick, and 2 switches. This is probably sold as a 6-channel transmitter. Also, get a receiver that is compatible with the transmitter.

Connect the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively.

Connect 4 signal pins from the receiver to 4 pins of the Arduino.

Program the transmitter to send the following signals on 4 channels:
* Forward/backward joystick axis with center at 1500, forward at 2000, backward at 1000
* Left/right joystick axis with center at 1500, left at 1000, right at 2000
* "control" switch with off under 1300 and on above 1700
* "stop" switch with off under 1300 and on above 1700

---
## keyfob that can drive the car
### Description:
Use a keyfob remote to override and drive the car. One button will toggle between car control and remote control, and three buttons make the car drive forward, left, and right.
### Advantages: 
simple, cheap

## Disadvantages: 
car won't turn off if the transmitter is out of range, challenging to drive precisely, prevents the car from being driven by buttons on the car, car decelerates instead of stopping immediately

### Instructions:
Get a keyfob remote and receiver module with four buttons. (search for 2262/2272 4CH key remote control).

Connect the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively. 

Connect the D0, D1, D2, and D3 pins of the receiver module to 4 pins of the Arduino.

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
