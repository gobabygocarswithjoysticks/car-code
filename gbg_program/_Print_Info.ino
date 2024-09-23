int serialChecksum = 0;
boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    serialChecksum = 0;
    checksumPrintFlashString(F("{\"current values, millis:\": ")); serialChecksum += Serial.print(millis()); checksumPrintComma();
    checksumPrintFlashString(F("\"joyXVal\": ")); serialChecksum += Serial.print(joyXVal); checksumPrintComma();
    checksumPrintFlashString(F("\"joyYVal\": ")); serialChecksum += Serial.print(joyYVal); checksumPrintComma();
    checksumPrintFlashString(F("\"turnInput\": ")); serialChecksum += Serial.print(turnInput, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"speedInput\": ")); serialChecksum += Serial.print(speedInput, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"turnProcessed\": ")); serialChecksum += Serial.print(turnProcessed, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"speedProcessed\": ")); serialChecksum += Serial.print(speedProcessed, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"turnToDrive\": ")); serialChecksum += Serial.print(turnToDrive, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"speedToDrive\": ")); serialChecksum += Serial.print(speedToDrive, 4); checksumPrintComma();
    checksumPrintFlashString(F("\"leftMotorWriteVal\": ")); serialChecksum += Serial.print(leftMotorWriteVal); checksumPrintComma();
    checksumPrintFlashString(F("\"rightMotorWriteVal\": ")); serialChecksum += Serial.print(rightMotorWriteVal); checksumPrintComma();
    checksumPrintFlashString(F("\"speedKnobVal\": ")); serialChecksum += Serial.print(speedKnobVal); checksumPrintComma();
    checksumPrintFlashString(F("\"movementAllowed\": ")); serialChecksum += Serial.print(movementAllowed ? "true" : "false"); checksumPrintComma();
    checksumPrintFlashString(F("\"joyOK\": ")); serialChecksum += Serial.print(joyOK ? "true" : "false"); checksumPrintComma();
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
    checksumPrintFlashString(F("\"buttons\": ")); serialChecksum += Serial.print(buttonBits); checksumPrintComma();
    checksumPrintFlashString(F("\"b_m_p\": "));
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

    checksumPrintComma();
    checksumPrintFlashString(F("\"CHECKSUM\": ")); Serial.print(serialChecksum + 1 /*closing bracket*/ + count_digits(serialChecksum));
    Serial.println("}");
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  serialChecksum = 0;
  checksumPrintFlashString(F("{\"current settings, version:\": ")); serialChecksum += Serial.print(version_number); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_RIGHT\": "));  serialChecksum += Serial.print(CONTROL_RIGHT); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_CENTER_X\": "));  serialChecksum += Serial.print(CONTROL_CENTER_X); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_LEFT\": "));  serialChecksum += Serial.print(CONTROL_LEFT); checksumPrintComma();
  checksumPrintFlashString(F("\"X_DEADZONE\": "));  serialChecksum += Serial.print(X_DEADZONE); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_UP\": "));  serialChecksum += Serial.print(CONTROL_UP); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_CENTER_Y\": "));  serialChecksum += Serial.print(CONTROL_CENTER_Y); checksumPrintComma();
  checksumPrintFlashString(F("\"CONTROL_DOWN\": "));  serialChecksum += Serial.print(CONTROL_DOWN); checksumPrintComma();
  checksumPrintFlashString(F("\"Y_DEADZONE\": "));  serialChecksum += Serial.print(Y_DEADZONE); checksumPrintComma();
  checksumPrintFlashString(F("\"ACCELERATION_FORWARD\": "));  serialChecksum += Serial.print(ACCELERATION_FORWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"DECELERATION_FORWARD\": "));  serialChecksum += Serial.print(DECELERATION_FORWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"ACCELERATION_BACKWARD\": "));  serialChecksum += Serial.print(ACCELERATION_BACKWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"DECELERATION_BACKWARD\": "));  serialChecksum += Serial.print(DECELERATION_BACKWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"ACCELERATION_TURNING\": "));  serialChecksum += Serial.print(ACCELERATION_TURNING, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"DECELERATION_TURNING\": "));  serialChecksum += Serial.print(DECELERATION_TURNING, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"FASTEST_FORWARD\": "));  serialChecksum += Serial.print(FASTEST_FORWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"FASTEST_BACKWARD\": "));  serialChecksum += Serial.print(FASTEST_BACKWARD, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"TURN_SPEED\": "));  serialChecksum += Serial.print(TURN_SPEED, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"SCALE_TURNING_WHEN_MOVING\": "));  serialChecksum += Serial.print(SCALE_TURNING_WHEN_MOVING, 4); checksumPrintComma();
  checksumPrintFlashString(F("\"REVERSE_TURN_IN_REVERSE\": "));  serialChecksum += Serial.print(REVERSE_TURN_IN_REVERSE ? "true" : "false"); checksumPrintComma();
  checksumPrintFlashString(F("\"LEFT_MOTOR_CENTER\": "));  serialChecksum += Serial.print(LEFT_MOTOR_CENTER); checksumPrintComma();
  checksumPrintFlashString(F("\"LEFT_MOTOR_SLOW\": "));  serialChecksum += Serial.print(LEFT_MOTOR_SLOW); checksumPrintComma();
  checksumPrintFlashString(F("\"LEFT_MOTOR_FAST\": "));  serialChecksum += Serial.print(LEFT_MOTOR_FAST); checksumPrintComma();
  checksumPrintFlashString(F("\"RIGHT_MOTOR_CENTER\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_CENTER); checksumPrintComma();
  checksumPrintFlashString(F("\"RIGHT_MOTOR_SLOW\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_SLOW); checksumPrintComma();
  checksumPrintFlashString(F("\"RIGHT_MOTOR_FAST\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_FAST); checksumPrintComma();
  checksumPrintFlashString(F("\"USE_SPEED_KNOB\": "));  serialChecksum += Serial.print(USE_SPEED_KNOB ? "true" : "false"); checksumPrintComma();
  checksumPrintFlashString(F("\"SPEED_KNOB_SLOW_VAL\": "));  serialChecksum += Serial.print(SPEED_KNOB_SLOW_VAL); checksumPrintComma();
  checksumPrintFlashString(F("\"SPEED_KNOB_FAST_VAL\": "));  serialChecksum += Serial.print(SPEED_KNOB_FAST_VAL); checksumPrintComma();
  checksumPrintFlashString(F("\"SCALE_ACCEL_WITH_SPEED\": "));  serialChecksum += Serial.print(SCALE_ACCEL_WITH_SPEED ? "true" : "false"); checksumPrintComma();
  checksumPrintFlashString(F("\"SPEED_KNOB_PIN\": "));  serialChecksum += Serial.print(SPEED_KNOB_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"JOY_X_PIN\": "));  serialChecksum += Serial.print(JOY_X_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"JOY_Y_PIN\": "));  serialChecksum += Serial.print(JOY_Y_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"LEFT_MOTOR_CONTROLLER_PIN\": "));  serialChecksum += Serial.print(LEFT_MOTOR_CONTROLLER_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"RIGHT_MOTOR_CONTROLLER_PIN\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_CONTROLLER_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"ENABLE_STARTUP_PULSE\": "));  serialChecksum += Serial.print((ENABLE_STARTUP_PULSE ? "true" : "false")); checksumPrintComma();
  checksumPrintFlashString(F("\"LEFT_MOTOR_PULSE\": "));  serialChecksum += Serial.print(LEFT_MOTOR_PULSE); checksumPrintComma();
  checksumPrintFlashString(F("\"RIGHT_MOTOR_PULSE\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_PULSE); checksumPrintComma();
  checksumPrintFlashString(F("\"START_MOTOR_PULSE_TIME\": "));  serialChecksum += Serial.print(START_MOTOR_PULSE_TIME); checksumPrintComma();
  checksumPrintFlashString(F("\"JOY_CALIB_COUNT\": "));  serialChecksum += Serial.print(JOY_CALIB_COUNT); checksumPrintComma();
  checksumPrintFlashString(F("\"ENABLE_BUTTON_CTRL\": "));  serialChecksum += Serial.print((ENABLE_BUTTON_CTRL ? "true" : "false")); checksumPrintComma();
  checksumPrintFlashString(F("\"USE_BUTTON_MODE_PIN\": "));  serialChecksum += Serial.print((USE_BUTTON_MODE_PIN ? "true" : "false")); checksumPrintComma();
  checksumPrintFlashString(F("\"BUTTON_MODE_PIN\": "));  serialChecksum += Serial.print(BUTTON_MODE_PIN); checksumPrintComma();
  checksumPrintFlashString(F("\"NUM_DRIVE_BUTTONS\": "));  serialChecksum += Serial.print(NUM_DRIVE_BUTTONS); checksumPrintComma();

  for (byte db = 0; db < maxNumDriveButtons; db++) {
    checksumPrintFlashString(F("\"DRIVE_BUTTON_"));
    serialChecksum += Serial.print(db + 1);
    checksumPrintFlashString(F("\": ["));
    serialChecksum += Serial.print(driveButtons[db].pin);
    checksumPrintFlashString(F(","));
    serialChecksum += Serial.print(driveButtons[db].speed, 4);
    checksumPrintFlashString(F(","));
    serialChecksum += Serial.print(driveButtons[db].turn, 4);
    checksumPrintFlashString(F("], "));
  }
  checksumPrintFlashString(F("\"STEERING_OFF_SWITCH\": "));  serialChecksum += Serial.print((STEERING_OFF_SWITCH ? "true" : "false")); checksumPrintComma();
  checksumPrintFlashString(F("\"STEERING_OFF_SWITCH_PIN\": "));  serialChecksum += Serial.print(STEERING_OFF_SWITCH_PIN); checksumPrintComma();

#if defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ESP32)
  checksumPrintFlashString(F("\"CAR_WIFI_NAME\": "));  serialChecksum += Serial.print(CAR_WIFI_NAME); checksumPrintComma();
  checksumPrintFlashString(F("\"CAR_WIFI_PASSWORD\": "));  serialChecksum += Serial.print(CAR_WIFI_PASSWORD); checksumPrintComma();
#endif

  checksumPrintFlashString(F("\"CHECKSUM\": ")); Serial.print(serialChecksum + 1/*closing bracket*/ + count_digits(serialChecksum));
  Serial.println("}");

}

void checksumPrintComma() {
  serialChecksum += Serial.print(", ");
}
void checksumPrintFlashString(const __FlashStringHelper *ifsh) {
  serialChecksum += Serial.print(ifsh);
}

static int count_digits (int n) {
  if (n < 0) {
    n = -n;
  }
  if (n < 10) return 1;
  if (n < 100) return 2;
  if (n < 1000) return 3;
  if (n < 10000) return 4;
  return 5;
}
