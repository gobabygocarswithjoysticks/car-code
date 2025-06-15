/*
  This file contains code for receiving settings values over Serial, and saving and recalling settings from EEPROM (lasts between power cycles) memory.
  To counter the possibility of values in EEPROM becoming corrupted, every value is stored three times and if there are differences, there is a vote bit by bit to restore the more likely correct value.
  A checksum value is also calculated so that if the data isn't restored correctly the car is likely to detect that.
  If the checksum reveals data corruption, the car enters a safe mode instead of driving with wrong values.
*/

const unsigned int repeat_space = 200; // space between each copy of a variable (larger than number of bytes needed for settings)

byte bufP = 0;
char buf[60] = { 0 }; // buffer to fill with Serial input

char resultBuf[15] = { 0 }; // for replying with the received value

uint32_t eepromCRC = 0;

boolean errorCorrectionPerformed = false;

void settingsMemory()
{
  byte settingsMemoryKeyRead; // the read value
  unsigned int settingsMemoryKeyReadAddress = 0;

  errorCorrectionPerformed = false;
  EEPROMread(settingsMemoryKeyReadAddress, settingsMemoryKeyRead);
#if defined(FAKE_EEPROM)
  if (errorCorrectionPerformed) {
    EEPROM.commit(); // rp2040 EEPROM library requires this to be used to write the updated data to the flash that is simulating EEPROM (flash has more limited cycles)
  }
#endif

  if (settingsMemoryKeyRead != settings_memory_key) { // eeprom doesn't have the key value, use default instead of not yet programmed EEPROM
    settingsMemoryKeyReadAddress = 0;
    EEPROMwrite(settingsMemoryKeyReadAddress, settings_memory_key);
#if defined(FAKE_EEPROM)
    EEPROM.commit(); // rp2040 EEPROM library requires this to be used to write the updated data to the flash that is simulating EEPROM (flash has more limited cycles)
#endif
    saveSettings();
    movementAllowed = false;
  } // end of special case of overwriting using default values
  recallSettings();
}

#include "settings_names.h"

#define NUM_SETTINGS_ID_INT 19
const SettingID settingsID_int[NUM_SETTINGS_ID_INT] = {
  S_CONTROL_RIGHT,
  S_CONTROL_CENTER_X,
  S_CONTROL_LEFT,
  S_X_DEADZONE,
  S_CONTROL_UP,
  S_CONTROL_CENTER_Y,
  S_CONTROL_DOWN,
  S_Y_DEADZONE,
  S_LEFT_MOTOR_CENTER,
  S_LEFT_MOTOR_SLOW,
  S_LEFT_MOTOR_FAST,
  S_RIGHT_MOTOR_CENTER,
  S_RIGHT_MOTOR_SLOW,
  S_RIGHT_MOTOR_FAST,
  S_LEFT_MOTOR_PULSE,
  S_RIGHT_MOTOR_PULSE,
  S_JOY_CALIB_COUNT,
  S_SPEED_KNOB_SLOW_VAL,
  S_SPEED_KNOB_FAST_VAL,
};

int16_t* settingsPtr_int[NUM_SETTINGS_ID_INT] = {
  &CONTROL_RIGHT,
  &CONTROL_CENTER_X,
  &CONTROL_LEFT,
  &X_DEADZONE,
  &CONTROL_UP,
  &CONTROL_CENTER_Y,
  &CONTROL_DOWN,
  &Y_DEADZONE,
  &LEFT_MOTOR_CENTER,
  &LEFT_MOTOR_SLOW,
  &LEFT_MOTOR_FAST,
  &RIGHT_MOTOR_CENTER,
  &RIGHT_MOTOR_SLOW,
  &RIGHT_MOTOR_FAST,
  &LEFT_MOTOR_PULSE,
  &RIGHT_MOTOR_PULSE,
  &JOY_CALIB_COUNT,
  &SPEED_KNOB_SLOW_VAL,
  &SPEED_KNOB_FAST_VAL,
};

