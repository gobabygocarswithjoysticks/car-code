/*
  This file contains code for receiving settings values over Serial, and saving and recalling settings from EEPROM (lasts between power cycles) memory.
  To counter the possibility of values in EEPROM becoming corrupted, every value is stored three times and if there are differences, there is a vote bit by bit to restore the more likely correct value.
  A checksum value is also calculated so that if the data isn't restored correctly the car is likely to detect that.
  If the checksum reveals data corruption, the car enters a safe mode instead of driving with wrong values.
*/

const unsigned int repeat_space = 200; // space between each copy of a variable

void settingsMemory()
{
  byte settingsMemoryKeyRead; // the read value
  unsigned int settingsMemoryKeyReadAddress = 0;
  EEPROMread(settingsMemoryKeyReadAddress, settingsMemoryKeyRead);
  if (settingsMemoryKeyRead != settings_memory_key) { // eeprom doesn't have the key value, use default instead of not yet programmed EEPROM
    settingsMemoryKeyReadAddress = 0;
    EEPROMwrite(settingsMemoryKeyReadAddress, settings_memory_key);
    saveSettings();
    movementAllowed = false;
  }
  recallSettings();
}

byte bufP = 0;
char buf[60] = { 0 }; // buffer to fill with Serial input

char resultBuf[15] = { 0 }; // for replying with the received value

uint32_t eepromCRC = 0;

