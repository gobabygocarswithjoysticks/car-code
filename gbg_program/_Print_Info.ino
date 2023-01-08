int serialChecksum = 0;
boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    serialChecksum = 0;
    serialChecksum += Serial.print(F("{\"current values, millis:\": ")); serialChecksum += Serial.print(millis()); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"joyXVal\": ")); serialChecksum += Serial.print(joyXVal); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"joyYVal\": ")); serialChecksum += Serial.print(joyYVal); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"turnInput\": ")); serialChecksum += Serial.print(turnInput, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"speedInput\": ")); serialChecksum += Serial.print(speedInput, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"turnProcessed\": ")); serialChecksum += Serial.print(turnProcessed, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"speedProcessed\": ")); serialChecksum += Serial.print(speedProcessed, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"turnToDrive\": ")); serialChecksum += Serial.print(turnToDrive, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"speedToDrive\": ")); serialChecksum += Serial.print(speedToDrive, 4); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"leftMotorWriteVal\": ")); serialChecksum += Serial.print(leftMotorWriteVal); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"rightMotorWriteVal\": ")); serialChecksum += Serial.print(rightMotorWriteVal); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"speedKnobVal\": ")); serialChecksum += Serial.print(speedKnobVal); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"movementAllowed\": ")); serialChecksum += Serial.print(movementAllowed ? "true" : "false"); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"joyOK\": ")); serialChecksum += Serial.print(joyOK ? "true" : "false"); serialChecksum += Serial.print(", ");
    serialChecksum += Serial.print(F("\"CHECKSUM\": ")); Serial.print(serialChecksum + 1 /*closing bracket*/ + count_digits(serialChecksum));
    Serial.println("}");
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  serialChecksum = 0;
  serialChecksum += Serial.print(F("{\"current settings, version:\": ")); serialChecksum += Serial.print(version_number); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_RIGHT\": "));  serialChecksum += Serial.print(CONTROL_RIGHT); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_CENTER_X\": "));  serialChecksum += Serial.print(CONTROL_CENTER_X); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_LEFT\": "));  serialChecksum += Serial.print(CONTROL_LEFT); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"X_DEADZONE\": "));  serialChecksum += Serial.print(X_DEADZONE); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_UP\": "));  serialChecksum += Serial.print(CONTROL_UP); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_CENTER_Y\": "));  serialChecksum += Serial.print(CONTROL_CENTER_Y); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"CONTROL_DOWN\": "));  serialChecksum += Serial.print(CONTROL_DOWN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"Y_DEADZONE\": "));  serialChecksum += Serial.print(Y_DEADZONE); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"ACCELERATION_FORWARD\": "));  serialChecksum += Serial.print(ACCELERATION_FORWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"DECELERATION_FORWARD\": "));  serialChecksum += Serial.print(DECELERATION_FORWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"ACCELERATION_BACKWARD\": "));  serialChecksum += Serial.print(ACCELERATION_BACKWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"DECELERATION_BACKWARD\": "));  serialChecksum += Serial.print(DECELERATION_BACKWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"ACCELERATION_TURNING\": "));  serialChecksum += Serial.print(ACCELERATION_TURNING, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"DECELERATION_TURNING\": "));  serialChecksum += Serial.print(DECELERATION_TURNING, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"FASTEST_FORWARD\": "));  serialChecksum += Serial.print(FASTEST_FORWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"FASTEST_BACKWARD\": "));  serialChecksum += Serial.print(FASTEST_BACKWARD, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"TURN_SPEED\": "));  serialChecksum += Serial.print(TURN_SPEED, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"SCALE_TURNING_WHEN_MOVING\": "));  serialChecksum += Serial.print(SCALE_TURNING_WHEN_MOVING, 4); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"REVERSE_TURN_IN_REVERSE\": "));  serialChecksum += Serial.print(REVERSE_TURN_IN_REVERSE ? "true" : "false"); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"LEFT_MOTOR_CENTER\": "));  serialChecksum += Serial.print(LEFT_MOTOR_CENTER); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"LEFT_MOTOR_SLOW\": "));  serialChecksum += Serial.print(LEFT_MOTOR_SLOW); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"LEFT_MOTOR_FAST\": "));  serialChecksum += Serial.print(LEFT_MOTOR_FAST); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"RIGHT_MOTOR_CENTER\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_CENTER); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"RIGHT_MOTOR_SLOW\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_SLOW); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"RIGHT_MOTOR_FAST\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_FAST); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"USE_SPEED_KNOB\": "));  serialChecksum += Serial.print(USE_SPEED_KNOB ? "true" : "false"); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"SPEED_KNOB_SLOW_VAL\": "));  serialChecksum += Serial.print(SPEED_KNOB_SLOW_VAL); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"SPEED_KNOB_FAST_VAL\": "));  serialChecksum += Serial.print(SPEED_KNOB_FAST_VAL); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"SCALE_ACCEL_WITH_SPEED\": "));  serialChecksum += Serial.print(SCALE_ACCEL_WITH_SPEED ? "true" : "false"); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"SPEED_KNOB_PIN\": "));  serialChecksum += Serial.print(SPEED_KNOB_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"JOY_X_PIN\": "));  serialChecksum += Serial.print(JOY_X_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"JOY_Y_PIN\": "));  serialChecksum += Serial.print(JOY_Y_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"LEFT_MOTOR_CONTROLLER_PIN\": "));  serialChecksum += Serial.print(LEFT_MOTOR_CONTROLLER_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"RIGHT_MOTOR_CONTROLLER_PIN\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_CONTROLLER_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"LEFT_MOTOR_PULSE\": "));  serialChecksum += Serial.print(LEFT_MOTOR_PULSE); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"RIGHT_MOTOR_PULSE\": "));  serialChecksum += Serial.print(RIGHT_MOTOR_PULSE); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"START_MOTOR_PULSE_TIME\": "));  serialChecksum += Serial.print(START_MOTOR_PULSE_TIME); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"ENABLE_STARTUP_PULSE\": "));  serialChecksum += Serial.print((ENABLE_STARTUP_PULSE ? "true" : "false")); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"JOY_CALIB_COUNT\": "));  serialChecksum += Serial.print(JOY_CALIB_COUNT); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"ENABLE_BUTTON_CTRL\": "));  serialChecksum += Serial.print((ENABLE_BUTTON_CTRL ? "true" : "false")); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"USE_BUTTON_MODE_PIN\": "));  serialChecksum += Serial.print((USE_BUTTON_MODE_PIN ? "true" : "false")); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"BUTTON_MODE_PIN\": "));  serialChecksum += Serial.print(BUTTON_MODE_PIN); serialChecksum += Serial.print(", ");
  serialChecksum += Serial.print(F("\"NUM_DRIVE_BUTTONS\": "));  serialChecksum += Serial.print(NUM_DRIVE_BUTTONS); serialChecksum += Serial.print(", ");

  for (byte db = 0; db < maxNumDriveButtons; db++) {
    serialChecksum += Serial.print(F("\"DRIVE_BUTTON_"));
    serialChecksum += Serial.print(db + 1);
    serialChecksum += Serial.print("\": [");
    serialChecksum += Serial.print(driveButtons[db].pin);
    serialChecksum += Serial.print(",");
    serialChecksum += Serial.print(driveButtons[db].speed, 4);
    serialChecksum += Serial.print(",");
    serialChecksum += Serial.print(driveButtons[db].turn, 4);
    serialChecksum += Serial.print("], ");
  }

  serialChecksum += Serial.print(F("\"CHECKSUM\": ")); Serial.print(serialChecksum + 1/*closing bracket*/ + count_digits(serialChecksum));
  Serial.println("}");

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