void settingsSerial() {
  int8_t in = Serial.read();
  if (in != -1) {
    if (in == ',') {
      buf[bufP] = 0;  // null terminator
      //process new input
      char *k = strtok(buf, ":,");
      char *v = strtok(0, ":,");
      boolean changedSomething = true;  // used to see if a valid command was found
      memset(resultBuf, '\0', 15);

      boolean found = false;
      for (byte i = 0; i < NUM_SETTINGS_ID_INT; i++) { // check if the key matches any of the simple integer settings
        if (strcmp_P(k, SETTING[settingsID_int[i]]) == 0) {
          *settingsPtr_int[i] = atoi(v);
          sprintf(resultBuf, "%d", *settingsPtr_int[i]);
          found = true;
          break;
        }
      }

      if (found) {
      }else if (strcmp_P(k, SETTING[S_ACCELERATION_FORWARD]) == 0) {
        ACCELERATION_FORWARD = abs(atof(v));
        dtostrf(ACCELERATION_FORWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_DECELERATION_FORWARD]) == 0) {
        DECELERATION_FORWARD = abs(atof(v));
        dtostrf(DECELERATION_FORWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_ACCELERATION_BACKWARD]) == 0) {
        ACCELERATION_BACKWARD = abs(atof(v));
        dtostrf(ACCELERATION_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_DECELERATION_BACKWARD]) == 0) {
        DECELERATION_BACKWARD = abs(atof(v));
        dtostrf(DECELERATION_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_ACCELERATION_TURNING]) == 0) {
        ACCELERATION_TURNING = abs(atof(v));
        dtostrf(ACCELERATION_TURNING, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_DECELERATION_TURNING]) == 0) {
        DECELERATION_TURNING = abs(atof(v));
        dtostrf(DECELERATION_TURNING, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_FASTEST_FORWARD]) == 0) {
        FASTEST_FORWARD = constrain(abs(atof(v)), 0, 1);
        dtostrf(FASTEST_FORWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_FASTEST_BACKWARD]) == 0) {
        FASTEST_BACKWARD = constrain(abs(atof(v)), 0, 1);
        dtostrf(FASTEST_BACKWARD, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_TURN_SPEED]) == 0) {
        TURN_SPEED = constrain(abs(atof(v)), 0, 1);
        dtostrf(TURN_SPEED, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_SCALE_TURNING_WHEN_MOVING]) == 0) {
        SCALE_TURNING_WHEN_MOVING = atof(v);
        dtostrf(SCALE_TURNING_WHEN_MOVING, 0, 4, resultBuf);
      } else if (strcmp_P(k, SETTING[S_REVERSE_TURN_IN_REVERSE]) == 0) {
        REVERSE_TURN_IN_REVERSE = atoi(v);
        printTrueOrFalse(REVERSE_TURN_IN_REVERSE);
      } else if (strcmp_P(k, SETTING[S_USE_SPEED_KNOB]) == 0) {
        USE_SPEED_KNOB = atoi(v);
        printTrueOrFalse(USE_SPEED_KNOB);
      } else if (strcmp_P(k, SETTING[S_SCALE_ACCEL_WITH_SPEED]) == 0) {
        SCALE_ACCEL_WITH_SPEED = atoi(v);
        printTrueOrFalse(SCALE_ACCEL_WITH_SPEED);
      } else if (strcmp_P(k, SETTING[S_SPEED_KNOB_PIN]) == 0) {
        SPEED_KNOB_PIN = atoi(v);
        pinMode(SPEED_KNOB_PIN, INPUT);
        sprintf(resultBuf, "%d", SPEED_KNOB_PIN);
      } else if (strcmp_P(k, SETTING[S_JOY_X_PIN]) == 0) {
        JOY_X_PIN = atoi(v);
        pinMode(JOY_X_PIN, INPUT);
        sprintf(resultBuf, "%d", JOY_X_PIN);
      } else if (strcmp_P(k, SETTING[S_JOY_Y_PIN]) == 0) {
        JOY_Y_PIN = atoi(v);
        pinMode(JOY_Y_PIN, INPUT);
        sprintf(resultBuf, "%d", JOY_Y_PIN);
#ifdef IS_PCB
      } else if (strcmp_P(k, SETTING[S_SWAP_MOTORS]) == 0) {
        SWAP_MOTORS = atoi(v);
        printTrueOrFalse(SWAP_MOTORS);
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        leftMotorController.detach();
        rightMotorController.detach();
        if (SWAP_MOTORS) {
          leftMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
          rightMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
        } else {
          leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
          rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
        }
#else // not pcb
      } else if (strcmp_P(k, SETTING[S_LEFT_MOTOR_CONTROLLER_PIN]) == 0) {
        // detach both because when swapping left and right we don't want both attached to the same pin
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        rightMotorController.detach();
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        leftMotorController.detach();
        LEFT_MOTOR_CONTROLLER_PIN = atoi(v);
        delay(10);
        leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        sprintf(resultBuf, "%d", LEFT_MOTOR_CONTROLLER_PIN);
      } else if (strcmp_P(k, SETTING[S_RIGHT_MOTOR_CONTROLLER_PIN]) == 0) {
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        leftMotorController.detach();
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        rightMotorController.detach();
        RIGHT_MOTOR_CONTROLLER_PIN = atoi(v);
        delay(10);
        rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
        rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
        leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
        leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
        sprintf(resultBuf, "%d", RIGHT_MOTOR_CONTROLLER_PIN);
#endif

      } else if (strcmp_P(k, SETTING[S_ENABLE_STARTUP_PULSE]) == 0) {
        ENABLE_STARTUP_PULSE = atoi(v);
        printTrueOrFalse(ENABLE_STARTUP_PULSE);
      } else if (strcmp_P(k, SETTING[S_START_MOTOR_PULSE_TIME]) == 0) {
        START_MOTOR_PULSE_TIME = constrain(atoi(v), 0, 1000);
        sprintf(resultBuf, "%d", START_MOTOR_PULSE_TIME);
      } else if (strcmp_P(k, SETTING[S_ENABLE_BUTTON_CTRL]) == 0) {
        ENABLE_BUTTON_CTRL = atoi(v);
        if (ENABLE_BUTTON_CTRL) {
          if (USE_BUTTON_MODE_PIN) {
            pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
          }
          for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
            pinMode(driveButtons[i].pin, INPUT_PULLUP);
          }
        }
        printTrueOrFalse(ENABLE_BUTTON_CTRL);
      } else if (strcmp_P(k, SETTING[S_USE_BUTTON_MODE_PIN]) == 0) {
        USE_BUTTON_MODE_PIN = atoi(v);
        if (ENABLE_BUTTON_CTRL) {
          if (USE_BUTTON_MODE_PIN) {
            pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
          }
          for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
            pinMode(driveButtons[i].pin, INPUT_PULLUP);
          }
        }
        printTrueOrFalse(USE_BUTTON_MODE_PIN);
      } else if (strcmp_P(k, SETTING[S_BUTTON_MODE_TOGGLE]) == 0) {
        BUTTON_MODE_TOGGLE = atoi(v);
        printTrueOrFalse(BUTTON_MODE_TOGGLE);
      } else if (strcmp_P(k, SETTING[S_BUTTON_MODE_PIN]) == 0) {
        BUTTON_MODE_PIN = atoi(v);
        pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
        sprintf(resultBuf, "%d", BUTTON_MODE_PIN);
      } else if (strcmp_P(k, SETTING[S_NUM_DRIVE_BUTTONS]) == 0) {
        NUM_DRIVE_BUTTONS = constrain(atoi(v), 0, 6);
        if (ENABLE_BUTTON_CTRL) {
          if (USE_BUTTON_MODE_PIN) {
            pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
          }
          for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
            pinMode(driveButtons[i].pin, INPUT_PULLUP);
          }
        }
        sprintf(resultBuf, "%d", NUM_DRIVE_BUTTONS);
      } else if (strcmp(k, "DRIVE_BUTTONS") == 0) {
        int db = atoi(v); // which row of the driveButtons array?
        if (db >= 1 && db <= maxNumDriveButtons) { // a valid driveButtons index
          db--; // convert from array indices starting at 1 to starting at 0
          strtok(v, "_"); // the value after db
          driveButtons[db].pin   = atoi(strtok(0, "_"));
          driveButtons[db].speed = atof(strtok(0, "_"));
          driveButtons[db].turn  = atof(strtok(0, "_"));

          pinMode(driveButtons[db].pin, INPUT_PULLUP);

          Serial.print(F("{\"result\": \"change\""));
          Serial.print(",");
          Serial.print(F("\"setting\": \""));
          Serial.print("DRIVE_BUTTON_");
          Serial.print(db + 1);
          Serial.print("\",");
          Serial.print(F("\"value\":["));

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
      else if (strcmp_P(k, SETTING[S_STEERING_OFF_SWITCH]) == 0) {
        STEERING_OFF_SWITCH = atoi(v);
        if (STEERING_OFF_SWITCH) {
          pinMode(STEERING_OFF_SWITCH_PIN, INPUT_PULLUP);
          sprintf(resultBuf, "true");
        }
        else
          sprintf(resultBuf, "false");
      }
      else if (strcmp_P(k, SETTING[S_STEERING_OFF_SWITCH_PIN]) == 0) {
        STEERING_OFF_SWITCH_PIN = atoi(v);
        pinMode(STEERING_OFF_SWITCH_PIN, INPUT_PULLUP);
        sprintf(resultBuf, "%d", STEERING_OFF_SWITCH_PIN);
      }
      else if (strcmp_P(k, SETTING[S_USE_RC_CONTROL]) == 0) {
        USE_RC_CONTROL = atoi(v);
        if (USE_RC_CONTROL) {
          detachRCControl();
          sprintf(resultBuf, "true");
          setupRCControl();
        } else {
          detachRCControl();
          sprintf(resultBuf, "false");
        }
      } else if (strcmp_P(k, SETTING[S_TURN_RC_PIN]) == 0) {
        detachRCControl();
        RC_PIN[TURN_RC] = atoi(v);
        setupRCControl();
        sprintf(resultBuf, "%d", RC_PIN[TURN_RC]);
      } else if (strcmp_P(k, SETTING[S_SPEED_RC_PIN]) == 0) {
        detachRCControl();
        RC_PIN[SPEED_RC] = atoi(v);
        setupRCControl();
        sprintf(resultBuf, "%d", RC_PIN[SPEED_RC]);
      } else if (strcmp_P(k, SETTING[S_CTRL_RC_PIN]) == 0) {
        detachRCControl();
        RC_PIN[CTRL_RC] = atoi(v);
        setupRCControl();
        sprintf(resultBuf, "%d", RC_PIN[CTRL_RC]);
      } else if (strcmp_P(k, SETTING[S_STOP_RC_PIN]) == 0) {
        detachRCControl();
        RC_PIN[STOP_RC] = atoi(v);
        setupRCControl();
        sprintf(resultBuf, "%d", RC_PIN[STOP_RC]);
      } else if (strcmp_P(k, SETTING[S_NO_RC_STOP_UNTIL_START]) == 0) {
        NO_RC_STOP_UNTIL_START = atoi(v);
        printTrueOrFalse(NO_RC_STOP_UNTIL_START);
      } else if (strcmp_P(k, SETTING[S_USE_STOP_SWITCH]) == 0) {
        USE_STOP_SWITCH = atoi(v);
        if (USE_STOP_SWITCH) {
          pinMode(STOP_PIN, INPUT_PULLUP);
        }
        printTrueOrFalse(USE_STOP_SWITCH);
      } else if (strcmp_P(k, SETTING[S_ON_OFF_SWITCH_PIN]) == 0) {
        STOP_PIN = atoi(v);
        if (USE_STOP_SWITCH) {
          pinMode(STOP_PIN, INPUT_PULLUP);
        }
        sprintf(resultBuf, "%d", STOP_PIN);
      } else if (strcmp_P(k, SETTING[S_ON_OFF_ACTIVE_HIGH]) == 0) {
        STOP_PIN_HIGH = atoi(v);
        printTrueOrFalse(STOP_PIN_HIGH);
      } else if (strcmp_P(k, SETTING[S_ARM_STOP_AFTER_START]) == 0) {
        NO_STOP_UNTIL_START = atoi(v);
        printTrueOrFalse(NO_STOP_UNTIL_START);

      } else if (strcmp_P(k, SETTING[S_USE_ON_OFF_BUTTONS]) == 0) {
        USE_ON_OFF_BUTTONS = atoi(v);
        if (USE_ON_OFF_BUTTONS) {
          pinMode(ON_BUTTON, INPUT_PULLUP);
          pinMode(OFF_BUTTON, INPUT_PULLUP);
        }
        printTrueOrFalse(USE_ON_OFF_BUTTONS);
      } else if (strcmp_P(k, SETTING[S_ON_BUTTON]) == 0) {
        ON_BUTTON = atoi(v);
        pinMode(ON_BUTTON, INPUT_PULLUP);
        sprintf(resultBuf, "%d", ON_BUTTON);
      } else if (strcmp_P(k, SETTING[S_OFF_BUTTON]) == 0) {
        OFF_BUTTON = atoi(v);
        pinMode(OFF_BUTTON, INPUT_PULLUP);
        sprintf(resultBuf, "%d", OFF_BUTTON);
      } else if (strcmp_P(k, SETTING[S_ON_OFF_BUTTONS_ACTIVE_HIGH]) == 0) {
        ON_OFF_BUTTONS_ACTIVE_HIGH = atoi(v);
        printTrueOrFalse(ON_OFF_BUTTONS_ACTIVE_HIGH);
      }

#if defined(HAS_WIFI)
      else if (strcmp_P(k, SETTING[S_CAR_WIFI_NAME]) == 0) {
        CAR_WIFI_NAME = constrain(atoi(v), 0, 99);
        sprintf(resultBuf, "%d", CAR_WIFI_NAME);
      }
      else if (strcmp_P(k, SETTING[S_CAR_WIFI_PASSWORD]) == 0) {
        CAR_WIFI_PASSWORD = atoi(v);
        CAR_WIFI_PASSWORD = constrain(CAR_WIFI_PASSWORD, 0, 999999999);
        sprintf(resultBuf, "%d", CAR_WIFI_PASSWORD);
      }
      else if (strcmp_P(k, SETTING[S_USE_WIFI]) == 0) {
        USE_WIFI = atoi(v);
        if (USE_WIFI)
          sprintf(resultBuf, "true");
        else
          sprintf(resultBuf, "false");
      }
#endif
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
#if defined(FAKE_EEPROM)
        EEPROM.commit();
#endif
#ifdef ESP32
        ESP.restart();
#endif
        delay(5000); // trigger wdt
      } else if (strcmp(k, "REBOOT") == 0) {
#ifdef ESP32
        ESP.restart();
#endif
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
#ifdef IS_PCB
  EEPROMwrite(addressW, SWAP_MOTORS);
#else
  EEPROMwrite(addressW, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMwrite(addressW, RIGHT_MOTOR_CONTROLLER_PIN);
#endif
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
  EEPROMwrite(addressW, USE_RC_CONTROL);
  EEPROMwrite(addressW, RC_PIN[TURN_RC]);
  EEPROMwrite(addressW, RC_PIN[SPEED_RC]);
  EEPROMwrite(addressW, RC_PIN[CTRL_RC]);
  EEPROMwrite(addressW, RC_PIN[STOP_RC]);
  EEPROMwrite(addressW, NO_RC_STOP_UNTIL_START);

  EEPROMwrite(addressW, USE_STOP_SWITCH);
  EEPROMwrite(addressW, STOP_PIN);
  EEPROMwrite(addressW, STOP_PIN_HIGH);
  EEPROMwrite(addressW, NO_STOP_UNTIL_START);

  EEPROMwrite(addressW, USE_ON_OFF_BUTTONS);
  EEPROMwrite(addressW, ON_BUTTON);
  EEPROMwrite(addressW, OFF_BUTTON);
  EEPROMwrite(addressW, ON_OFF_BUTTONS_ACTIVE_HIGH);

#if defined(HAS_WIFI)
  EEPROMwrite(addressW, CAR_WIFI_NAME);
  EEPROMwrite(addressW, CAR_WIFI_PASSWORD);
  EEPROMwrite(addressW, USE_WIFI);
#endif
  EEPROMwrite(addressW, eepromCRC);
  // addressW equaled 177 on pico W on this line

#if defined(FAKE_EEPROM)
  EEPROM.commit();
#endif
}
void recallSettings()
{
  unsigned int addressR = 1;
  eepromCRC = 0;
  errorCorrectionPerformed = false;
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
#ifdef IS_PCB
  EEPROMread(addressR, SWAP_MOTORS);
#else
  EEPROMread(addressR, LEFT_MOTOR_CONTROLLER_PIN);
  EEPROMread(addressR, RIGHT_MOTOR_CONTROLLER_PIN);
#endif
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

  EEPROMread(addressR, USE_RC_CONTROL);
  EEPROMread(addressR, RC_PIN[TURN_RC]);
  EEPROMread(addressR, RC_PIN[SPEED_RC]);
  EEPROMread(addressR, RC_PIN[CTRL_RC]);
  EEPROMread(addressR, RC_PIN[STOP_RC]);
  EEPROMread(addressR, NO_RC_STOP_UNTIL_START);

  EEPROMread(addressR, USE_STOP_SWITCH);
  EEPROMread(addressR, STOP_PIN);
  EEPROMread(addressR, STOP_PIN_HIGH);
  EEPROMread(addressR, NO_STOP_UNTIL_START);

  EEPROMread(addressR, USE_ON_OFF_BUTTONS);
  EEPROMread(addressR, ON_BUTTON);
  EEPROMread(addressR, OFF_BUTTON);
  EEPROMread(addressR, ON_OFF_BUTTONS_ACTIVE_HIGH);

#if defined(HAS_WIFI)
  EEPROMread(addressR, CAR_WIFI_NAME);
  EEPROMread(addressR, CAR_WIFI_PASSWORD);
  EEPROMread(addressR, USE_WIFI);
#endif

  uint32_t tempEepromCRC = eepromCRC;
  uint32_t readCRC = 0;
  EEPROMread(addressR, readCRC);

#if defined(FAKE_EEPROM)
  if (errorCorrectionPerformed) {
    EEPROM.commit(); // rp2040 EEPROM library requires this to be used to write the updated data to the flash that is simulating EEPROM (flash has more limited cycles)
  }
#endif

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
#ifdef AVR // setting all pins to input messes up rpi picos
    for (byte pin = 2; pin <= 100; pin++) {
      pinMode(pin, INPUT);
    }
#endif

    LED_SETUP;
    delay(50);

#ifdef AVR
    wdt_disable();
#endif
    while (true) { // flash SOS forever
      Serial.println(F("{\"error\": \"eeprom failure\"}"));

      for (byte j = 0; j < 3; j++) { // S O S
        int16_t t = (j == 1) ? 600 : 200;
        for (byte i = 0; i < 3; i++) {
          LED_ON; delay(t); LED_OFF; delay(200); 
#if defined(IS_PICO)
          rp2040.wdt_reset();
#endif
        }
        delay(j < 2 ? 400 : 1400);
      }
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
    EEPROM.write(address, *p); // Three copies of the data are stored in EEPROM to detect and correct single bit errors
    EEPROM.write(address + repeat_space, *p);
    EEPROM.write(address + repeat_space * 2, *p);
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
      EEPROM.write(address, *p); // replace all three copies with the majority value
      EEPROM.write(address + repeat_space, *p);
      EEPROM.write(address + repeat_space * 2, *p);

      errorCorrectionPerformed = true;
      LED_SETUP;
      LED_OFF;
      delay(100);
      LED_ON;
      delay(50);
      // correcting errors can take a while. don't let the watchdog timeout
#ifdef AVR
      wdt_reset();
#elif defined(IS_PICO)
      rp2040.wdt_reset();
#endif

    }
    eepromCRC = crc_update(eepromCRC, *p);
    address++;
    p++;
  }
}

void printTrueOrFalse(bool val) {
  if (val)
    sprintf(resultBuf, "true");
  else
    sprintf(resultBuf, "false");
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
