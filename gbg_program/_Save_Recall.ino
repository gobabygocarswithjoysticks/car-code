const unsigned int repeat_space = 150; // space between each copy of a variable

void settingsMemory() {
  byte settingsMemoryKeyRead; // the read value
  unsigned int settingsMemoryKeyReadAddress = 0;
  EEPROMread(settingsMemoryKeyReadAddress, settingsMemoryKeyRead);
  if (settingsMemoryKeyRead != settings_memory_key) { // eeprom doesn't have the key value, use default instead of not yet programmed EEPROM
    settingsMemoryKeyReadAddress = 0;
    EEPROMwrite(settingsMemoryKeyReadAddress, settings_memory_key);
    saveSettings();
  }
  recallSettings();
}

char buf[50] = {0}; // buffer to fill with Serial input
byte bufP = 0;

char resultBuf[15] = {0};  // for replying with the received value

void(* resetFunc) (void) = 0; // reboots the Arduino https://www.theengineeringprojects.com/2015/11/reset-arduino-programmatically.html

uint32_t eepromCRC = 0;

void settingsSerial() {
  char in = Serial.read();
  if (in != -1) {
    if (in == ',') {
      buf[bufP] = 0; // null terminator
      //process new input
      char *k = strtok(buf, ":,");
      char *v = strtok(0, ":,");
      boolean changedSomething = true; // used to see if a valid command was found
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
      } else if (strcmp(k, "PRINT_VARIABLES_INTERVAL_MILLIS") == 0) {
        PRINT_VARIABLES_INTERVAL_MILLIS = atoi(v);
        sprintf(resultBuf, "%d", PRINT_VARIABLES_INTERVAL_MILLIS);
      }
      else if (strcmp(k, "JOY_X_PIN") == 0) {
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
      else if (strcmp(k, "SAVE") == 0) {
        saveSettings();
        changedSomething = false;
        Serial.println(F("{\"result\": \"saved\"}"));
      } else if (strcmp(k, "SETTINGS") == 0) {
        printSettings();
        changedSomething = false;
      }  else if (strcmp(k, "REVERT") == 0) {
        unsigned int settingsMemoryKeyAddr = 0;
        EEPROMwrite(settingsMemoryKeyAddr, settings_memory_key + 1); // so that on reset the arduino discards EEPROM
        resetFunc();
      }  else if (strcmp(k, "REBOOT") == 0) {
        resetFunc();
      }  else if (strcmp(k, "G") == 0) {
        changedSomething = false;
        movementAllowed = true;
        Serial.println(F("{\"result\": \"movement allowed\"}"));
      }  else if (strcmp(k, "S") == 0) {
        changedSomething = false;
        movementAllowed = false;
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        Serial.println(F("{\"result\": \"stopped\"}"));
      }  else if (strcmp(k, "C1") == 0) { // todo: remove
        changedSomething = false;


        EEPROM.write(random(0, 460), random(0, 256));



      }  else if (strcmp(k, "C2") == 0) { // todo: remove
        changedSomething = false;

        byte r = random(0, 150);
        EEPROM.write(r, 56);
        EEPROM.write(r + 150, 56);


      } else {
        Serial.println(F("{\"result\": \"no change\"}"));
        changedSomething = false;
      }

      if (changedSomething) {
        Serial.print(F("{\"result\": \"change\"")); Serial.print(", ");
        Serial.print(F("\"setting\": \"")); Serial.print(k); Serial.print("\", ");
        Serial.print(F("\"value\": \"")); Serial.print(resultBuf); Serial.println("\"}");
      }

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
  EEPROMwrite(addressW, PRINT_VARIABLES_INTERVAL_MILLIS);

  EEPROMwrite(addressW, JOY_X_PIN);
  EEPROMwrite(addressW, JOY_Y_PIN);
  EEPROMwrite(addressW, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, SPEED_KNOB_PIN);

  EEPROMwrite(addressW, eepromCRC);
}
void recallSettings() {
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
  EEPROMread(addressR, PRINT_VARIABLES_INTERVAL_MILLIS);

  EEPROMread(addressR, JOY_X_PIN);
  EEPROMread(addressR, JOY_Y_PIN);
  EEPROMread(addressR, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, RIGHT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, SPEED_KNOB_PIN);

  uint32_t tempEepromCRC = eepromCRC;
  uint32_t readCRC = 0;
  EEPROMread(addressR, readCRC);

  if (tempEepromCRC != readCRC) {
    Serial.println(F("{\"error\": \"eeprom failure\"}"));
    leftMotorController.detach();
    rightMotorController.detach();
    // stop sending any signals out of the pins (set all to inputs, even if it's on a Mega)
    for (byte pin = 2; pin <= 100; pin++) {
      pinMode(pin, INPUT);
      digitalWrite(pin, LOW);
    }
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) { // flash SOS forever
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH); delay(500); digitalWrite(LED_BUILTIN, LOW); delay(400);
      digitalWrite(LED_BUILTIN, HIGH); delay(500); digitalWrite(LED_BUILTIN, LOW); delay(400);
      digitalWrite(LED_BUILTIN, HIGH); delay(500); digitalWrite(LED_BUILTIN, LOW); delay(400);
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
      delay(1000);
    }
  }
}

template <typename T>
void EEPROMwrite (unsigned int & address, const T & value)
{
  uint32_t tempEepromCRC = eepromCRC;
  //modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  const byte * p = (const byte*) &value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    EEPROM.update(address, *p);
    EEPROM.update(address + repeat_space, *p);
    EEPROM.update(address + repeat_space * 2, *p);
    tempEepromCRC = crc_update(tempEepromCRC, *p);
    // writes 3 copies
    address++;
    *p++;
  }
  eepromCRC = tempEepromCRC;
}

template <typename T>
void EEPROMread (unsigned int & address, T & value)
{
  //modified from code by Nick Gammon https://forum.arduino.cc/t/how-do-i-convert-a-struct-to-a-byte-array-and-back-to-a-struct-again/261791/8
  byte * p = ( byte*) &value;
  for (unsigned int i = 0; i < sizeof value; i++) {
    byte a = EEPROM.read(address);
    byte b = EEPROM.read(address + repeat_space);
    byte c = EEPROM.read(address + repeat_space * 2);
    // majority voting
    if (a == b && b == c) { //and a==c, so all agree
      *p = a; // a normal read
    } else { // disagreement, correct the corrupted bit (if two bits flip, both in the same place, then instead of correcting the error, the error is kept, but this is hopefully unlikely)
      *p = (a & b) | (b & c) | (c & a); //bitwise majority https://stackoverflow.com/a/29892322
      EEPROM.update(address, *p); // replace all three copies with the majority value
      EEPROM.update(address + repeat_space, *p);
      EEPROM.update(address + repeat_space * 2, *p);
      digitalWrite(LED_BUILTIN, LOW); delay(100); digitalWrite(LED_BUILTIN, HIGH); delay(100);
    }
    eepromCRC = crc_update(eepromCRC, *p);
    address++;
    *p++;
  }
}

// for calculating checksum of EEPROM data
//https://forum.arduino.cc/t/fixed-working-arduino-crc-32-code/89249/5
const uint32_t PROGMEM  crc_table[16] = {
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
