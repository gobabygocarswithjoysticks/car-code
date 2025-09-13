#include "settings_names.h"
int serialChecksum = 0;
boolean printVariables(int interval) {
  ///// if interval is -1, nothing is printed /////
  if (interval < 0)
    return false;
  if (millis() - lastMillisPrintedValues >= (unsigned int)interval) {
    lastMillisPrintedValues = millis();
    serialChecksum = 1; Serial.print("{");
    prnt(F("current values, millis:"), millis());
    prnt(F("joyXVal"), joyXVal);
    prnt(F("joyYVal"), joyYVal);
    prntf(F("turnInput"), turnInput);
    prntf(F("speedInput"), speedInput);
    prntf(F("turnProcessed"), turnProcessed);
    prntf(F("speedProcessed"), speedProcessed);
    prntf(F("turnToDrive"), turnToDrive);
    prntf(F("speedToDrive"), speedToDrive);
    prnt(F("leftMotorWriteVal"), leftMotorWriteVal);
    prnt(F("rightMotorWriteVal"), rightMotorWriteVal);
    prntf(F("speedKnobVal"), speedKnobVal);
    prntbool(F("movementAllowed"), movementAllowed);
    prntbool(F("joyOK"), joyOK);
    int buttonBits = 0;
    if (ENABLE_BUTTON_CTRL) {
      int button;
      for (button = 0; button < maxNumDriveButtons; button++) {
        if (digitalRead(driveButtons[button].pin) == BUTTONS_ACTIVE_HIGH)
          bitSet(buttonBits, button);
      }

      // extra MSB set to 1 if buttons active, to indicate the size of maxNumDriveButtons
      if (ENABLE_BUTTON_CTRL) {
        if (USE_BUTTON_MODE_PIN) {
          if (buttonModeActive) {
            bitSet(buttonBits, button);
          }
        } else { // mode pin use = false; buttons on
          bitSet(buttonBits, button);
        }
      } //else  buttons off
    }
    prnt(F("buttons"), buttonBits);
    serialChecksum += Serial.print(F("\"b_m_p\":"));
    if (rcFlags.RCOverride) {
      serialChecksum += Serial.print("\"R\"");
    } else {
      if (ENABLE_BUTTON_CTRL) {
        if (USE_BUTTON_MODE_PIN) {
          if (buttonModeActive)
            serialChecksum += Serial.print("\"Y\"");
          else
            serialChecksum += Serial.print("\"N\"");
        } else { // mode pin use = false; buttons on
          serialChecksum += Serial.print("\"A\"");
        }
      } else { // buttons off
        serialChecksum += Serial.print("\"B\"");
      }
    }

    serialChecksum += Serial.print(F(",\"CHECKSUM\":"));
    Serial.print(serialChecksum + 1 /*closing bracket*/ + 3 /*serialChecksum is in the hundreds and takes 3 characters*/);
    Serial.println("}");
    return true;
  }
  return false;
}

