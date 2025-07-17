At the request of many families and physical therapists, we added features to this code to allow remote control of the go baby go car.

* on/off switch
* on/off buttons
* joystick/button toggling
* RC receiver input

Please talk to a physical therapist about the best way to use a remote control to benefit the kid, and take a look at these resources: 

(remote control user manual coming soon)

Questions about the car software or these instructions? Please email us at gobabygocarswithjoysticks@gmail.com

## Notes on Wifi/website remote control

This feature is available with ESP32, Pico 1W, and Pico 2W processors.

Check the “use wifi” setting on the programmer website, then follow the instructions and QR codes on the programmer website.

The Pico will create a wifi network and will serve a webpage that allows you to control the car from a phone, tablet, or computer.

The wifi range is unreliable (sometimes measured as 10 feet sometimes as over 100 feet).

No hardware is required for this method of remote control.

## Notes on keyfob remote buttons:
The pins on the receivers are consistently labeled D0, D1, D2, and D3. The buttons on the keyfob remotes are not consistently labeled, but D0 is usually the rightmost button and D3 is usually the leftmost button.

```
  2  0
3  1
```
These instructions will use the numbers instead of the inconsistent letter labels on the keyfob remotes.

## Notes on keyfob remote range:
If the range is really short, try adjusting the screw on the receiver. I tried to tune it to the middle of the range in which it received a signal. You could also try adjusting the screw while someone else walks away with the remote. Remove the screwdriver from the screw before evaluating each adjustment to the screw, since the receiver is effected by the screwdriver touching the screw.

---
## keyfob on/off toggle
### Description: 
Press one button on a keyfob remote to stop the car, and another button to start it again.

### Advantages: 
* simple
* cheap

### Disadvantages: 
* car won't turn off if the transmitter is out of range

### Instructions:
Get a keyfob remote and receiver module with at least 2 (probably 4) buttons. (search for 2262/2272 4CH key remote control).

It is recommended to open up the keyfob and add solder bridges to the 8 addressing pins to set a unique address for the remote. This will prevent other remotes from interfering with the car. Try to give each car that you build a different address. Also add solder bridges to the 8 addressing pins on the receiver module to match the address of the remote.

Wire the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively. 

Wire the D0 and D3 pins of the receiver module to 2 pins of the Arduino.

Using the programming website (connect to car then press "show all"):
* check "use on off buttons"
* set "on button" to the pin connected to the receiver module D3 pin
* set "off button" to the pin connected to the receiver module D0 pin
* check "on off buttons active high"

### How to Use:
Press button D0 on the keyfob to turn the car off, and button D3 to turn it back on.

---
## keyfob on/off switch
### Description: 
Modify a keyfob remote with a switch that stops the car or allows it to drive.

### Advantages: 
* cheap
* car will turn off if the transmitter is out of range

### Disadvantages: 
* modifying the keyfob makes it more complicated
* the signal from the keyfob needs to be continuous, and the keyfob I tested had a continuous signal for 20 feet, then a blinking, inconsistent signal out to 100 feet because of trouble transmitting longer distances. The car would only be useful within the shorter 20 foot range.

### Instructions:
Get a keyfob remote and receiver module with at least 1 button. (search for 2262/2272 4CH key remote control).

**Modify the keyfob to add a switch:**

Open the keyfob remote and wire a toggle switch parallel to one of the buttons. When the switch is on the remote should transmit continuously. Check that it is legal to transmit for minutes at a time on the frequency of the remote in your country. Optionally, replace the battery with a larger one to give a longer battery life.

It is recommended to open up the keyfob and add solder bridges to the 8 addressing pins to set a unique address for the remote. This will prevent other remotes from interfering with the car. Try to give each car that you build a different address. Also add solder bridges to the 8 addressing pins on the receiver module to match the address of the remote.

Wire the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively.

Wire an output pin of the receiver module to a pin of the Arduino.

Using the programming website (connect to car then press "show all"):
* check "use stop switch"
* set "stop pin" to the pin connected to the receiver module output pin
* uncheck "stop pin high"
* set "no stop until start"
  * If you want the car to be able to drive without the remote, check "no stop until start". If "no stop until start" is checked, the car will be able to drive until the switch on the remote is turned on then off again.
  * If "no stop until start" is not checked, the car will not be able to drive until the switch on the remote is turned on. This means the remote is required for using the car.