void settingsSerial() {
  char in = Serial.read();
  if (in != -1) {
    if (in == ',') {
      buf[bufP] = 0;  // null terminator
      //process new input
      char *k = strtok(buf, ":,");
      char *v = strtok(0, ":,");
      boolean changedSomething = true;  // used to see if a valid command was found
      memset(resultBuf, '\0', 15);
      if (strcmp(k, "CONTROL_RIGHT") == 0) {
        CONTROL_RIGHT = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_RIGHT);
      } else if (strcmp(k, "CONTROL_CENTER_X") == 0) {
        CONTROL_CENTER_X = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_CENTER_X);
      } else if (strcmp(k, "CONTROL_LEFT") == 0) {
        CONTROL_LEFT = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_LEFT);
      } else if (strcmp(k, "X_DEADZONE") == 0) {
        X_DEADZONE = abs(atoi(v));
        sprintf(resultBuf, "%d", X_DEADZONE);
      } else if (strcmp(k, "CONTROL_UP") == 0) {
        CONTROL_UP = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_UP);
      } else if (strcmp(k, "CONTROL_CENTER_Y") == 0) {
        CONTROL_CENTER_Y = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_CENTER_Y);
      } else if (strcmp(k, "CONTROL_DOWN") == 0) {
        CONTROL_DOWN = atoi(v);
        sprintf(resultBuf, "%d", CONTROL_DOWN);
      } else if (strcmp(k, "Y_DEADZONE") == 0) {
        Y_DEADZONE = abs(atoi(v));
        sprintf(resultBuf, "%d", Y_DEADZONE);
      } else if (strcmp(k, "ACCELERATION_FORWARD") == 0) {
        ACCELERATION_FORWARD = abs(atof(v));
        dtostrf(ACCELERATION_FORWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "DECELERATION_FORWARD") == 0) {
        DECELERATION_FORWARD = abs(atof(v));
        dtostrf(DECELERATION_FORWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "ACCELERATION_BACKWARD") == 0) {
        ACCELERATION_BACKWARD = abs(atof(v));
        dtostrf(ACCELERATION_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "DECELERATION_BACKWARD") == 0) {
        DECELERATION_BACKWARD = abs(atof(v));
        dtostrf(DECELERATION_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "ACCELERATION_TURNING") == 0) {
        ACCELERATION_TURNING = abs(atof(v));
        dtostrf(ACCELERATION_TURNING, 0, 4, resultBuf);
      } else if (strcmp(k, "DECELERATION_TURNING") == 0) {
        DECELERATION_TURNING = abs(atof(v));
        dtostrf(DECELERATION_TURNING, 0, 4, resultBuf);
      } else if (strcmp(k, "FASTEST_FORWARD") == 0) {
        FASTEST_FORWARD = constrain(abs(atof(v)), 0, 1);
        dtostrf(FASTEST_FORWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "FASTEST_BACKWARD") == 0) {
        FASTEST_BACKWARD = constrain(abs(atof(v)), 0, 1);
        dtostrf(FASTEST_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp(k, "TURN_SPEED") == 0) {
        TURN_SPEED = constrain(abs(atof(v)), 0, 1);
        dtostrf(TURN_SPEED, 0, 4, resultBuf);
      } else if (strcmp(k, "SCALE_TURNING_WHEN_MOVING") == 0) {
        SCALE_TURNING_WHEN_MOVING = atof(v);
        dtostrf(SCALE_TURNING_WHEN_MOVING, 0, 4, resultBuf);
      } else if (strcmp(k, "SCALE_ACCEL_WITH_SPEED") == 0) {
        SCALE_ACCEL_WITH_SPEED = atoi(v);
        if (SCALE_ACCEL_WITH_SPEED)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "REVERSE_TURN_IN_REVERSE") == 0) {
        REVERSE_TURN_IN_REVERSE = atoi(v);
        if (REVERSE_TURN_IN_REVERSE)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "LEFT_MOTOR_CENTER") == 0) {
        LEFT_MOTOR_CENTER = atoi(v);
        sprintf(resultBuf, "%d", LEFT_MOTOR_CENTER);
      } else if (strcmp(k, "LEFT_MOTOR_SLOW") == 0) {
        LEFT_MOTOR_SLOW = atoi(v);
        sprintf(resultBuf, "%d", LEFT_MOTOR_SLOW);
      } else if (strcmp(k, "LEFT_MOTOR_FAST") == 0) {
        LEFT_MOTOR_FAST = atoi(v);
        sprintf(resultBuf, "%d", LEFT_MOTOR_FAST);
      } else if (strcmp(k, "RIGHT_MOTOR_CENTER") == 0) {
        RIGHT_MOTOR_CENTER = atoi(v);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_CENTER);
      } else if (strcmp(k, "RIGHT_MOTOR_SLOW") == 0) {
        RIGHT_MOTOR_SLOW = atoi(v);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_SLOW);
      } else if (strcmp(k, "RIGHT_MOTOR_FAST") == 0) {
        RIGHT_MOTOR_FAST = atoi(v);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_FAST);
      } else if (strcmp(k, "USE_SPEED_KNOB") == 0) {
        USE_SPEED_KNOB = atoi(v);
        if (USE_SPEED_KNOB)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "SPEED_KNOB_SLOW_VAL") == 0) {
        SPEED_KNOB_SLOW_VAL = atoi(v);
        sprintf(resultBuf, "%d", SPEED_KNOB_SLOW_VAL);
      } else if (strcmp(k, "SPEED_KNOB_FAST_VAL") == 0) {
        SPEED_KNOB_FAST_VAL = atoi(v);
        sprintf(resultBuf, "%d", SPEED_KNOB_FAST_VAL);
      } else if (strcmp(k, "JOY_X_PIN") == 0) {
        JOY_X_PIN = atoi(v);
        pinMode(JOY_X_PIN, INPUT);
        sprintf(resultBuf, "%d", JOY_X_PIN);
      } else if (strcmp(k, "JOY_Y_PIN") == 0) {
        JOY_Y_PIN = atoi(v);
        pinMode(JOY_Y_PIN, INPUT);
        sprintf(resultBuf, "%d", JOY_Y_PIN);
      } else if (strcmp(k, "LEFT_MOTOR_CONTROLLER_PIN") == 0) {
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        leftMotorController.detach();
        LEFT_MOTOR_CONTROLLER_PIN = atoi(v);
        leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        sprintf(resultBuf, "%d", LEFT_MOTOR_CONTROLLER_PIN);
      } else if (strcmp(k, "RIGHT_MOTOR_CONTROLLER_PIN") == 0) {
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        rightMotorController.detach();
        RIGHT_MOTOR_CONTROLLER_PIN = atoi(v);
        rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_CONTROLLER_PIN);
      } else if (strcmp(k, "SPEED_KNOB_PIN") == 0) {
        SPEED_KNOB_PIN = atoi(v);
        pinMode(SPEED_KNOB_PIN, INPUT);
        sprintf(resultBuf, "%d", SPEED_KNOB_PIN);
      }

      else if (strcmp(k, "LEFT_MOTOR_PULSE") == 0) {
        LEFT_MOTOR_PULSE = atoi(v);
        sprintf(resultBuf, "%d", LEFT_MOTOR_PULSE);
      } else if (strcmp(k, "RIGHT_MOTOR_PULSE") == 0) {
        RIGHT_MOTOR_PULSE = atoi(v);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_PULSE);
      } else if (strcmp(k, "START_MOTOR_PULSE_TIME") == 0) {
        START_MOTOR_PULSE_TIME = max(atoi(v), 0);
        sprintf(resultBuf, "%d", START_MOTOR_PULSE_TIME);
      } else if (strcmp(k, "ENABLE_STARTUP_PULSE") == 0) {
        ENABLE_STARTUP_PULSE = atoi(v);
        if (ENABLE_STARTUP_PULSE)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "JOY_CALIB_COUNT") == 0) {
        JOY_CALIB_COUNT = atoi(v);
        sprintf(resultBuf, "%d", JOY_CALIB_COUNT);
      } else if (strcmp(k, "BUTTON_MODE_PIN") == 0) {
        BUTTON_MODE_PIN = atoi(v);
        pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
        sprintf(resultBuf, "%d", BUTTON_MODE_PIN);
      } else if (strcmp(k, "ENABLE_BUTTON_CTRL") == 0) {
        ENABLE_BUTTON_CTRL = atoi(v);
        if (ENABLE_BUTTON_CTRL) {
          if (USE_BUTTON_MODE_PIN) {
            pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
          }
          for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
            pinMode(driveButtons[i].pin, INPUT_PULLUP);
          }
        }
        if (ENABLE_BUTTON_CTRL)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "USE_BUTTON_MODE_PIN") == 0) {
        USE_BUTTON_MODE_PIN = atoi(v);
        if (ENABLE_BUTTON_CTRL) {
          if (USE_BUTTON_MODE_PIN) {
            pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
          }
          for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
            pinMode(driveButtons[i].pin, INPUT_PULLUP);
          }
        }
        if (USE_BUTTON_MODE_PIN)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      } else if (strcmp(k, "NUM_DRIVE_BUTTONS") == 0) {
        NUM_DRIVE_BUTTONS = constrain(atoi(v), 0, 6);
        sprintf(resultBuf, "%d", NUM_DRIVE_BUTTONS);
      }

      else if (strcmp(k, "DRIVE_BUTTONS") == 0) {

        int db = atoi(v); // which row of the driveButtons array?
        if (db >= 1 && db <= maxNumDriveButtons) { // a valid driveButtons index
          db--; // convert from array indices starting at 1 to starting at 0
          strtok(v, "_"); // the value after db
          driveButtons[db].pin   = atoi(strtok(0, "_"));
          driveButtons[db].speed = atof(strtok(0, "_"));
          driveButtons[db].turn  = atof(strtok(0, "_"));

          pinMode(driveButtons[db].pin, INPUT_PULLUP);

          Serial.print(F("{\"result\": \"change\""));
          Serial.print(", ");
          Serial.print(F("\"setting\": \""));
          Serial.print("DRIVE_BUTTON_");
          Serial.print(db + 1);
          Serial.print("\", ");
          Serial.print(F("\"value\": ["));

          Serial.print(driveButtons[db].pin);
          Serial.print(",");
          dtostrf(driveButtons[db].speed, 0, 4, resultBuf);
          Serial.print(resultBuf);
          Serial.print(",");
          dtostrf(driveButtons[db].turn, 0, 4, resultBuf);
          Serial.print(resultBuf);

          Serial.println("]}");

        }
        changedSomething = false;
      }
      else if (strcmp(k, "STEERING_OFF_SWITCH") == 0) {
        STEERING_OFF_SWITCH = atoi(v);
        if (STEERING_OFF_SWITCH) {
          pinMode(STEERING_OFF_SWITCH_PIN, INPUT_PULLUP);
          sprintf(resultBuf, "true");
        }
        else
          sprintf(resultBuf, "false");
      }
      else if (strcmp(k, "STEERING_OFF_SWITCH_PIN") == 0) {
        STEERING_OFF_SWITCH_PIN = atoi(v);
        pinMode(STEERING_OFF_SWITCH_PIN, INPUT_PULLUP);
        sprintf(resultBuf, "%d", STEERING_OFF_SWITCH_PIN);
      }
      else if (strcmp(k, "SAVE") == 0) {
        saveSettings();
        changedSomething = false;
        Serial.println(F("{\"result\": \"saved\"}"));
      } else if (strcmp(k, "SETTINGS") == 0) {
        printSettings();
        changedSomething = false;
      } else if (strcmp(k, "REVERT") == 0) {
        unsigned int settingsMemoryKeyAddr = 0;
        EEPROMwrite(settingsMemoryKeyAddr, settings_memory_key + 1);  // so that on reset the arduino discards EEPROM
        delay(5000); // trigger wdt
      } else if (strcmp(k, "REBOOT") == 0) {
        delay(5000); // trigger wdt
      } else if (strcmp(k, "G") == 0) {
        changedSomething = false;
        movementAllowed = true;
        Serial.println(F("{\"result\": \"movement allowed\"}"));
      } else if (strcmp(k, "S") == 0) {
        changedSomething = false;
        movementAllowed = false;
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        Serial.println(F("{\"result\": \"stopped\"}"));
      } else {
        Serial.println(F("{\"result\": \"no change\"}"));
        changedSomething = false;
      }

      if (changedSomething) {
        Serial.print(F("{\"result\": \"change\""));
        Serial.print(", ");
        Serial.print(F("\"setting\": \""));
        Serial.print(k);
        Serial.print("\", ");
        Serial.print(F("\"value\": \""));
        Serial.print(resultBuf);
        Serial.println("\"}");
      }

      bufP = 0;
    } else if (isAlphaNumeric(in) || in == '-' || in == '.' || in == ':' || in == '_') { // removes things like spaces and new line characters
      buf[bufP] = toupper(in); // settings names are all caps, but this way lower case versions will also be accepted
      if ((uint8_t)(bufP + 2) < sizeof buf) {
        bufP++; // don't go outside the buffer, leaving the last position always unused
      }
    }
  }
}

