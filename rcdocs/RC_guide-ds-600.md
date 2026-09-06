# Instructions for using the remote:

![rc-controller.drawio.png](https://raw.githubusercontent.com/gobabygocarswithjoysticks/car-code/refs/heads/main/rcdocs/ds-600-5.drawio.png)

![rc-controller.drawio.png](https://raw.githubusercontent.com/gobabygocarswithjoysticks/car-code/refs/heads/main/rcdocs/ds-600-6.drawio.png)

If the remote is turned from on to off or the car goes out of the remote's range, the car will stop until the remote is connected again so that the car can't run away.

### These are the controls for the remote:

* __Joystick__: only the right joystick controls the car (the left joystick does nothing)
* __Override Button__: when this is pressed, the remote control has complete control over driving the car
    * If channel 5 is used for the control pin, pushing button 5 will override and pushing button 5 again will stop overriding.
    * If channel 6 is used for the control pin, holding button 6 will override and releasing button 6 will stop overriding.
* __Stop Button (3)__: when this is pressed, the car will stop driving (abruptly, not smoothly)
* __Go Button (4)__: when this is pressed, the car is allowed to start driving

When neither the Override nor Stop buttons are activated, the car will drive according to the mode it was programmed with (see below)

The remote control also has a power button. When not using the car, turn the remote off.

Any other switches or controls on the remote control have no function. Make sure not to press the "OK" trigger button, since it turns on a "cruise control" setting designed for remote control boats that is not useful for a go baby go car.

# There are 3 decisions about how the remote control should work that need to be made when the car is being programmed. 

## 1. Override Channel
The first decision has to be made when the remote is wired to the car.

### If channel 5 is used for the control pin, pushing button 5 will override and pushing button 5 again will stop overriding.

### If channel 6 is used for the control pin, holding button 6 will override and releasing button 6 will stop overriding.

## 2. Mode: 
The second decision is about the "mode" setting. There are 3 choices:

### Standard Mode
The kid has complete control of the car in this mode unless the caregiver uses the override or stop switches on the remote. The standard mode is best for kids who don’t need much intervention or for families who want the simplest option for the remote. In standard mode, there is the sharpest division between who is driving the car: either the kid drives the car completely by themselves, or the caregiver uses the override switch and has complete control over the car.

### Shared Mode
In this mode, the remote influences the driving of the car but does not completely override it. This mode is kind of like walking next to a kid's car and physically nudging it to redirect it while the kid is still driving.

(In this mode, the kid’s controls are summed with the remote’s joystick so that both inputs have influence).

Shared mode can be used to help a kid steer without needing to use the remote override switch.
Compared to flipping the override switch, driving the car remotely for a second to turn it, and flipping the switch back, shared mode allows the caregiver to just give a small turning input in an easier and smoother way.
An advantage of this mode is that it can give easy, smooth, nudges to the car to redirect it away from walls.

For button cars, the advantage of shared mode is that the caregiver would be able to spin the car in place when the child isn’t pressing the button, and steer the car when the child is pressing the button.

A disadvantage of shared mode is that it could be confusing for the kid when the car moves differently from how the kid is using the controls and this could make it harder for the kid to learn to drive independently. 

### Steered Mode
This mode is designed primarily for button cars (since it ignores directional input from the kid).

In steered mode, the remote drives the car, except the car only moves if the kid is also pushing on the controls. (otherwise, the car slows to a stop). 

A tip for caregivers using this mode is to always point the remote’s joystick in whatever direction (forwards, turning, backwards) they want the car to go once the kid presses the button on the car.

## 3. Whether the car operates without the remote present:
The third decision to be made about how the car is programmed concerns what the car should do if the car is turned on without the remote control also being turned on. There are 2 choices:

### Can't drive without remote

With this option, the car will not move unless the remote is also turned on.

Choosing this option means the remote acts like a car key.
 
This option would add a safety mechanism to help prevent any children in the household from driving the car unsupervised. 

When the remote is turned on, the car will drive in whatever mode the car had been programmed with (standard, shared, or steered).

### Can drive without remote

With this option, turning the car on without the remote makes the car revert to Standard mode, so the kid has full control of the car and there's no remote override.

If the remote is later turned on, the car will drive in whatever mode the car had been programmed with.

The advantage of this option is that the car can always be used, even if the remote is out of charge, misplaced, or forgotten.

If the remote is then turned back off, the car stops, as it always does when the remote is turned from on to off.
