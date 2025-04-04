int serialChecksum = 0;
boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    serialChecksum = 0;
    prnt(F("{\"current values, millis\": "), millis());
    prnt(F("\"joyXVal\": "), joyXVal);
    prnt(F("\"joyYVal\": "), joyYVal);
    prnt(F("\"turnInput\": "), turnInput);
    prnt(F("\"speedInput\": "), speedInput);
    prnt(F("\"turnProcessed\": "), turnProcessed);
    prnt(F("\"speedProcessed\": "), speedProcessed);
    prnt(F("\"turnToDrive\": "), turnToDrive);
    prnt(F("\"speedToDrive\": "), speedToDrive);
    prnt(F("\"leftMotorWriteVal\": "), leftMotorWriteVal);
    prnt(F("\"rightMotorWriteVal\": "), rightMotorWriteVal);
    prnt(F("\"speedKnobVal\": "), speedKnobVal);
    prnt(F("\"movementAllowed\": "), movementAllowed ? "true" : "false");
    prnt(F("\"joyOK\": "), joyOK ? "true" : "false");
    int buttonBits = 0;
    if (ENABLE_BUTTON_CTRL) {
      int button;
      for (button = 0; button < maxNumDriveButtons; button++) {
        if (digitalRead(driveButtons[button].pin) == LOW)
          bitSet(buttonBits, button);
      }

      // extra MSB set to 1 if buttons active, to indicate the size of maxNumDriveButtons
      if (ENABLE_BUTTON_CTRL) {
        if (USE_BUTTON_MODE_PIN) {
          if (digitalRead(BUTTON_MODE_PIN) == LOW)
            bitSet(buttonBits, button);
        } else { // mode pin use = false; buttons on
          bitSet(buttonBits, button);
        }
      } //else  buttons off


      if (ENABLE_BUTTON_CTRL && !(USE_BUTTON_MODE_PIN && digitalRead(BUTTON_MODE_PIN) == HIGH)) { // buttons on
        bitSet(buttonBits, button); // extra MSB set to 1, to indicate the size of maxNumDriveButtons
      }
    }
    prnt(F("\"buttons\": "), buttonBits);
    serialChecksum += Serial.print(F("\"b_m_p\": "));
    if (ENABLE_BUTTON_CTRL) {
      if (USE_BUTTON_MODE_PIN) {
        if (digitalRead(BUTTON_MODE_PIN) == LOW)
          serialChecksum += Serial.print("\"Y\"");
        else
          serialChecksum += Serial.print("\"N\"");
      } else { // mode pin use = false; buttons on
        serialChecksum += Serial.print("\"A\"");
      }
    } else { // buttons off
      serialChecksum += Serial.print("\"B\"");
    }

    serialChecksum += Serial.print(F("\", CHECKSUM\": "));
    Serial.print(serialChecksum + 1 /*closing bracket*/ + count_digits(serialChecksum));
    Serial.println("}");
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  serialChecksum = 0;
  prnt(F("\"current settings, version\": "), version_number);
  prnt(F("\"CONTROL_RIGHT\": "), CONTROL_RIGHT);
  prnt(F("\"CONTROL_CENTER_X\": "), CONTROL_CENTER_X);
  prnt(F("\"CONTROL_LEFT\": "), CONTROL_LEFT);
  prnt(F("\"X_DEADZONE\": "), X_DEADZONE);
  prnt(F("\"CONTROL_UP\": "), CONTROL_UP);
  prnt(F("\"CONTROL_CENTER_Y\": "), CONTROL_CENTER_Y);
  prnt(F("\"CONTROL_DOWN\": "), CONTROL_DOWN);
  prnt(F("\"Y_DEADZONE\": "), Y_DEADZONE);
  prnt(F("\"ACCELERATION_FORWARD\": "), ACCELERATION_FORWARD);
  prnt(F("\"DECELERATION_FORWARD\": "), DECELERATION_FORWARD);
  prnt(F("\"ACCELERATION_BACKWARD\": "), ACCELERATION_BACKWARD);
  prnt(F("\"DECELERATION_BACKWARD\": "), DECELERATION_BACKWARD);
  prnt(F("\"ACCELERATION_TURNING\": "), ACCELERATION_TURNING);
  prnt(F("\"DECELERATION_TURNING\": "), DECELERATION_TURNING);
  prnt(F("\"FASTEST_FORWARD\": "), FASTEST_FORWARD);
  prnt(F("\"FASTEST_BACKWARD\": "), FASTEST_BACKWARD);
  prnt(F("\"TURN_SPEED\": "), TURN_SPEED);
  prnt(F("\"SCALE_TURNING_WHEN_MOVING\": "), SCALE_TURNING_WHEN_MOVING);
  prnt(F("\"REVERSE_TURN_IN_REVERSE\": "), REVERSE_TURN_IN_REVERSE ? "true" : "false");
  prnt(F("\"LEFT_MOTOR_CENTER\": "), LEFT_MOTOR_CENTER);
  prnt(F("\"LEFT_MOTOR_SLOW\": "), LEFT_MOTOR_SLOW);
  prnt(F("\"LEFT_MOTOR_FAST\": "), LEFT_MOTOR_FAST);
  prnt(F("\"RIGHT_MOTOR_CENTER\": "), RIGHT_MOTOR_CENTER);
  prnt(F("\"RIGHT_MOTOR_SLOW\": "), RIGHT_MOTOR_SLOW);
  prnt(F("\"RIGHT_MOTOR_FAST\": "), RIGHT_MOTOR_FAST);
  prnt(F("\"USE_SPEED_KNOB\": "), USE_SPEED_KNOB ? "true" : "false");
  prnt(F("\"SPEED_KNOB_SLOW_VAL\": "), SPEED_KNOB_SLOW_VAL);
  prnt(F("\"SPEED_KNOB_FAST_VAL\": "), SPEED_KNOB_FAST_VAL);
  prnt(F("\"SCALE_ACCEL_WITH_SPEED\": "), SCALE_ACCEL_WITH_SPEED ? "true" : "false");
  prnt(F("\"SPEED_KNOB_PIN\": "), SPEED_KNOB_PIN);
  prnt(F("\"JOY_X_PIN\": "), JOY_X_PIN);
  prnt(F("\"JOY_Y_PIN\": "), JOY_Y_PIN);