void saveSettings()
{
  unsigned int addressW = 1;
  eepromCRC = 0;

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

  EEPROMwrite(addressW, JOY_X_PIN);
  EEPROMwrite(addressW, JOY_Y_PIN);
  EEPROMwrite(addressW, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, SPEED_KNOB_PIN);

  EEPROMwrite(addressW, LEFT_MOTOR_PULSE);
  EEPROMwrite(addressW, RIGHT_MOTOR_PULSE);
  EEPROMwrite(addressW, START_MOTOR_PULSE_TIME);
  EEPROMwrite(addressW, ENABLE_STARTUP_PULSE);
  EEPROMwrite(addressW, JOY_CALIB_COUNT);
  EEPROMwrite(addressW, BUTTON_MODE_PIN);
  EEPROMwrite(addressW, ENABLE_BUTTON_CTRL);
  EEPROMwrite(addressW, USE_BUTTON_MODE_PIN);
  EEPROMwrite(addressW, NUM_DRIVE_BUTTONS);
  EEPROMwrite(addressW, driveButtons);
  EEPROMwrite(addressW, STEERING_OFF_SWITCH);
  EEPROMwrite(addressW, STEERING_OFF_SWITCH_PIN);

  EEPROMwrite(addressW, eepromCRC);
}
void recallSettings()
{
  unsigned int addressR = 1;
  eepromCRC = 0;

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

  EEPROMread(addressR, JOY_X_PIN);
  EEPROMread(addressR, JOY_Y_PIN);
  EEPROMread(addressR, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, SPEED_KNOB_PIN);

  EEPROMread(addressR, LEFT_MOTOR_PULSE);
  EEPROMread(addressR, RIGHT_MOTOR_PULSE);
  EEPROMread(addressR, START_MOTOR_PULSE_TIME);
  EEPROMread(addressR, ENABLE_STARTUP_PULSE);
  EEPROMread(addressR, JOY_CALIB_COUNT);
  EEPROMread(addressR, BUTTON_MODE_PIN);
  EEPROMread(addressR, ENABLE_BUTTON_CTRL);
  EEPROMread(addressR, USE_BUTTON_MODE_PIN);
  EEPROMread(addressR, NUM_DRIVE_BUTTONS);
  EEPROMread(addressR, driveButtons);
  EEPROMread(addressR, STEERING_OFF_SWITCH);
  EEPROMread(addressR, STEERING_OFF_SWITCH_PIN);

  uint32_t tempEepromCRC = eepromCRC;
  uint32_t readCRC = 0;
  EEPROMread(addressR, readCRC);
  // a checksum isn't a perfect guarantee that the stored data is valid, but likely better than 1 in 1000
  if (tempEepromCRC != readCRC) { // calculated checksum of data doesn't match stored checksum of data
    delay(50);
    Serial.println(F("{\"error\": \"eeprom failure\"}"));
    delay(50); // eeprom bad, shut down everything
    if (leftMotorController.attached())
      leftMotorController.detach();
    if (rightMotorController.attached())
      rightMotorController.detach();
    delay(100);
    // stop sending any signals out of the pins (set all to inputs, even if it's on a Mega)
    for (byte pin = 2; pin <= 100; pin++) {
      pinMode(pin, INPUT);
      digitalWrite(pin, LOW);
    }
    pinMode(LED_BUILTIN, OUTPUT);
    delay(50);
#ifdef AVR
    wdt_disable();
#endif
    while (true) { // flash SOS forever
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
      rp2040.wdt_reset();
#endif
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      delay(400);
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
      rp2040.wdt_reset();
#endif
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
      rp2040.wdt_reset();
#endif
      digitalWrite(LED_BUILTIN, LOW);
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(400);
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
      rp2040.wdt_reset();
#endif
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
      rp2040.wdt_reset();
#endif
      delay(1000);
    }
  }
}

