#define rcTimeoutMicros 70000 // timeout for RC control
#if defined(IS_PICO) || defined(ESP32)
#else
#include <PinChangeInterrupt.h>
#endif

boolean buttonModeActive = false;

volatile unsigned long lastRisingMicros[NUM_RC_INPUTS];
volatile unsigned long anyRCRisingMillis;
volatile int16_t remoteInput[NUM_RC_INPUTS];
struct RCFlags { // https://en.cppreference.com/w/cpp/language/bit_field.html
  uint8_t everActivated: 1; // true if the RC signal has ever been received
  uint8_t RCOverride: 1;   // true if the RC control is overriding the joystick control
  uint8_t RCStop: 1;     // true if the RC control is stopping the car
  uint8_t RC_make_motors_e_stop: 1; // true if the RC control is making the motors emergency stop
  uint8_t Start_Stop_Buttons_e_stop: 1; // true if the start/stop buttons are making the motors emergency stop
  uint8_t Start_Switch_Ever_Activated: 1; // true if the start switch has ever been activated to start the car
  uint8_t buttonModeActive: 1; // used for toggling button mode
  uint8_t lastButtonModePinState: 1; // used for toggling button mode
} rcFlags; // a bunch of booleans for remote control, stored efficiently

void speedRCISR(void) {
  RCISR(SPEED_RC);
}
void turnRCISR(void) {
  RCISR(TURN_RC);
}
void ctrlRCISR(void) {
  RCISR(CTRL_RC);
}
void stopRCISR(void) {
  RCISR(STOP_RC);
}
// must match the enum for x_RC
void (*RCISRs[])() = {
  turnRCISR,
  speedRCISR,
  ctrlRCISR,
  stopRCISR
};

void RCISR(byte whichRCInput) {
  if (digitalRead(RC_PIN[whichRCInput]) == HIGH) {
    lastRisingMicros[whichRCInput] = micros();
    anyRCRisingMillis = millis();
  } else if ((micros() - lastRisingMicros[whichRCInput]) <= rcTimeoutMicros) {
    unsigned long pulseTime = micros() - lastRisingMicros[whichRCInput];
    if (pulseTime > 500 && pulseTime < 2500) { // only accept pulses between 500 and 2500 microseconds
      remoteInput[whichRCInput] = constrain((int16_t)pulseTime - 1500, -500, 500); // convert to range -500 to 500
    }
  }
}

void setupRCControl() {
  rcFlags.RCStop = true;
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    pinMode(RC_PIN[i], INPUT_PULLUP);
  }

#if defined(IS_PICO) || defined(ESP32)
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    attachInterrupt(digitalPinToInterrupt(RC_PIN[i]), RCISRs[i], CHANGE); // attach the ISR to all RC pins
  }
#else // uno or nano, needs PinChangeInterrupt library
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    attachPCINT(digitalPinToPCINT(RC_PIN[i]), RCISRs[i], CHANGE); // attach the ISR to all RC pins
  }
#endif
}
void detachRCControl() {
#if defined(IS_PICO) || defined(ESP32)
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    detachInterrupt(digitalPinToInterrupt(RC_PIN[i])); // attach the ISR to all RC pins
  }
#else // nano or uno, needs PinChangeInterrupt library
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    detachPCINT(digitalPinToPCINT(RC_PIN[i])); // attach the ISR to all RC pins
  }
#endif
}

void runRCInput(float &speed, float &turn) {
  if (USE_RC_CONTROL == false) {
    rcFlags.RCOverride = false;
    rcFlags.RC_make_motors_e_stop = false;
    return;
  }
  bool validSignal = ((millis() - anyRCRisingMillis) <= (rcTimeoutMicros / 1000)); // millis takes weeks to overflow so don't worry about it
  unsigned long copiedLastRisingMicros[NUM_RC_INPUTS];
  int16_t copiedRemoteInput[NUM_RC_INPUTS];

  noInterrupts(); // disable interrupts to copy the values safely
  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    copiedLastRisingMicros[i] = lastRisingMicros[i];
    copiedRemoteInput[i] = remoteInput[i];
  }
  interrupts(); // re-enable interrupts


  for (byte i = 0; i < NUM_RC_INPUTS; i++) {
    if ((micros() - copiedLastRisingMicros[i]) > rcTimeoutMicros) {
      validSignal = false;
    }
  }

  if (validSignal) {
    rcFlags.everActivated = true;
    if (copiedRemoteInput[CTRL_RC] > rcControlSwitchDeadband) {
      rcFlags.RCOverride = true;
    } else if (copiedRemoteInput[CTRL_RC] < -rcControlSwitchDeadband) {
      rcFlags.RCOverride = false;
    }
    if (copiedRemoteInput[STOP_RC] > rcControlSwitchDeadband) {
      rcFlags.RCStop = true;
    } else if (copiedRemoteInput[STOP_RC] < -rcControlSwitchDeadband) {
      rcFlags.RCStop = false;
    }
    if (abs(copiedRemoteInput[SPEED_RC]) < rcControlDeadband) {
      copiedRemoteInput[SPEED_RC] = 0;
    }
    if (abs(copiedRemoteInput[TURN_RC]) < rcControlDeadband) {
      copiedRemoteInput[TURN_RC] = 0;
    }
    if (rcFlags.RCOverride) {
      // RC overwrites
      speed = copiedRemoteInput[SPEED_RC] / 500.0;
      turn = copiedRemoteInput[TURN_RC] / 500.0;
    } else { // RCOverride switch is off
      if (RC_MODE == 1) { // RC adds to joystick and button inputs
        speed += copiedRemoteInput[SPEED_RC] / 500.0;
        turn += copiedRemoteInput[TURN_RC] / 500.0;
      } else if (RC_MODE == 2) {
        if (abs(speed) > 0.001 || abs(turn) > 0.001) { // local controls are activated...
          // so follow RC input
          speed = copiedRemoteInput[SPEED_RC] / 500.0;
          turn = copiedRemoteInput[TURN_RC] / 500.0;
        } else { // local controls are deactivated so don't move
          speed = 0;
          turn = 0;
        }
      } else {
        // else, override is off and mode=0 so don't affect local controls
      }
    }
    rcFlags.RC_make_motors_e_stop = rcFlags.RCStop;
  } else { // receiving invalid signal
    // if the no_stop_until_start setting is false, always turn off the car if the signal stops
    // if the no_stop_until_start setting is true, turn off the car only if the rc control has ever been activated
    rcFlags.RC_make_motors_e_stop = (NO_RC_STOP_UNTIL_START == false || rcFlags.everActivated == true);
    rcFlags.RCOverride = false;
  }
}
