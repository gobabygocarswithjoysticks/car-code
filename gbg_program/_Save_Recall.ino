const unsigned int address_start = 1; // can be used to reserve some space at the start for values other than settings like the settings_memory_key.

void settingsMemory() {
  if (EEPROM.read(0) != settings_memory_key) {
    EEPROM.write(0, settings_memory_key);
    saveSettings();
  }
  recallSettings();
}

char buf[50] = {0};
byte bufP = 0;

void settingsSerial() {
  char in = Serial.read();
  if (in != -1) {
    if (in == ',') {
      buf[bufP] = 0; // null terminator
      //process new input
      char *k = strtok(buf, ":,");
      char *v = strtok(0, ":,");
      boolean changedSomething = true; // used to see if a valid command was found
      if (strcmp(k, "CONTROL_RIGHT") == 0) CONTROL_RIGHT = atoi(v);
      else if (strcmp(k, "CONTROL_CENTER_X") == 0) CONTROL_CENTER_X = atoi(v);
      else if (strcmp(k, "CONTROL_LEFT") == 0) CONTROL_LEFT = atoi(v);
      else if (strcmp(k, "X_DEADZONE") == 0) X_DEADZONE = atoi(v);
      else if (strcmp(k, "CONTROL_UP") == 0) CONTROL_UP = atoi(v);
      else if (strcmp(k, "CONTROL_CENTER_Y") == 0) CONTROL_CENTER_Y = atoi(v);
      else if (strcmp(k, "CONTROL_DOWN") == 0) CONTROL_DOWN = atoi(v);
      else if (strcmp(k, "Y_DEADZONE") == 0) Y_DEADZONE = atoi(v);
      else if (strcmp(k, "ACCELERATION_FORWARD") == 0) ACCELERATION_FORWARD = atof(v);
      else if (strcmp(k, "DECELERATION_FORWARD") == 0) DECELERATION_FORWARD = atof(v);
      else if (strcmp(k, "ACCELERATION_BACKWARD") == 0) ACCELERATION_BACKWARD = atof(v);
      else if (strcmp(k, "DECELERATION_BACKWARD") == 0) DECELERATION_BACKWARD = atof(v);
      else if (strcmp(k, "ACCELERATION_TURNING") == 0) ACCELERATION_TURNING = atof(v);
      else if (strcmp(k, "DECELERATION_TURNING") == 0) DECELERATION_TURNING = atof(v);
      else if (strcmp(k, "FASTEST_FORWARD") == 0) FASTEST_FORWARD = atof(v);
      else if (strcmp(k, "FASTEST_BACKWARD") == 0) FASTEST_BACKWARD = atof(v);
      else if (strcmp(k, "TURN_SPEED") == 0) TURN_SPEED = atof(v);
      else if (strcmp(k, "SCALE_TURNING_WHEN_MOVING") == 0) SCALE_TURNING_WHEN_MOVING = atof(v);
      else if (strcmp(k, "SCALE_ACCEL_WITH_SPEED") == 0) SCALE_ACCEL_WITH_SPEED = atoi(v);
      else if (strcmp(k, "REVERSE_TURN_IN_REVERSE") == 0) REVERSE_TURN_IN_REVERSE = atoi(v);
      else if (strcmp(k, "LEFT_MOTOR_CENTER") == 0) LEFT_MOTOR_CENTER = atoi(v);
      else if (strcmp(k, "LEFT_MOTOR_SLOW") == 0) LEFT_MOTOR_SLOW = atoi(v);
      else if (strcmp(k, "LEFT_MOTOR_FAST") == 0) LEFT_MOTOR_FAST = atoi(v);
      else if (strcmp(k, "RIGHT_MOTOR_CENTER") == 0) RIGHT_MOTOR_CENTER = atoi(v);
      else if (strcmp(k, "RIGHT_MOTOR_SLOW") == 0) RIGHT_MOTOR_SLOW = atoi(v);
      else if (strcmp(k, "RIGHT_MOTOR_FAST") == 0) RIGHT_MOTOR_FAST = atoi(v);
      else if (strcmp(k, "USE_SPEED_KNOB") == 0) USE_SPEED_KNOB = atoi(v);
      else if (strcmp(k, "SPEED_KNOB_SLOW_VAL") == 0) SPEED_KNOB_SLOW_VAL = atoi(v);
      else if (strcmp(k, "SPEED_KNOB_FAST_VAL") == 0) SPEED_KNOB_FAST_VAL = atoi(v);
      else if (strcmp(k, "PRINT_VARIABLES_INTERVAL_MILLIS") == 0) PRINT_VARIABLES_INTERVAL_MILLIS = atoi(v);

      else if (strcmp(k, "JOY_X_PIN") == 0) {
        JOY_X_PIN = atoi(v);
        pinMode(JOY_X_PIN, INPUT);
      } else if (strcmp(k, "JOY_Y_PIN") == 0) {
        JOY_Y_PIN = atoi(v);
        pinMode(JOY_Y_PIN, INPUT);
      } else if (strcmp(k, "LEFT_MOTOR_CONTROLLER_PIN") == 0) {
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        leftMotorController.detach();
        LEFT_MOTOR_CONTROLLER_PIN = atoi(v);
        leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
      } else if (strcmp(k, "RIGHT_MOTOR_CONTROLLER_PIN") == 0) {
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        rightMotorController.detach();
        RIGHT_MOTOR_CONTROLLER_PIN = atoi(v);
        rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
      } else if (strcmp(k, "SPEED_KNOB_PIN") == 0) {
        SPEED_KNOB_PIN = atoi(v);
        pinMode(SPEED_KNOB_PIN, INPUT);
      }
      else if (strcmp(k, "SAVE") == 0) {
        saveSettings();
      } else if (strcmp(k, "SETTINGS") == 0) {
        printSettings();
        changedSomething = false;
      } else {
        Serial.println(F("{\"result\": \"no change\"}"));
        changedSomething = false;
      }

      if (changedSomething) Serial.println(F("{\"result\": \"success\"}"));

      bufP = 0;
    } else if (isAlphaNumeric(in) || in == '-' || in == '.' || in == ':' || in == '_') { //removes things like spaces and new line characters
      buf[bufP] = toupper(in); // settings names are all caps, but this way lower case versions will also be accepted
      if (bufP + 2 < sizeof buf) {
        bufP++; // don't go outside the buffer, leaving the last position always unused
      }
    }
  }
}