template <typename T>
void EEPROMwrite(unsigned int& address, const T & value)
{
  uint32_t tempEepromCRC = eepromCRC;
  // modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  const byte* p = (const byte*)&value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    EEPROM.update(address, *p); // Three copies of the data are stored in EEPROM to detect and correct single bit errors
    EEPROM.update(address + repeat_space, *p);
    EEPROM.update(address + repeat_space * 2, *p);
    tempEepromCRC = crc_update(tempEepromCRC, *p);
    // writes 3 copies
    address++;
    p++;
  }
  eepromCRC = tempEepromCRC;
}

template <typename T>
void EEPROMread(unsigned int& address, T & value)
{
  // modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  byte* p = (byte*)&value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    byte a = EEPROM.read(address);
    byte b = EEPROM.read(address + repeat_space);
    byte c = EEPROM.read(address + repeat_space * 2);
    // majority voting
    if (a == b && b == c) { // and a==c, so all agree
      *p = a; // a normal read
    } else { // disagreement, correct the corrupted bit
      //(if two bits flip, both in the same place, then instead of correcting the error, the error is kept, but this is hopefully unlikely and likely to be caught by the checksum)
      *p = (a & b) | (b & c) | (c & a); // bitwise majority https://stackoverflow.com/a/29892322
      EEPROM.update(address, *p); // replace all three copies with the majority value
      EEPROM.update(address + repeat_space, *p);
      EEPROM.update(address + repeat_space * 2, *p);
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
    eepromCRC = crc_update(eepromCRC, *p);
    address++;
    p++;
  }
}

// for calculating checksum of EEPROM data
// https://forum.arduino.cc/t/fixed-working-arduino-crc-32-code/89249/5
const uint32_t PROGMEM crc_table[16] = {
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};
uint32_t crc_update(uint32_t crc, byte data)
{
  byte tbl_idx;
  tbl_idx = crc ^ (data >> (0 * 4));
  crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
  tbl_idx = crc ^ (data >> (1 * 4));
  crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
  return crc;
}
