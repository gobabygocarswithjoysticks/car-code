boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    Serial.print("joyXVal: "); Serial.print(joyXVal); Serial.print(", ");
    Serial.print("joyYVal: "); Serial.print(joyYVal); Serial.print(", ");
    Serial.print("turnInput: "); Serial.print(turnInput, 3); Serial.print(", ");
    Serial.print("speedInput: "); Serial.print(speedInput, 3); Serial.print(", ");
    Serial.print("turnProcessed: "); Serial.print(turnToDrive, 3); Serial.print(", ");
    Serial.print("speedProcessed: "); Serial.print(speedToDrive, 3); Serial.print(", ");
    Serial.print("turnToDrive: "); Serial.print(turnToDrive, 3); Serial.print(", ");
    Serial.print("speedToDrive: "); Serial.print(speedToDrive, 3); Serial.print(", ");
    Serial.print("leftMotorWriteVal: "); Serial.print(leftMotorWriteVal); Serial.print(", ");
    Serial.print("rightMotorWriteVal: "); Serial.print(rightMotorWriteVal); Serial.print(", ");
    Serial.print("speedKnobScaler: "); Serial.print(speedKnobScaler); Serial.print(", ");
    Serial.println();
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  Serial.println();
  Serial.println("-----------------------------------");
  Serial.print("Last programmed on "); Serial.println(__DATE__);
  Serial.println("current settings:");
  Serial.print("CONTROL_RIGHT: ");  Serial.println(CONTROL_RIGHT);
  Serial.print("CONTROL_CENTER_X: ");  Serial.println(CONTROL_CENTER_X);
  Serial.print("CONTROL_LEFT: ");  Serial.println(CONTROL_LEFT);
  Serial.print("CONTROL_UP: ");  Serial.println(CONTROL_UP);
  Serial.print("CONTROL_CENTER_Y: ");  Serial.println(CONTROL_CENTER_Y);
  Serial.print("CONTROL_DOWN: ");  Serial.println(CONTROL_DOWN);
  Serial.print("ACCELERATION_FORWARD: ");  Serial.println(ACCELERATION_FORWARD);
  Serial.print("DECELERATION_FORWARD: ");  Serial.println(DECELERATION_FORWARD);
  Serial.print("ACCELERATION_BACKWARD: ");  Serial.println(ACCELERATION_BACKWARD);
  Serial.print("DECELERATION_BACKWARD: ");  Serial.println(DECELERATION_BACKWARD);
  Serial.print("ACCELERATION_TURNING: ");  Serial.println(ACCELERATION_TURNING);
  Serial.print("DECELERATION_TURNING: ");  Serial.println(DECELERATION_TURNING);
  Serial.print("FASTEST_FORWARD: ");  Serial.println(FASTEST_FORWARD);
  Serial.print("FASTEST_BACKWARD: ");  Serial.println(FASTEST_BACKWARD);
  Serial.print("TURN_SPEED: ");  Serial.println(TURN_SPEED);
  Serial.print("SCALE_TURNING_WHEN_MOVING: ");  Serial.println(SCALE_TURNING_WHEN_MOVING);
  Serial.print("joyXPin: ");  Serial.println(joyXPin);
  Serial.print("joyYPin: ");  Serial.println(joyYPin);
  Serial.print("leftMotorControllerPin: ");  Serial.println(leftMotorControllerPin);
  Serial.print("rightMotorControllerPin: ");  Serial.println(rightMotorControllerPin);
  Serial.print("speedKnobPin: ");  Serial.println(speedKnobPin);
  Serial.print("buttonControlPin: ");  Serial.println(buttonControlPin);
  Serial.print("numDriveButtons: ");  Serial.println(numDriveButtons);
  Serial.print("reverseTurnInReverse: ");  Serial.println(reverseTurnInReverse);
  Serial.print("LEFT_MOTOR_CENTER: ");  Serial.println(LEFT_MOTOR_CENTER);
  Serial.print("LEFT_MOTOR_SLOW: ");  Serial.println(LEFT_MOTOR_SLOW);
  Serial.print("LEFT_MOTOR_FAST: ");  Serial.println(LEFT_MOTOR_FAST);
  Serial.print("RIGHT_MOTOR_CENTER: ");  Serial.println(RIGHT_MOTOR_CENTER);
  Serial.print("RIGHT_MOTOR_SLOW: ");  Serial.println(RIGHT_MOTOR_SLOW);
  Serial.print("RIGHT_MOTOR_FAST: ");  Serial.println(RIGHT_MOTOR_FAST);
  Serial.print("speedKnobSlowVal: ");  Serial.println(speedKnobSlowVal);
  Serial.print("speedKnobFastVal: ");  Serial.println(speedKnobFastVal);

  Serial.println("-----------------------------------");

}