### How to use:
Turn on the switch on the remote to allow the car to drive. Turn it off to stop the car. If the car goes out of range of the remote, it will stop driving.

---
## keyfob that can drive the car
### Description:
Use a keyfob remote to override and drive the car. One button will toggle between car control and remote control, and three buttons make the car drive forward, left, and right.
### Advantages: 
* simple
* cheap

## Disadvantages: 
* car won't turn off if the transmitter is out of range
* challenging to drive precisely
* prevents the car from being driven by buttons on the car
* car decelerates instead of stopping immediately
* the signal from the keyfob needs to be continuous, and the keyfob I tested had a continuous signal for 20 feet, then a blinking, inconsistent signal out to 100 feet because of trouble transmitting longer distances. The car could only be driven within the shorter 20 foot range.

### Instructions:
Get a keyfob remote and receiver module with four buttons. (search for 2262/2272 4CH key remote control).

It is recommended to open up the keyfob and add solder bridges to the 8 addressing pins to set a unique address for the remote. This will prevent other remotes from interfering with the car. Try to give each car that you build a different address. Also add solder bridges to the 8 addressing pins on the receiver module to match the address of the remote.

Wire the receiver module GND and 5V pins to the Arduino GND and 5V pins, respectively. 

Wire the D0, D1, D2, and D3 pins of the receiver module to 4 pins of the Arduino.

Using the programming website (connect to car then press "show all"):
* check "enable button ctrl"
* check "use button mode pin"
* set "button mode pin" to the pin connected to the receiver module D0 pin
* check "button mode toggle"
* check "buttons active high"
* set "num drive buttons" to 3
* drive button 1
  * set "drive button 1" "pin" to the pin connected to the receiver module D1 pin
  * set "drive button 1" "speed" to 0
  * set "drive button 1" "turn" to 1
* drive button 2
  * set "drive button 2" "pin" to the pin connected to the receiver module D2 pin
  * set "drive button 2" "speed" to 1
  * set "drive button 2" "turn" to 0
* drive button 3
  * set "drive button 3" "pin" to the pin connected to the receiver module D3 pin
  * set "drive button 3" "speed" to 0
  * set "drive button 3" "turn" to -1

### How to use:
Press button D0 on the keyfob to toggle between car control and remote control. When in remote control mode, press button D1 to turn right, button D2 to drive forward, and button D3 to turn left.

---

## RC receiver
### Advantages: 
* drive the car precisely with the remote control
* car will turn off if the transmitter is out of range

### Disadvantages: 
* more expensive
* more complicated

### Instructions:
Get a RC transmitter with at least 4 channels, 1 joystick, and 2 switches. This is probably sold as a 6-channel transmitter. Also, get a receiver that is compatible with the transmitter.

Wire the receiver module GND and power pins to the Arduino GND and 5V pins, respectively.

Wire 4 signal pins from the receiver to 4 pins of the Arduino.

Program the transmitter to send the following signals on 4 channels:
* Forward/backward joystick axis with center at 1500, forward at 2000, backward at 1000
* Left/right joystick axis with center at 1500, left at 1000, right at 2000
* "control" switch with off under 1300 and on above 1700
* "stop" switch with off under 1300 and on above 1700


Using the programming website (connect to car then press "show all"):
* check "use rc"
* set "rc speed pin" to the pin connected to the receiver channel for the forward/backward joystick axis
* set "rc turn pin" to the pin connected to the receiver channel for the left/right joystick axis
* set "rc control pin" to the pin connected to the receiver channel for the "control" switch
* set "rc stop pin" to the pin connected to the receiver channel for the "stop" switch
* set "rc inactive until connected"
  * If you want the car to be able to drive without the remote, check "rc inactive until connected". If "rc inactive until connected" is checked, the car will be able to drive until the transmitter is first turned on. After that, until the car is turned off, it will stop if the transmitter is turned off or goes out of range.
  * If "rc inactive until connected" is not checked, the car will not be able to drive until the transmitter is turned on. This means the remote is required for using the car.

### How to use:
When the "control" switch is on, the joystick on the transmitter's joystick can be used to drive the car. When the "control" switch is off, the joystick or buttons on the car can be used to drive the car.

When the "stop" switch is on, the car will stop driving.

If the transmitter is turned off or goes out of range, the car will stop driving.
