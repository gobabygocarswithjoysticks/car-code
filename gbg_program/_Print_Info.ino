boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    Serial.print(F("{\"current values, millis:\": ")); Serial.print(millis()); Serial.print(", ");
    Serial.print(F("\"joyXVal\": ")); Serial.print(joyXVal); Serial.print(", ");
    Serial.print(F("\"joyYVal\": ")); Serial.print(joyYVal); Serial.print(", ");
    Serial.print(F("\"turnInput\": ")); Serial.print(turnInput, 4); Serial.print(", ");
    Serial.print(F("\"speedInput\": ")); Serial.print(speedInput, 4); Serial.print(", ");
    Serial.print(F("\"turnProcessed\": ")); Serial.print(turnProcessed, 4); Serial.print(", ");
    Serial.print(F("\"speedProcessed\": ")); Serial.print(speedProcessed, 4); Serial.print(", ");
    Serial.print(F("\"turnToDrive\": ")); Serial.print(turnToDrive, 4); Serial.print(", ");
    Serial.print(F("\"speedToDrive\": ")); Serial.print(speedToDrive, 4); Serial.print(", ");
    Serial.print(F("\"leftMotorWriteVal\": ")); Serial.print(leftMotorWriteVal); Serial.print(", ");
    Serial.print(F("\"rightMotorWriteVal\": ")); Serial.print(rightMotorWriteVal); Serial.print(", ");
    Serial.print(F("\"speedKnobVal\": ")); Serial.print(speedKnobVal); Serial.print(", ");
    Serial.print(F("\"movementAllowed\": ")); Serial.print(movementAllowed ? "true" : "false"); Serial.print(", ");
    Serial.print(F("\"joyOK\": ")); Serial.print(joyOK ? "true" : "false");// Serial.print(", ");
    Serial.println("}");
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  Serial.print(F("{\"current settings, version:\": ")); Serial.print(version_number); Serial.print(", ");
  Serial.print(F("\"CONTROL_RIGHT\": "));  Serial.print(CONTROL_RIGHT); Serial.print(", ");
  Serial.print(F("\"CONTROL_CENTER_X\": "));  Serial.print(CONTROL_CENTER_X); Serial.print(", ");
  Serial.print(F("\"CONTROL_LEFT\": "));  Serial.print(CONTROL_LEFT); Serial.print(", ");
  Serial.print(F("\"X_DEADZONE\": "));  Serial.print(X_DEADZONE); Serial.print(", ");
  Serial.print(F("\"CONTROL_UP\": "));  Serial.print(CONTROL_UP); Serial.print(", ");
  Serial.print(F("\"CONTROL_CENTER_Y\": "));  Serial.print(CONTROL_CENTER_Y); Serial.print(", ");
  Serial.print(F("\"CONTROL_DOWN\": "));  Serial.print(CONTROL_DOWN); Serial.print(", ");
  Serial.print(F("\"Y_DEADZONE\": "));  Serial.print(Y_DEADZONE); Serial.print(", ");
  Serial.print(F("\"ACCELERATION_FORWARD\": "));  Serial.print(ACCELERATION_FORWARD, 4); Serial.print(", ");
  Serial.print(F("\"DECELERATION_FORWARD\": "));  Serial.print(DECELERATION_FORWARD, 4); Serial.print(", ");
  Serial.print(F("\"ACCELERATION_BACKWARD\": "));  Serial.print(ACCELERATION_BACKWARD, 4); Serial.print(", ");
  Serial.print(F("\"DECELERATION_BACKWARD\": "));  Serial.print(DECELERATION_BACKWARD, 4); Serial.print(", ");
  Serial.print(F("\"ACCELERATION_TURNING\": "));  Serial.print(ACCELERATION_TURNING, 4); Serial.print(", ");
  Serial.print(F("\"DECELERATION_TURNING\": "));  Serial.print(DECELERATION_TURNING, 4); Serial.print(", ");
  Serial.print(F("\"FASTEST_FORWARD\": "));  Serial.print(FASTEST_FORWARD, 4); Serial.print(", ");
  Serial.print(F("\"FASTEST_BACKWARD\": "));  Serial.print(FASTEST_BACKWARD, 4); Serial.print(", ");
  Serial.print(F("\"TURN_SPEED\": "));  Serial.print(TURN_SPEED, 4); Serial.print(", ");
  Serial.print(F("\"SCALE_TURNING_WHEN_MOVING\": "));  Serial.print(SCALE_TURNING_WHEN_MOVING, 4); Serial.print(", ");
  Serial.print(F("\"REVERSE_TURN_IN_REVERSE\": "));  Serial.print(REVERSE_TURN_IN_REVERSE ? "true" : "false"); Serial.print(", ");
  Serial.print(F("\"LEFT_MOTOR_CENTER\": "));  Serial.print(LEFT_MOTOR_CENTER); Serial.print(", ");
  Serial.print(F("\"LEFT_MOTOR_SLOW\": "));  Serial.print(LEFT_MOTOR_SLOW); Serial.print(", ");
  Serial.print(F("\"LEFT_MOTOR_FAST\": "));  Serial.print(LEFT_MOTOR_FAST); Serial.print(", ");
  Serial.print(F("\"RIGHT_MOTOR_CENTER\": "));  Serial.print(RIGHT_MOTOR_CENTER); Serial.print(", ");
  Serial.print(F("\"RIGHT_MOTOR_SLOW\": "));  Serial.print(RIGHT_MOTOR_SLOW); Serial.print(", ");
  Serial.print(F("\"RIGHT_MOTOR_FAST\": "));  Serial.print(RIGHT_MOTOR_FAST); Serial.print(", ");
  Serial.print(F("\"USE_SPEED_KNOB\": "));  Serial.print(USE_SPEED_KNOB ? "true" : "false"); Serial.print(", ");
  Serial.print(F("\"SPEED_KNOB_SLOW_VAL\": "));  Serial.print(SPEED_KNOB_SLOW_VAL); Serial.print(", ");
  Serial.print(F("\"SPEED_KNOB_FAST_VAL\": "));  Serial.print(SPEED_KNOB_FAST_VAL); Serial.print(", ");
  Serial.print(F("\"SCALE_ACCEL_WITH_SPEED\": "));  Serial.print(SCALE_ACCEL_WITH_SPEED ? "true" : "false"); Serial.print(", ");
  Serial.print(F("\"SPEED_KNOB_PIN\": "));  Serial.print(SPEED_KNOB_PIN); Serial.print(", ");
  Serial.print(F("\"PRINT_VARIABLES_INTERVAL_MILLIS\": "));  Serial.print(PRINT_VARIABLES_INTERVAL_MILLIS); Serial.print(", ");
  Serial.print(F("\"JOY_X_PIN\": "));  Serial.print(JOY_X_PIN); Serial.print(", ");
  Serial.print(F("\"JOY_Y_PIN\": "));  Serial.print(JOY_Y_PIN); Serial.print(", ");
  Serial.print(F("\"LEFT_MOTOR_CONTROLLER_PIN\": "));  Serial.print(LEFT_MOTOR_CONTROLLER_PIN); Serial.print(", ");
  Serial.print(F("\"RIGHT_MOTOR_CONTROLLER_PIN\": "));  Serial.print(RIGHT_MOTOR_CONTROLLER_PIN); //Serial.print(", ");
  Serial.println("}");

}