void saveSettings() {
  unsigned int addressW = address_start;

  EEPROMwrite(addressW, CONTROL_RIGHT);
  EEPROMwrite(addressW, CONTROL_CENTER_X);
  EEPROMwrite(addressW, CONTROL_LEFT);
  EEPROMwrite(addressW, X_DEADZONE);
  EEPROMwrite(addressW, CONTROL_UP);
  EEPROMwrite(addressW, CONTROL_CENTER_Y);
  EEPROMwrite(addressW, CONTROL_DOWN);
  EEPROMwrite(addressW, Y_DEADZONE);
  EEPROMwrite(addressW, ACCELERATION_FORWARD);
  EEPROMwrite(addressW, DECELERATION_FORWARD);
  EEPROMwrite(addressW, ACCELERATION_BACKWARD);
  EEPROMwrite(addressW, DECELERATION_BACKWARD);
  EEPROMwrite(addressW, ACCELERATION_TURNING);
  EEPROMwrite(addressW, DECELERATION_TURNING);
  EEPROMwrite(addressW, FASTEST_FORWARD);
  EEPROMwrite(addressW, FASTEST_BACKWARD);
  EEPROMwrite(addressW, TURN_SPEED);
  EEPROMwrite(addressW, SCALE_TURNING_WHEN_MOVING);
  EEPROMwrite(addressW, SCALE_ACCEL_WITH_SPEED);
  EEPROMwrite(addressW, REVERSE_TURN_IN_REVERSE);
  EEPROMwrite(addressW, LEFT_MOTOR_CENTER);
  EEPROMwrite(addressW, LEFT_MOTOR_SLOW);
  EEPROMwrite(addressW, LEFT_MOTOR_FAST);
  EEPROMwrite(addressW, RIGHT_MOTOR_CENTER);
  EEPROMwrite(addressW, RIGHT_MOTOR_SLOW);
  EEPROMwrite(addressW, RIGHT_MOTOR_FAST);
  EEPROMwrite(addressW, USE_SPEED_KNOB);
  EEPROMwrite(addressW, SPEED_KNOB_SLOW_VAL);
  EEPROMwrite(addressW, SPEED_KNOB_FAST_VAL);
  EEPROMwrite(addressW, PRINT_VARIABLES_INTERVAL_MILLIS);

  EEPROMwrite(addressW, JOY_X_PIN);
  EEPROMwrite(addressW, JOY_Y_PIN);
  EEPROMwrite(addressW, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, SPEED_KNOB_PIN);

}
void recallSettings() {
  unsigned int addressR = address_start;

  EEPROMread(addressR, CONTROL_RIGHT);
  EEPROMread(addressR, CONTROL_CENTER_X);
  EEPROMread(addressR, CONTROL_LEFT);
  EEPROMread(addressR, X_DEADZONE);
  EEPROMread(addressR, CONTROL_UP);
  EEPROMread(addressR, CONTROL_CENTER_Y);
  EEPROMread(addressR, CONTROL_DOWN);
  EEPROMread(addressR, Y_DEADZONE);
  EEPROMread(addressR, ACCELERATION_FORWARD);
  EEPROMread(addressR, DECELERATION_FORWARD);
  EEPROMread(addressR, ACCELERATION_BACKWARD);
  EEPROMread(addressR, DECELERATION_BACKWARD);
  EEPROMread(addressR, ACCELERATION_TURNING);
  EEPROMread(addressR, DECELERATION_TURNING);
  EEPROMread(addressR, FASTEST_FORWARD);
  EEPROMread(addressR, FASTEST_BACKWARD);
  EEPROMread(addressR, TURN_SPEED);
  EEPROMread(addressR, SCALE_TURNING_WHEN_MOVING);
  EEPROMread(addressR, SCALE_ACCEL_WITH_SPEED);
  EEPROMread(addressR, REVERSE_TURN_IN_REVERSE);
  EEPROMread(addressR, LEFT_MOTOR_CENTER);
  EEPROMread(addressR, LEFT_MOTOR_SLOW);
  EEPROMread(addressR, LEFT_MOTOR_FAST);
  EEPROMread(addressR, RIGHT_MOTOR_CENTER);
  EEPROMread(addressR, RIGHT_MOTOR_SLOW);
  EEPROMread(addressR, RIGHT_MOTOR_FAST);
  EEPROMread(addressR, USE_SPEED_KNOB);
  EEPROMread(addressR, SPEED_KNOB_SLOW_VAL);
  EEPROMread(addressR, SPEED_KNOB_FAST_VAL);
  EEPROMread(addressR, PRINT_VARIABLES_INTERVAL_MILLIS);

  EEPROMread(addressR, JOY_X_PIN);
  EEPROMread(addressR, JOY_Y_PIN);
  EEPROMread(addressR, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, SPEED_KNOB_PIN);

}

template <typename T>
void EEPROMwrite (unsigned int & address, const T& value)
{
  //modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  const byte * p = (const byte*) &value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    EEPROM.update(address++, *p++);
  }
}

template <typename T>
void EEPROMread (unsigned int & address, T& value)
{
  //modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  byte * p = ( byte*) &value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    *p++ = EEPROM.read(address++);
  }
}
