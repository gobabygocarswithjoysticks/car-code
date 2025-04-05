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
    prnt(F("turnInput"), turnInput);
    prnt(F("speedInput"), speedInput);
    prnt(F("turnProcessed"), turnProcessed);
    prnt(F("speedProcessed"), speedProcessed);
    prnt(F("turnToDrive"), turnToDrive);
    prnt(F("speedToDrive"), speedToDrive);
    prnt(F("leftMotorWriteVal"), leftMotorWriteVal);
    prnt(F("rightMotorWriteVal"), rightMotorWriteVal);
    prnt(F("speedKnobVal"), speedKnobVal);
    prnt(F("movementAllowed"), movementAllowed ? "true" : "false");
    prnt(F("joyOK"), joyOK ? "true" : "false");
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
    prnt(F("buttons"), buttonBits);
    serialChecksum += Serial.print(F("\"b_m_p\":"));
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

    serialChecksum += Serial.print(F(",\"CHECKSUM\":"));
    Serial.print(serialChecksum + 1 /*closing bracket*/ + 3 /*serialChecksum is in the hundreds and takes 3 characters*/);
    Serial.println("}");
    return true;
  }
  return false;
}

void printSettings() {
  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  serialChecksum = 2; // starting and ending brackets
  Serial.print(F("{"));
  prnt(F("current settings, version:"), version_number);
  prnt(FV(SETTING[S_CONTROL_RIGHT]), CONTROL_RIGHT);
  prnt(FV(SETTING[S_CONTROL_CENTER_X]), CONTROL_CENTER_X);
  prnt(FV(SETTING[S_CONTROL_LEFT]), CONTROL_LEFT);
  prnt(FV(SETTING[S_X_DEADZONE]), X_DEADZONE);
  prnt(FV(SETTING[S_CONTROL_UP]), CONTROL_UP);
  prnt(FV(SETTING[S_CONTROL_CENTER_Y]), CONTROL_CENTER_Y);
  prnt(FV(SETTING[S_CONTROL_DOWN]), CONTROL_DOWN);
  prnt(FV(SETTING[S_Y_DEADZONE]), Y_DEADZONE);
  prnt(FV(SETTING[S_ACCELERATION_FORWARD]), ACCELERATION_FORWARD);
  prnt(FV(SETTING[S_DECELERATION_FORWARD]), DECELERATION_FORWARD);
  prnt(FV(SETTING[S_ACCELERATION_BACKWARD]), ACCELERATION_BACKWARD);
  prnt(FV(SETTING[S_DECELERATION_BACKWARD]), DECELERATION_BACKWARD);
  prnt(FV(SETTING[S_ACCELERATION_TURNING]), ACCELERATION_TURNING);
  prnt(FV(SETTING[S_DECELERATION_TURNING]), DECELERATION_TURNING);
  prnt(FV(SETTING[S_FASTEST_FORWARD]), FASTEST_FORWARD);
  prnt(FV(SETTING[S_FASTEST_BACKWARD]), FASTEST_BACKWARD);
  prnt(FV(SETTING[S_TURN_SPEED]), TURN_SPEED);
  prnt(FV(SETTING[S_SCALE_TURNING_WHEN_MOVING]), SCALE_TURNING_WHEN_MOVING);
  prnt(FV(SETTING[S_REVERSE_TURN_IN_REVERSE]), REVERSE_TURN_IN_REVERSE ? "true" : "false");
  prnt(FV(SETTING[S_LEFT_MOTOR_CENTER]), LEFT_MOTOR_CENTER);
  prnt(FV(SETTING[S_LEFT_MOTOR_SLOW]), LEFT_MOTOR_SLOW);
  prnt(FV(SETTING[S_LEFT_MOTOR_FAST]), LEFT_MOTOR_FAST);
  prnt(FV(SETTING[S_RIGHT_MOTOR_CENTER]), RIGHT_MOTOR_CENTER);
  prnt(FV(SETTING[S_RIGHT_MOTOR_SLOW]), RIGHT_MOTOR_SLOW);
  prnt(FV(SETTING[S_RIGHT_MOTOR_FAST]), RIGHT_MOTOR_FAST);
  prnt(FV(SETTING[S_USE_SPEED_KNOB]), USE_SPEED_KNOB ? "true" : "false");
  prnt(FV(SETTING[S_SPEED_KNOB_SLOW_VAL]), SPEED_KNOB_SLOW_VAL);
  prnt(FV(SETTING[S_SPEED_KNOB_FAST_VAL]), SPEED_KNOB_FAST_VAL);
  prnt(FV(SETTING[S_SCALE_ACCEL_WITH_SPEED]), SCALE_ACCEL_WITH_SPEED ? "true" : "false");
  prnt(FV(SETTING[S_SPEED_KNOB_PIN]), SPEED_KNOB_PIN);
  prnt(FV(SETTING[S_JOY_X_PIN]), JOY_X_PIN);
  prnt(FV(SETTING[S_JOY_Y_PIN]), JOY_Y_PIN);

#ifdef IS_PCB
  prnt(FV(SETTING[S_SWAP_MOTORS]), SWAP_MOTORS ? "true" : "false");
#else
  prnt(FV(SETTING[S_LEFT_MOTOR_CONTROLLER_PIN]), LEFT_MOTOR_CONTROLLER_PIN);
  prnt(FV(SETTING[S_RIGHT_MOTOR_CONTROLLER_PIN]), RIGHT_MOTOR_CONTROLLER_PIN);
#endif
  prnt(FV(SETTING[S_ENABLE_STARTUP_PULSE]), ENABLE_STARTUP_PULSE ? "true" : "false");
  prnt(FV(SETTING[S_LEFT_MOTOR_PULSE]), LEFT_MOTOR_PULSE);
  prnt(FV(SETTING[S_RIGHT_MOTOR_PULSE]), RIGHT_MOTOR_PULSE);
  prnt(FV(SETTING[S_START_MOTOR_PULSE_TIME]), START_MOTOR_PULSE_TIME);
  prnt(FV(SETTING[S_JOY_CALIB_COUNT]), JOY_CALIB_COUNT);
  prnt(FV(SETTING[S_ENABLE_BUTTON_CTRL]), ENABLE_BUTTON_CTRL ? "true" : "false");
  prnt(FV(SETTING[S_USE_BUTTON_MODE_PIN]), USE_BUTTON_MODE_PIN ? "true" : "false");
  prnt(FV(SETTING[S_BUTTON_MODE_PIN]), BUTTON_MODE_PIN);
  prnt(FV(SETTING[S_NUM_DRIVE_BUTTONS]), NUM_DRIVE_BUTTONS);

  for (byte db = 0; db < maxNumDriveButtons; db++) {
    printAndAppendToChecksum(F("\"DRIVE_BUTTON_"));
    serialChecksum += Serial.print(db + 1);
    printAndAppendToChecksum(F("\":["));
    serialChecksum += Serial.print(driveButtons[db].pin);
    printAndAppendToChecksum(F(","));
    serialChecksum += Serial.print(driveButtons[db].speed, 4);
    printAndAppendToChecksum(F(","));
    serialChecksum += Serial.print(driveButtons[db].turn, 4);
    printAndAppendToChecksum(F("],"));
  }
  prnt(FV(SETTING[S_STEERING_OFF_SWITCH]), STEERING_OFF_SWITCH ? "true" : "false");
  prnt(FV(SETTING[S_STEERING_OFF_SWITCH_PIN]), STEERING_OFF_SWITCH_PIN);

#if defined(RC_CONTROL)
  prnt(FV(SETTING[S_USE_RC_CONTROL]), USE_RC_CONTROL ? "true" : "false");
  prnt(FV(SETTING[S_SPEED_RC_PIN]), RC_PIN[TURN_RC]);
  prnt(FV(SETTING[S_TURN_RC_PIN]), RC_PIN[SPEED_RC]);
#endif 
#if defined(HAS_WIFI)
  prnt(FV(SETTING[S_CAR_WIFI_NAME]), CAR_WIFI_NAME);
  prnt(FV(SETTING[S_CAR_WIFI_PASSWORD]), CAR_WIFI_PASSWORD);
  prnt(FV(SETTING[S_USE_WIFI]), USE_WIFI ? "true" : "false");
#endif

  printAndAppendToChecksum(F("\"CHECKSUM\":")); 
  Serial.print(serialChecksum + 4 /*serialChecksum is in the thousands so 4 characters*/);
  Serial.println("}");

}

template <typename T>
void prnt(const __FlashStringHelper *fsh, T value) {
  serialChecksum += 5;
  Serial.print("\"");
  serialChecksum += Serial.print(fsh);
  Serial.print("\":");
  serialChecksum += Serial.print(value);
  Serial.print(", ");
}

void printAndAppendToChecksum(const __FlashStringHelper *fsh) {
  serialChecksum += Serial.print(fsh);
}
