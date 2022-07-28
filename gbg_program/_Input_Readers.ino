
/**
  This function reads an axis of a joystick and scales the input (use two for a two axis joystick)
*/
float InputReader_JoystickAxis(int reading, int neg, int cent, int pos, int deadzone) {
  deadzone = max(deadzone, 0);
  ////////// scale joystick values from adjustible range to -1 to 1
  float output = 0;
  if (neg < pos) { //if the value for when the joystick is towards the left is less then the value for when the joystick is towards the right...
    if (reading < cent - deadzone)
      output = floatMap(reading, neg, cent - deadzone, -1, 0);
    if (reading > cent + deadzone)
      output = floatMap(reading, pos, cent + deadzone, 1, 0);
  }
  if (neg > pos) { // joystick axis is backwards
    if (reading > cent + deadzone)
      output = floatMap(reading, neg, cent + deadzone, -1, 0);
    if (reading < cent - deadzone)
      output = floatMap(reading, pos, cent - deadzone, 1, 0);
  }
  return constrain(output, -1, 1);
}

/**
  This function reads a set of buttons and edits turnInput and speedInput.
*/
void InputReader_Buttons(boolean enable, boolean resetValues, byte numDriveButtons, ButtonDriveConfig driveButtons[], float& turnInput, float& speedInput, boolean buttonState) {
  if (enable) {
    if (resetValues) {
      speedInput = 0;
      turnInput = 0; //overwrite other inputs and make them have no effect
    } //if reset values if false, buttons just add to
    //buttons for drive control add to the joystick's value, and add with each other.
    for (byte button = 0; button < numDriveButtons; button++) {
      if (digitalRead(driveButtons[button].pin) == buttonState) {
        speedInput += driveButtons[button].speed;
        turnInput += driveButtons[button].turn;
      }
    }
  }
}