#if defined(HAS_WIFI)
bool ptw = false;
char wifiSettingsBuffer[3000];
int wifiBufI = 0;
void printSettings(bool printToWifi) {
  ptw = printToWifi;
  wifiBufI = 0;
#else
void printSettings() {
#endif
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  serialChecksum = 2; // starting and ending brackets
#if defined(HAS_WIFI)
  if (printToWifi) {
    wifiSettingsBuffer[wifiBufI++] = '{';
  } else {
    Serial.print(F("{"));
  }
#else
  Serial.print(F("{"));
#endif

  prnt(F("current settings, version:"), version_number);
  prnt(FV(SETTING[S_CONTROL_RIGHT]), CONTROL_RIGHT);
  prnt(FV(SETTING[S_CONTROL_CENTER_X]), CONTROL_CENTER_X);
  prnt(FV(SETTING[S_CONTROL_LEFT]), CONTROL_LEFT);
  prnt(FV(SETTING[S_X_DEADZONE]), X_DEADZONE);
  prnt(FV(SETTING[S_CONTROL_UP]), CONTROL_UP);
  prnt(FV(SETTING[S_CONTROL_CENTER_Y]), CONTROL_CENTER_Y);
  prnt(FV(SETTING[S_CONTROL_DOWN]), CONTROL_DOWN);
  prnt(FV(SETTING[S_Y_DEADZONE]), Y_DEADZONE);
  prntf(FV(SETTING[S_ACCELERATION_FORWARD]), ACCELERATION_FORWARD);
  prntf(FV(SETTING[S_DECELERATION_FORWARD]), DECELERATION_FORWARD);
  prntf(FV(SETTING[S_ACCELERATION_BACKWARD]), ACCELERATION_BACKWARD);
  prntf(FV(SETTING[S_DECELERATION_BACKWARD]), DECELERATION_BACKWARD);
  prntf(FV(SETTING[S_ACCELERATION_TURNING]), ACCELERATION_TURNING);
  prntf(FV(SETTING[S_DECELERATION_TURNING]), DECELERATION_TURNING);
  prntf(FV(SETTING[S_FASTEST_FORWARD]), FASTEST_FORWARD);
  prntf(FV(SETTING[S_FASTEST_BACKWARD]), FASTEST_BACKWARD);
  prntf(FV(SETTING[S_TURN_SPEED]), TURN_SPEED);
  prntf(FV(SETTING[S_SCALE_TURNING_WHEN_MOVING]), SCALE_TURNING_WHEN_MOVING);
  prntbool(FV(SETTING[S_REVERSE_TURN_IN_REVERSE]), REVERSE_TURN_IN_REVERSE);
  prnt(FV(SETTING[S_LEFT_MOTOR_CENTER]), LEFT_MOTOR_CENTER);
  prnt(FV(SETTING[S_LEFT_MOTOR_SLOW]), LEFT_MOTOR_SLOW);
  prnt(FV(SETTING[S_LEFT_MOTOR_FAST]), LEFT_MOTOR_FAST);
  prnt(FV(SETTING[S_RIGHT_MOTOR_CENTER]), RIGHT_MOTOR_CENTER);
  prnt(FV(SETTING[S_RIGHT_MOTOR_SLOW]), RIGHT_MOTOR_SLOW);
  prnt(FV(SETTING[S_RIGHT_MOTOR_FAST]), RIGHT_MOTOR_FAST);
  prntbool(FV(SETTING[S_USE_SPEED_KNOB]), USE_SPEED_KNOB);
  prnt(FV(SETTING[S_SPEED_KNOB_SLOW_VAL]), SPEED_KNOB_SLOW_VAL);
  prnt(FV(SETTING[S_SPEED_KNOB_FAST_VAL]), SPEED_KNOB_FAST_VAL);
  prntbool(FV(SETTING[S_SCALE_ACCEL_WITH_SPEED]), SCALE_ACCEL_WITH_SPEED);
  prnt(FV(SETTING[S_SPEED_KNOB_PIN]), SPEED_KNOB_PIN);
  prnt(FV(SETTING[S_JOY_X_PIN]), JOY_X_PIN);
  prnt(FV(SETTING[S_JOY_Y_PIN]), JOY_Y_PIN);

#ifdef IS_PCB
  prntbool(FV(SETTING[S_SWAP_MOTORS]), SWAP_MOTORS);
#else
  prnt(FV(SETTING[S_LEFT_MOTOR_CONTROLLER_PIN]), LEFT_MOTOR_CONTROLLER_PIN);
  prnt(FV(SETTING[S_RIGHT_MOTOR_CONTROLLER_PIN]), RIGHT_MOTOR_CONTROLLER_PIN);
#endif
  prntbool(FV(SETTING[S_ENABLE_STARTUP_PULSE]), ENABLE_STARTUP_PULSE);
  prnt(FV(SETTING[S_LEFT_MOTOR_PULSE]), LEFT_MOTOR_PULSE);
  prnt(FV(SETTING[S_RIGHT_MOTOR_PULSE]), RIGHT_MOTOR_PULSE);
  prnt(FV(SETTING[S_START_MOTOR_PULSE_TIME]), START_MOTOR_PULSE_TIME);
  prnt(FV(SETTING[S_JOY_CALIB_COUNT]), JOY_CALIB_COUNT);
  prntbool(FV(SETTING[S_ENABLE_BUTTON_CTRL]), ENABLE_BUTTON_CTRL);
  prntbool(FV(SETTING[S_USE_BUTTON_MODE_PIN]), USE_BUTTON_MODE_PIN);
  prnt(FV(SETTING[S_BUTTON_MODE_PIN]), BUTTON_MODE_PIN);
  prntbool(FV(SETTING[S_BUTTON_MODE_TOGGLE]), BUTTON_MODE_TOGGLE);
  prntbool(FV(SETTING[S_BUTTONS_ACTIVE_HIGH]), BUTTONS_ACTIVE_HIGH);
  prnt(FV(SETTING[S_NUM_DRIVE_BUTTONS]), NUM_DRIVE_BUTTONS);

  for (byte db = 0; db < maxNumDriveButtons; db++) {
    printAndAppendToChecksum(F("\"DRIVE_BUTTON_"));
#if defined(HAS_WIFI)
    if (printToWifi) {
      wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%d", db + 1);
      wifiSettingsBuffer[wifiBufI++] = '"';
      wifiSettingsBuffer[wifiBufI++] = ':';
      wifiSettingsBuffer[wifiBufI++] = '[';
      wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%d", driveButtons[db].pin);
      wifiSettingsBuffer[wifiBufI++] = ',';
      wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%.4f", driveButtons[db].speed);
      wifiSettingsBuffer[wifiBufI++] = ',';
      wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%.4f", driveButtons[db].turn);
      wifiSettingsBuffer[wifiBufI++] = ']';
      wifiSettingsBuffer[wifiBufI++] = ',';
    } else {
      serialChecksum += Serial.print(db + 1);
      printAndAppendToChecksum(F("\":["));
      serialChecksum += Serial.print(driveButtons[db].pin);
      printAndAppendToChecksum(F(","));
      serialChecksum += Serial.print(driveButtons[db].speed, 4);
      printAndAppendToChecksum(F(","));
      serialChecksum += Serial.print(driveButtons[db].turn, 4);
      printAndAppendToChecksum(F("],"));
    }
#else
    serialChecksum += Serial.print(db + 1);
    printAndAppendToChecksum(F("\":["));
    serialChecksum += Serial.print(driveButtons[db].pin);
    printAndAppendToChecksum(F(","));
    serialChecksum += Serial.print(driveButtons[db].speed, 4);
    printAndAppendToChecksum(F(","));
    serialChecksum += Serial.print(driveButtons[db].turn, 4);
    printAndAppendToChecksum(F("],"));
#endif
  }
  prntbool(FV(SETTING[S_STEERING_OFF_SWITCH]), STEERING_OFF_SWITCH);
  prnt(FV(SETTING[S_STEERING_OFF_SWITCH_PIN]), STEERING_OFF_SWITCH_PIN);

  prntbool(FV(SETTING[S_USE_RC_CONTROL]), USE_RC_CONTROL);
  prnt(FV(SETTING[S_SPEED_RC_PIN]), RC_PIN[SPEED_RC]);
  prnt(FV(SETTING[S_TURN_RC_PIN]), RC_PIN[TURN_RC]);
  prnt(FV(SETTING[S_CTRL_RC_PIN]), RC_PIN[CTRL_RC]);
  prnt(FV(SETTING[S_STOP_RC_PIN]), RC_PIN[STOP_RC]);
  prntbool(FV(SETTING[S_NO_RC_STOP_UNTIL_START]), NO_RC_STOP_UNTIL_START);
  prntbool(FV(SETTING[S_USE_STOP_SWITCH]), USE_STOP_SWITCH);
  prnt(FV(SETTING[S_ON_OFF_SWITCH_PIN]), STOP_PIN);
  prntbool(FV(SETTING[S_ON_OFF_ACTIVE_HIGH]), STOP_PIN_HIGH);
  prntbool(FV(SETTING[S_ARM_STOP_AFTER_START]), NO_STOP_UNTIL_START);
  prntbool(FV(SETTING[S_USE_ON_OFF_BUTTONS]), USE_ON_OFF_BUTTONS);
  prnt(FV(SETTING[S_ON_BUTTON]), ON_BUTTON);
  prnt(FV(SETTING[S_OFF_BUTTON]), OFF_BUTTON);
  prntbool(FV(SETTING[S_ON_OFF_BUTTONS_ACTIVE_HIGH]), ON_OFF_BUTTONS_ACTIVE_HIGH);

#if defined(HAS_WIFI)
  prntbool(FV(SETTING[S_USE_WIFI]), USE_WIFI);
  prnt(FV(SETTING[S_CAR_WIFI_NAME]), CAR_WIFI_NAME);
  prnt(FV(SETTING[S_CAR_WIFI_PASSWORD]), CAR_WIFI_PASSWORD);
#endif

#if defined(HAS_WIFI)
  if (printToWifi) {
    wifiBufI-=2; // delete space and comma from prnt CAR_WIFI_PASSWORD
    wifiSettingsBuffer[wifiBufI++] = '}';
    wifiSettingsBuffer[wifiBufI++ ] = '\0';
  } else {
    printAndAppendToChecksum(F("\"CHECKSUM\":"));
    Serial.print(serialChecksum + 4 /*serialChecksum is in the thousands so 4 characters*/);
    Serial.println("}");
  }
  ptw = false;
#else
  printAndAppendToChecksum(F("\"CHECKSUM\":"));
  Serial.print(serialChecksum + 4 /*serialChecksum is in the thousands so 4 characters*/);
  Serial.println("}");
#endif

}

void prnt(const __FlashStringHelper *fsh, int value) {
#if defined(HAS_WIFI)
  if (ptw) {
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%S", fsh);
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    wifiSettingsBuffer[wifiBufI++ ] = ':';
    wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%d", value);
    wifiSettingsBuffer[wifiBufI++ ] = ',';
    wifiSettingsBuffer[wifiBufI++ ] = ' ';
  } else {
    serialChecksum += 5;
    Serial.print("\"");
    serialChecksum += Serial.print(fsh);
    Serial.print("\":");
    serialChecksum += Serial.print(value);
    Serial.print(", ");
  }
#else
  serialChecksum += 5;
  Serial.print("\"");
  serialChecksum += Serial.print(fsh);
  Serial.print("\":");
  serialChecksum += Serial.print(value);
  Serial.print(", ");
#endif
}

#if defined(HAS_WIFI)
char stringLabel[50];
#endif
void prntf(const __FlashStringHelper *fsh, float value) {
#if defined(HAS_WIFI)
  if (ptw) {
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    strcpy_P(stringLabel, (char *)fsh);
    wifiBufI += sprintf_P(wifiSettingsBuffer + wifiBufI, "%s", stringLabel);
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    wifiSettingsBuffer[wifiBufI++ ] = ':';
    wifiBufI += sprintf(wifiSettingsBuffer + wifiBufI, "%.4f", value);
    wifiSettingsBuffer[wifiBufI++ ] = ',';
    wifiSettingsBuffer[wifiBufI++ ] = ' ';
  } else {
    serialChecksum += 5;
    Serial.print("\"");
    serialChecksum += Serial.print(fsh);
    Serial.print("\":");
    serialChecksum += Serial.print(value, 4);
    Serial.print(", ");
  }
#else
  serialChecksum += 5;
  Serial.print("\"");
  serialChecksum += Serial.print(fsh);
  Serial.print("\":");
  serialChecksum += Serial.print(value, 4);
  Serial.print(", ");
#endif
}

void prntbool(const __FlashStringHelper *fsh, boolean value) {
#if defined(HAS_WIFI)
  if (ptw) {
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    strcpy_P(stringLabel, (char *)fsh);
    wifiBufI += sprintf_P(wifiSettingsBuffer + wifiBufI, "%s", stringLabel);
    wifiSettingsBuffer[wifiBufI++ ] = '"';
    wifiSettingsBuffer[wifiBufI++ ] = ':';
    if (value) {
      wifiSettingsBuffer[wifiBufI++ ] = 't';
      wifiSettingsBuffer[wifiBufI++ ] = 'r';
      wifiSettingsBuffer[wifiBufI++ ] = 'u';
      wifiSettingsBuffer[wifiBufI++ ] = 'e';
      wifiSettingsBuffer[wifiBufI++ ] = ',';
      wifiSettingsBuffer[wifiBufI++ ] = ' ';
    } else {
      wifiSettingsBuffer[wifiBufI++ ] = 'f';
      wifiSettingsBuffer[wifiBufI++ ] = 'a';
      wifiSettingsBuffer[wifiBufI++ ] = 'l';
      wifiSettingsBuffer[wifiBufI++ ] = 's';
      wifiSettingsBuffer[wifiBufI++ ] = 'e';
      wifiSettingsBuffer[wifiBufI++ ] = ',';
      wifiSettingsBuffer[wifiBufI++ ] = ' ';
    }
  } else {
    serialChecksum += 3; // for the three total characters, " and ":
    Serial.print("\"");
    serialChecksum += Serial.print(fsh);
    Serial.print("\":");
    if (value) {
      serialChecksum += Serial.print("true, ");
    } else {
      serialChecksum += Serial.print("false, ");
    }
  }
#else
  serialChecksum += 3; // for the three total characters, " and ":
  Serial.print("\"");
  serialChecksum += Serial.print(fsh);
  Serial.print("\":");
  if (value) {
    serialChecksum += Serial.print("true, ");
  } else {
    serialChecksum += Serial.print("false, ");
  }
#endif
}

void printAndAppendToChecksum(const __FlashStringHelper * fsh) {
#if defined(HAS_WIFI)
  if (ptw) {
    strcpy_P(stringLabel, (char *)fsh);
    wifiBufI += sprintf_P(wifiSettingsBuffer + wifiBufI, "%s", stringLabel);
  } else {
    serialChecksum += Serial.print(fsh);
  }
#else
  serialChecksum += Serial.print(fsh);
#endif
}