#ifdef IS_PCB
  prnt(F("\"SWAP_MOTORS\": "), SWAP_MOTORS ? "true" : "false");
#else
  prnt(F("\"LEFT_MOTOR_CONTROLLER_PIN\": "), LEFT_MOTOR_CONTROLLER_PIN);
  prnt(F("\"RIGHT_MOTOR_CONTROLLER_PIN\": "), RIGHT_MOTOR_CONTROLLER_PIN);
#endif
  prnt(F("\"ENABLE_STARTUP_PULSE\": "), ENABLE_STARTUP_PULSE ? "true" : "false");
  prnt(F("\"LEFT_MOTOR_PULSE\": "), LEFT_MOTOR_PULSE);
  prnt(F("\"RIGHT_MOTOR_PULSE\": "), RIGHT_MOTOR_PULSE);
  prnt(F("\"START_MOTOR_PULSE_TIME\": "), START_MOTOR_PULSE_TIME);
  prnt(F("\"JOY_CALIB_COUNT\": "), JOY_CALIB_COUNT);
  prnt(F("\"ENABLE_BUTTON_CTRL\": "), ENABLE_BUTTON_CTRL ? "true" : "false");
  prnt(F("\"USE_BUTTON_MODE_PIN\": "), USE_BUTTON_MODE_PIN ? "true" : "false");
  prnt(F("\"BUTTON_MODE_PIN\": "), BUTTON_MODE_PIN);
  prnt(F("\"NUM_DRIVE_BUTTONS\": "), NUM_DRIVE_BUTTONS);

  for (byte db = 0; db < maxNumDriveButtons; db++) {
    serialChecksum += Serial.print(F("\"DRIVE_BUTTON_"));
    serialChecksum += Serial.print(db + 1);
    serialChecksum += Serial.print(F("\": ["));
    serialChecksum += Serial.print(driveButtons[db].pin);
    serialChecksum += Serial.print(",");
    serialChecksum += Serial.print(driveButtons[db].speed, 4);
    serialChecksum += Serial.print(",");
    serialChecksum += Serial.print(driveButtons[db].turn, 4);
    serialChecksum += Serial.print("], ");
  }
  prnt(F("\"STEERING_OFF_SWITCH\": "), STEERING_OFF_SWITCH ? "true" : "false");
  prnt(F("\"STEERING_OFF_SWITCH_PIN\": "), STEERING_OFF_SWITCH_PIN);

#if defined(RC_CONTROL)
  prnt(F("\"USE_RC_CONTROL\": "), USE_RC_CONTROL ? "true" : "false");
  prnt(F("\"SPEED_RC_PIN\": "), SPEED_RC_PIN);
  prnt(F("\"TURN_RC_PIN\": "), TURN_RC_PIN);
#endif 
#if defined(HAS_WIFI)
  prnt(F("\"CAR_WIFI_NAME\": "), CAR_WIFI_NAME);
  prnt(F("\"CAR_WIFI_PASSWORD\": "), CAR_WIFI_PASSWORD);
  prnt(F("\"USE_WIFI\": "), USE_WIFI ? "true" : "false");
#endif

  serialChecksum+=Serial.print(F("\"CHECKSUM\": ")); Serial.print(serialChecksum + 1/*closing bracket*/ + count_digits(serialChecksum));
  Serial.println("}");

}

template <typename T>
void prnt(const __FlashStringHelper *fsh, T value) {
  serialChecksum += Serial.print(fsh);
  serialChecksum += Serial.print(value);
  serialChecksum += Serial.print(", ");
}

int count_digits (int n) {
  if (n < 0) {
    n = -n;
  }
  if (n < 10) return 1;
  if (n < 100) return 2;
  if (n < 1000) return 3;
  if (n < 10000) return 4;
  return 5;
}
