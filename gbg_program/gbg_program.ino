/*
   This program is for controlling modified ride on cars for children who need different kinds of controls like joysticks.
   https://github.com/gobabygocarswithjoysticks/car-code
   Questions or comments? Please email gobabygocarswithjoysticks@gmail.com or post here: https://github.com/gobabygocarswithjoysticks/car-code/discussions
   Website for uploading and configuring this code: https://gobabygocarswithjoysticks.github.io/programmer/

   THIS PROGRAM IS COMPILED BY A GITHUB ACTION. IT WON'T COMPILE PROPERLY IN THE ARDUINO IDE BECAUSE PRE-PROCESSING AND #DEFINES WILL BE MISSING.

   This program has three types of functions that can be combined together to customize how the car drives.
       Input readers       -   get control inputs from the child
               InputReader_JoystickAxis      -   reads an axis of a joystick and scales the input (use two for a two axis joystick)
               InputReader_Buttons     -  reads a set of buttons and edits two values

       Input processors  -    limit speed, acceleration, and add other drive features

       Drive controllers   -  control motors of the car to make it go
               DriveController_TwoSideDrive     -   controls a car with two independent wheel motors
*/
#include <Arduino.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// "CONSTANTS" (change to calibrate and customize a car for a child) ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///// joystick input reader constants /////
int16_t CONTROL_RIGHT = 60;      // use to calibrate joystick (value from the X axis of the joystick when all the way to the left)
int16_t CONTROL_CENTER_X = 542;  // use to calibrate joystick (value from the X axis of the joystick when it is centered)
int16_t CONTROL_LEFT = 987;      // use to calibrate joystick (value from the X axis of the joystick when all the way to the right)
int16_t X_DEADZONE = 50;         // radius around center where joystick is considered centered

int16_t CONTROL_UP = 936;        // use to calibrate joystick (value from the Y axis of the joystick when all the way to the bottom)
int16_t CONTROL_CENTER_Y = 455;  // use to calibrate joystick (value from the Y axis of the joystick when it is centered)
int16_t CONTROL_DOWN = 44;       // use to calibrate joystick (value from the Y axis of the joystick when all the way to the top)
int16_t Y_DEADZONE = 50;         // radius around center where joystick is considered centered

///// input processor constants /////
float ACCELERATION_FORWARD = 0.25;   //change # to change the amount of acceleration when going forward (1/#=seconds to reach max speed)
float DECELERATION_FORWARD = 3;      //change # to change the amount of deceleration when going forward (1/#=seconds to reach max speed)
float ACCELERATION_BACKWARD = 0.25;  //change # to change the amount of acceleration when going backward (1/#=seconds to reach max speed)
float DECELERATION_BACKWARD = 3;     //change # to change the amount of deceleration when going backward (1/#=seconds to reach max speed)

float ACCELERATION_TURNING = 1;  //acceleration of turning speed
float DECELERATION_TURNING = 3;  //deceleration of turning speed

float FASTEST_FORWARD = 0.2;    //change # to limit the forward speed [0.0-1.0]
float FASTEST_BACKWARD = 0.2;   //change # to limit the backward speed [0.0-1.0]
float TURN_SPEED = 0.15;        //change # to limit the turning speed (greater than 0)

float SCALE_TURNING_WHEN_MOVING = 1.0;  // what amount of TURN_SPEED to use when moving forward or backward (this adjusts what turn radius the car has when the joystick is pushed to a corner)

boolean SCALE_ACCEL_WITH_SPEED = true;  //set true if using a speed knob and you want to keep time to max speed constant instead of acceleration being constant

boolean REVERSE_TURN_IN_REVERSE = false;  //flip turn axis when backing up so that the car goes in the direction the joystick is pointed when in reverse

// calibrate signals to motor controllers
int16_t LEFT_MOTOR_CENTER = 1500;
int16_t LEFT_MOTOR_SLOW = 10;   // CENTER +- what? makes the motor start to turn
int16_t LEFT_MOTOR_FAST = 500;  // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)
int16_t RIGHT_MOTOR_CENTER = 1500;
int16_t RIGHT_MOTOR_SLOW = 10;   // CENTER +- what? makes the motor start to turn
int16_t RIGHT_MOTOR_FAST = 500;  // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)
#ifdef IS_PCB
int16_t LEFT_MOTOR_PULSE = 20; // CENTER +- (sign of _SLOW) what? makes the car move a bit for the pulse on start
int16_t RIGHT_MOTOR_PULSE = 20;  // CENTER +- (sign of _SLOW) what? makes the car move a bit for the pulse on start
#else
int16_t LEFT_MOTOR_PULSE = 20; // CENTER +- (sign of _SLOW) what? makes the car move a bit for the pulse on start
int16_t RIGHT_MOTOR_PULSE = 20;  // CENTER +- (sign of _SLOW) what? makes the car move a bit for the pulse on start
#endif
int16_t START_MOTOR_PULSE_TIME = 150; // milliseconds for pulse on start

boolean ENABLE_STARTUP_PULSE = true; // small movement to indicate that the car is ready

int16_t JOY_CALIB_COUNT = 800; // how long does joystick need to be centered? (units of somewhere between 1 and 2 milliseconds)

boolean USE_SPEED_KNOB = false;  // true = use speed knob, false = don't read the speed knob (see FASTEST_FORWARD, FASTEST_BACKWARD and TURN_SPEED to limit speed)
int16_t SPEED_KNOB_SLOW_VAL = 1060;  // can be slightly out of range, so that it just gets really slow instead of stopping
int16_t SPEED_KNOB_FAST_VAL = 0;     //analogRead value when knob is turned fully towards "fast" setting

#ifdef ESP32
#define LED_SETUP pinMode(2, OUTPUT);
#define LED_ON digitalWrite(2, HIGH);
#define LED_OFF digitalWrite(2, LOW);
#elif defined(IS_PCB)
#ifdef HAS_WIFI // pico 1W or 2W
#define LED_SETUP pinMode(LED_BUILTIN, OUTPUT); pinMode(13, OUTPUT);
#define LED_ON digitalWrite(LED_BUILTIN, HIGH); digitalWrite(13, HIGH);
#define LED_OFF digitalWrite(LED_BUILTIN, LOW); digitalWrite(13, LOW);
#else // pico 1 or 2
#define LED_SETUP pinMode(25, OUTPUT); pinMode(13, OUTPUT);
#define LED_ON digitalWrite(25, HIGH); digitalWrite(13, HIGH);
#define LED_OFF digitalWrite(25, LOW); digitalWrite(13, LOW);
#endif // end of pico

#elif defined(IS_PICO) // pico
#ifdef HAS_WIFI // pico 1W or 2W
#define LED_SETUP pinMode(LED_BUILTIN, OUTPUT);
#define LED_ON digitalWrite(LED_BUILTIN, HIGH);
#define LED_OFF digitalWrite(LED_BUILTIN, LOW);
#else // pico 1 or 2
#define LED_SETUP pinMode(25, OUTPUT);
#define LED_ON digitalWrite(25, HIGH);
#define LED_OFF digitalWrite(25, LOW);
#endif // end of pico

#else // nano or uno
#define LED_SETUP pinMode(LED_BUILTIN, OUTPUT);
#define LED_ON digitalWrite(LED_BUILTIN, HIGH);
#define LED_OFF digitalWrite(LED_BUILTIN, LOW);
#endif

//////////////////////////////////////////// PINS /////////////////////////////////////
#if defined(IS_PICO)
///// joystick reader pins /////
byte JOY_X_PIN = 26;  // Analog input pin that the left-right potentiometer is attached to
byte JOY_Y_PIN = 27;  // Analog input pin that the forwards-backwards potentiometer is attached to

///// drive controller pins /////
#ifdef IS_PCB
byte LEFT_MOTOR_1_PIN = 22;
byte LEFT_MOTOR_EN_PIN = 21;
byte LEFT_MOTOR_2_PIN = 20;
byte RIGHT_MOTOR_EN_PIN = 18;
byte RIGHT_MOTOR_1_PIN = 17;
byte RIGHT_MOTOR_2_PIN = 16;
boolean SWAP_MOTORS = false;
#define LEFT_MOTOR_CONTROLLER_PIN LEFT_MOTOR_EN_PIN, LEFT_MOTOR_1_PIN, LEFT_MOTOR_2_PIN
#define RIGHT_MOTOR_CONTROLLER_PIN RIGHT_MOTOR_EN_PIN, RIGHT_MOTOR_1_PIN, RIGHT_MOTOR_2_PIN
#else
byte LEFT_MOTOR_CONTROLLER_PIN = 19;
byte RIGHT_MOTOR_CONTROLLER_PIN = 21;
#endif

byte SPEED_KNOB_PIN = 28;

byte BUTTON_MODE_PIN = 8; // can turn button control mode on and off
byte STEERING_OFF_SWITCH_PIN = 7;

#elif defined(ESP32)
///// joystick reader pins /////
byte JOY_X_PIN = 39;  // Analog input pin that the left-right potentiometer is attached to
byte JOY_Y_PIN = 35;  // Analog input pin that the forwards-backwards potentiometer is attached to

///// drive controller pins /////
byte LEFT_MOTOR_CONTROLLER_PIN = 18;
byte RIGHT_MOTOR_CONTROLLER_PIN = 21;

byte SPEED_KNOB_PIN = 33;

byte BUTTON_MODE_PIN = 23; // can turn button control mode on and off
byte STEERING_OFF_SWITCH_PIN = 4;

#else // nano or uno
byte JOY_X_PIN = A4;  // Analog input pin that the left-right potentiometer is attached to
byte JOY_Y_PIN = A1;  // Analog input pin that the forwards-backwards potentiometer is attached to

///// drive controller pins /////
byte LEFT_MOTOR_CONTROLLER_PIN = 3;
byte RIGHT_MOTOR_CONTROLLER_PIN = 6;

byte SPEED_KNOB_PIN = A3;

byte BUTTON_MODE_PIN = 2; // can turn button control mode on and off
byte STEERING_OFF_SWITCH_PIN = 4;

#endif


///////////////////////////////////////////// BUTTON CONTROL /////////////////////////////////
//// needed for button control settings
struct ButtonDriveConfig {
  byte pin;
  float speed;
  float turn;
};
// button control settings
const byte maxNumDriveButtons = 6;
#if defined(ESP32)
boolean ENABLE_BUTTON_CTRL = false;
boolean USE_BUTTON_MODE_PIN = false;
boolean BUTTON_MODE_TOGGLE = false;
byte NUM_DRIVE_BUTTONS = 6;
ButtonDriveConfig driveButtons[maxNumDriveButtons] = {
  //pin, speed, turn (there must be maxNumDriveButtons number of rows)
  {16, 1, 0}, //forwards
  {17, 0, -1}, //left
  {19, 0, 1}, //right
  {26, 1, -1}, //LF
  {22, 1, 1}, //RF
  {25, -1, 0} //backwards
};
#elif defined(IS_PCB)
boolean ENABLE_BUTTON_CTRL = false;
boolean USE_BUTTON_MODE_PIN = false;
boolean BUTTON_MODE_TOGGLE = false;
byte NUM_DRIVE_BUTTONS = 4;
ButtonDriveConfig driveButtons[maxNumDriveButtons] = {
  //pin, speed, turn (there must be maxNumDriveButtons number of rows)
  {1, 1, 0}, //forwards
  {2, 0, -1}, //left
  {3, 0, 1}, //right
  {4, -1, 0}, //backwards
  {5, 1, 1}, //RF
  {6, 1, -1} //LF
};
#else
boolean ENABLE_BUTTON_CTRL = false;
boolean BUTTON_MODE_TOGGLE = false;
boolean USE_BUTTON_MODE_PIN = false;
byte NUM_DRIVE_BUTTONS = 6;
ButtonDriveConfig driveButtons[maxNumDriveButtons] = {
  //pin, speed, turn (there must be maxNumDriveButtons number of rows)
  {7, 1, 0}, //forwards
  {8, 0, -1}, //left
  {9, 0, 1}, //right
  {10, 1, -1}, //LF
  {11, 1, 1}, //RF
  {12, -1, 0} //backwards
};
#endif

boolean BUTTONS_ACTIVE_HIGH = false; // true = buttons are active high, false = buttons are active low

boolean STEERING_OFF_SWITCH = false;

enum {
  SPEED_RC,
  TURN_RC,
  CTRL_RC,
  STOP_RC,
  NUM_RC_INPUTS
};
boolean USE_RC_CONTROL = false;
#if defined(ESP32)
byte RC_PIN[NUM_RC_INPUTS] = {16, 17, 19, 22};
#else
byte RC_PIN[NUM_RC_INPUTS] = {7, 8, 9, 10};
#endif

boolean NO_RC_STOP_UNTIL_START = false;

boolean USE_STOP_SWITCH = false;
#if defined(ESP32)
byte STOP_PIN = 22;
#else
byte STOP_PIN = 5;
#endif

boolean STOP_PIN_HIGH = false;
boolean NO_STOP_UNTIL_START = false;

boolean USE_ON_OFF_BUTTONS = false;
#if defined(ESP32)
byte ON_BUTTON = 16;
byte OFF_BUTTON = 17;
#else
byte ON_BUTTON = 11; // pin for the on button
byte OFF_BUTTON = 12; // pin for the off button
#endif
boolean ON_OFF_BUTTONS_ACTIVE_HIGH = false;

#if defined(HAS_WIFI)
int8_t CAR_WIFI_NAME = 1;
int32_t CAR_WIFI_PASSWORD = 12345678;
boolean USE_WIFI = false;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// END OF CONSTANTS SECTION //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int PRINT_VARIABLES_INTERVAL_MILLIS = 100;  // or -1 makes it not print variables to the serial monitor

const int16_t rcControlSwitchDeadband = 100;

#if defined(HAS_WIFI)
boolean activatedByRemote = true;

boolean deactivateIfRemoteDisconnects = false;
byte remoteMode = 0; //0 is car, 1 is remote
float remoteFB = 0;
float remoteLR = 0;
unsigned long lastRemoteCommandMillis = 0;
#endif

//////////////////////////////////////////////////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////////////
//variables below this line aren't settings, don't change them.
///// joystick reader variables /////
int joyXVal;  // value read from the joystick
int joyYVal;  // value read from the joystick

///// variables set by input reader /////
float speedInput;
float turnInput;


///// variables to send to drive controller (sent through input processors)
float speedToDrive;
float turnToDrive;

float speedProcessed;
float turnProcessed;

int speedKnobVal;  //raw value
float speedKnobScaler;

///// servo signal values sent to motor controllers /////
int leftMotorWriteVal;
int rightMotorWriteVal;

float timeInterval;  // time in seconds (usually a small fraction of a second) between runs of loop(), used for limiting acceleration

unsigned long lastMillisPrintedValues;

///// common types of motor controllers (ESCs) can be controlled with the Servo library /////
#ifdef ESP32
// tiny Servo library for ESP32
#define SERVO_LEDC_RESOLUTION 32768 // 2^15
class Servo {
  protected:
    byte pin;
    boolean isAttached = false;
  public:
    Servo() {
      isAttached = false;
    }
    void attach(byte _pin) {
      isAttached = true;
      pin = _pin;
      ledcAttach(pin, 50, 15);//frequency, bits of resolution
    }
    void writeMicroseconds(int microseconds) {
      if (isAttached)
        ledcWrite(pin, microseconds * 1024 / 625); // microseconds * 50 / 1000000 * 2^15
    }
    void detach() {
      ledcDetach(pin);
      isAttached = false;
    }
    boolean attached() {
      return isAttached;
    }
};
#elif defined(IS_PCB)
// H-bridge motor control library that matches the Servo library
class Servo {
  protected:
    byte pinEn;
    byte pin1;
    byte pin2;
    boolean isAttached = false;
  public:
    Servo() {
      isAttached = false;
    }
    void attach(byte _pinEn, byte _pin1, byte _pin2) {
      isAttached = true;
      pinEn = _pinEn;
      pin1 = _pin1;
      pin2 = _pin2;
      pinMode(pinEn, OUTPUT);
      digitalWrite(pinEn, HIGH);
      pinMode(pin1, OUTPUT);
      analogWrite(pin1, 0);
      pinMode(pin2, OUTPUT);
      analogWrite(pin2, 0);
    }
    void writeMicroseconds(int microseconds) {
      if (isAttached) {
        microseconds = constrain(microseconds, 1000, 2000);
        if (microseconds > 1500) {
          analogWrite(pin2, 0);
          analogWrite(pin1, constrain((microseconds - 1500) * 255 / 500, 0, 255));
        } else if (microseconds < 1500) {
          analogWrite(pin1, 0);
          analogWrite(pin2, constrain((1500 - microseconds) * 255 / 500, 0, 255));
        } else {
          analogWrite(pin1, 0);
          analogWrite(pin2, 0);
        }
      }
    }
    void detach() {
      digitalWrite(pinEn, LOW);
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      pinMode(pinEn, INPUT);
      pinMode(pin1, INPUT);
      pinMode(pin2, INPUT);
      isAttached = false;
    }
    boolean attached() {
      return isAttached;
    }
};
#else
#include <Servo.h>  // https://www.arduino.cc/reference/en/libraries/servo/, used version 1.1.8
#endif

Servo leftMotorController;
Servo rightMotorController;

#include <EEPROM.h> // used version 2.0

#ifdef AVR
#include <avr/wdt.h>
void (*resetFunc)(void) = 0; // reboots the Arduino https://www.theengineeringprojects.com/2015/11/reset-arduino-programmatically.html
ISR(WDT_vect) // Watchdog timer interrupt.
{
  wdt_reset();
  resetFunc();
}
#endif

#ifdef IS_PCB
#if defined(HAS_WIFI)
const int version_number = 15;  // pcb with picoW or pico2W
const byte settings_memory_key = 15;
#else
const int version_number = 14;  // pcb with pico or pico2
const byte settings_memory_key = 14;
#endif
#else
#if defined(HAS_WIFI)
const int version_number = 19;  // esp32, picoW or pico2W
const byte settings_memory_key = 19;
#else // not pcb or wifi-capable, standard nano or uno or pico without wifi, but with capability for RC control (now part of the standard code)
// the version_number is used by the website to know how many settings to expect. This helps error-check the serial data.
const int version_number = 18;  // nano or uno
//if the 0th eeprom value isn't this key, the hardcoded values are saved to EEPROM.
const byte settings_memory_key = 18;
#endif
#endif

#define rcTimeoutMicros 2500000 // timeout for RC control
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
void (*RCISRs[])() = {
  speedRCISR,
  turnRCISR,
  ctrlRCISR,
  stopRCISR
};

void RCISR(byte whichRCInput) {
  if (digitalRead(RC_PIN[whichRCInput]) == HIGH) {
    lastRisingMicros[whichRCInput] = micros();
    anyRCRisingMillis = millis();
  } else if ((micros() - lastRisingMicros[whichRCInput]) <= rcTimeoutMicros) {
    unsigned long pulseTime = micros() - lastRisingMicros[whichRCInput];
    if(pulseTime > 500 && pulseTime < 2500) { // only accept pulses between 500 and 2500 microseconds
      remoteInput[whichRCInput] = constrain((int16_t)pulseTime - 1500, -500, 500); // convert to range -500 to 500
    }
  }
}

void setupRCControl() {
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
    if (rcFlags.RCOverride) {
      speed = copiedRemoteInput[SPEED_RC];
      turn = copiedRemoteInput[TURN_RC];
      if(abs(speed) < 50) {
        speed = 0;
      }
      if(abs(turn) < 50) {
        turn = 0;
      }
      speed = speed / 500.0;
      turn = turn / 500.0;
    }
    rcFlags.RC_make_motors_e_stop = rcFlags.RCStop;

  } else { // receiving invalid signal
    // if the no_stop_until_start setting is false, always turn off the car if the signal stops
    // if the no_stop_until_start setting is true, turn off the car only if the rc control has ever been activated
    rcFlags.RC_make_motors_e_stop = (NO_RC_STOP_UNTIL_START == false || rcFlags.everActivated == true);
    rcFlags.RCOverride = false;
  }
}

const boolean use_memory = true;  // recall and save settings from EEPROM, and allow for changing settings using the serial monitor.

boolean movementAllowed;

boolean joyOK;  // has the joystick input held steadily inside the deadzone? movement is disabled otherwise
long joystickCenterCounter;

boolean startupPulse;

void setup() {
  LED_SETUP;
  LED_ON;
#ifdef ESP32
  /*
    ESP32 sends data on start at 115200 from the bootloader (they can't be stopped from doing this).
    Switching to 250000 here was breaking the connection to the website.
    I can't make all cars use 115200 since that would break compatibility with old cars.
    This special case for esp32s (that the website has a checkbox for) is the best solution I could think of.
  */
  Serial.begin(115200);
#else
  Serial.begin(250000);
#endif
  Serial.println();
  delay(50);
  LED_OFF;

#ifdef AVR
  cli();
  wdt_reset();
  // set up Watchdog Timer
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (0 << WDE) |
           (0 << WDP3) | (1 << WDP2) | (1 << WDP1) |
           (1 << WDP0);
  sei();
#elif defined(IS_PICO)
  rp2040.wdt_begin(2000);
  rp2040.wdt_reset();
  EEPROM.begin(1024);
#elif defined(ESP32)
  EEPROM.begin(1024);
#endif

  //initialize variables
  joyXVal = 512;
  joyYVal = 512;
  speedInput = 0;
  turnInput = 0;
  speedToDrive = 0;
  turnToDrive = 0;
  speedProcessed = 0;
  turnProcessed = 0;
  speedKnobVal = -1;
  speedKnobScaler = 1;
  timeInterval = 0;
  lastMillisPrintedValues = 0;
  movementAllowed = true;
  joyOK = false;
  joystickCenterCounter = 0;

  if (use_memory)
    settingsMemory();

  leftMotorWriteVal = LEFT_MOTOR_CENTER;
  rightMotorWriteVal = RIGHT_MOTOR_CENTER;
  startupPulse = ENABLE_STARTUP_PULSE;

  delay(120);
  printSettings();  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
  delay(120);
  printSettings();  // do it again to increase the chance of a valid message going through

  setupPins();
#if defined(HAS_WIFI)
  setupWifi();
#endif

}
void setupPins() {
  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);
  pinMode(SPEED_KNOB_PIN, INPUT);

  if (ENABLE_BUTTON_CTRL) {
    if (USE_BUTTON_MODE_PIN) {
      pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
    }
    for (byte i = 0; i < NUM_DRIVE_BUTTONS; i++) {
      pinMode(driveButtons[i].pin, INPUT_PULLUP);
    }
  }

  if (STEERING_OFF_SWITCH) {
    pinMode(STEERING_OFF_SWITCH_PIN, INPUT_PULLUP);
  }

  if ( USE_RC_CONTROL) {
    setupRCControl();
  }

  if (USE_STOP_SWITCH) {
    pinMode(STOP_PIN, INPUT_PULLUP);
  }

  if (USE_ON_OFF_BUTTONS) {
    pinMode(ON_BUTTON, INPUT_PULLUP);
    pinMode(OFF_BUTTON, INPUT_PULLUP);
  }



#ifdef IS_PCB
  // PCB
  if (SWAP_MOTORS) {
    leftMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
    rightMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
  } else { // normal
    leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
    rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);
  }
  leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);//tell the motor controller to not move
  rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);//tell the motor controller to not move

#else
  ///// ESCs controlled with the Servo library need to be "attached" to the pin the ESC is wired to
  leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
  rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);

  leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);//tell the motor controller to not move
  rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);//tell the motor controller to not move
#endif
  delay(100);
}

void loop()
{
#ifdef AVR
  wdt_reset();
#elif defined(IS_PICO)
  rp2040.wdt_reset();
#endif

#if defined(HAS_WIFI)
  runWifi();
#endif

  if (use_memory)
    settingsSerial();

  timeInterval = calculateTimeInterval();
  ////////////////////////////// PUT INPUT READERS HERE /////////////////////////
  /**
    InputReader_JoystickAxis(value, left/down, center, right/up, deadzone)
    InputReader_Buttons(boolean enable, boolean resetValues, byte numDriveButtons, ButtonDriveConfig driveButtons[], float& turnInput, float& speedInput, boolean buttonState) // speedInput and turnInput are edited by this function
  */
  joyXVal = analogRead(JOY_X_PIN);
  joyYVal = analogRead(JOY_Y_PIN);
#ifdef ESP32
  joyXVal /= 4;
  joyYVal /= 4;
#endif
  turnInput = InputReader_JoystickAxis(joyXVal, CONTROL_LEFT, CONTROL_CENTER_X, CONTROL_RIGHT, X_DEADZONE);
  speedInput = InputReader_JoystickAxis(joyYVal, CONTROL_DOWN, CONTROL_CENTER_Y, CONTROL_UP, Y_DEADZONE);


  if (ENABLE_BUTTON_CTRL) {
    if (BUTTON_MODE_TOGGLE) {
      boolean buttonModePinState = digitalRead(BUTTON_MODE_PIN);
      if (buttonModePinState == BUTTONS_ACTIVE_HIGH && rcFlags.lastButtonModePinState == !BUTTONS_ACTIVE_HIGH) {
        buttonModeActive = !buttonModeActive; // toggle button mode
      }
      rcFlags.lastButtonModePinState = buttonModePinState;
    } else {
      buttonModeActive = !USE_BUTTON_MODE_PIN || (digitalRead(BUTTON_MODE_PIN) == BUTTONS_ACTIVE_HIGH);
    }
    InputReader_Buttons(buttonModeActive, true, NUM_DRIVE_BUTTONS, driveButtons, turnInput, speedInput, BUTTONS_ACTIVE_HIGH);
  } else {
    buttonModeActive = false;
  }

  if (joyOK) {
    runRCInput(speedInput, turnInput); // variables are passed as references, so the function can edit the values
  }else{
    rcFlags.RCOverride = false;
  }

#if defined(HAS_WIFI)
  if (joyOK) {
    runWifiInput(speedInput, turnInput); // references, so the function can edit the values
  }
#endif


  ////////////////////////////// PUT INPUT PROCESSORS HERE ///////////////////////
  /**
    float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scale, float ACCELERATION_FORWARD, float DECELERATION_FORWARD, float ACCELERATION_BACKWARD, float DECELERATION_BACKWARD, float timeInterval)
    float InputProcessor_LimitAccelerationTwoSettings(float velocity, float velocityTarget, float scale, float ACCELERATION, float DECELERATION, float timeInterval)
    float InputProcessor_LimitAccelerationOneSetting(float velocity, float velocityTarget, float scale, float ACCEL, float timeInterval)

    float InputProcessor_ReadSpeedKnob(byte SPEED_KNOB_PIN, int slowVal, int fastVal, int& raw)

    float InputProcessor_ScaleTurningWhenMoving(float stwm, float x, float y)

    void InputProcessor_ScaleInput(float speedKnobScaler, float &turnInput, float &speedInput, float FASTEST_FORWARD, float FASTEST_BACKWARD, float TURN_SPEED) // speedInput and turnInput are edited by this function

  */
#if defined(HAS_WIFI)
  if (activatedByRemote || !USE_WIFI) {
#endif
    turnProcessed = turnInput;
    speedProcessed = speedInput;
#if defined(HAS_WIFI) //using wifi and not activated by remote
  } else {
    turnProcessed = 0; // soft stop, not e stop - website connection is less reliable than RC control
    speedProcessed = 0;
  }
#endif

  if (STEERING_OFF_SWITCH) {
    if (digitalRead(STEERING_OFF_SWITCH_PIN) == LOW) {
      turnProcessed = 0;
    }
  }

  /// scale based on speed knob and speed limits ///
  if (USE_SPEED_KNOB) {
    speedKnobScaler = InputProcessor_ReadKnob(SPEED_KNOB_PIN, SPEED_KNOB_SLOW_VAL, SPEED_KNOB_FAST_VAL, speedKnobVal);
  } else {
    speedKnobScaler = 1.0;
  }
  InputProcessor_ScaleInput(speedKnobScaler, turnProcessed, speedProcessed, FASTEST_FORWARD, FASTEST_BACKWARD, TURN_SPEED);

  //reduce the turning when moving forward or backward. change SCALE_TURNING_WHEN_MOVING to change the turn radius when the joystick is pushed to a corner.
  turnProcessed = InputProcessor_ScaleTurningWhenMoving(SCALE_TURNING_WHEN_MOVING, turnProcessed, speedProcessed, speedProcessed >= 0 ? FASTEST_FORWARD : FASTEST_BACKWARD);
  turnProcessed = constrain(turnProcessed, -1, 1);

  // option to make the car go in the direction the joystick is pointed when in reverse
  // if the joystick is pulled to the back right corner, the front can turn towards the left (the reverse of normal for when the joystick is to the right) so the back of the car steers to the right
  if (speedProcessed < 0 && REVERSE_TURN_IN_REVERSE) {
    turnProcessed = -turnProcessed;
  }

  speedToDrive = InputProcessor_LimitAccelerationFourSettings(speedToDrive, speedProcessed, (SCALE_ACCEL_WITH_SPEED ? speedKnobScaler : 1.0), ACCELERATION_FORWARD, DECELERATION_FORWARD, ACCELERATION_BACKWARD, DECELERATION_BACKWARD, timeInterval);
  turnToDrive = InputProcessor_LimitAccelerationTwoSettings(turnToDrive, turnProcessed, (SCALE_ACCEL_WITH_SPEED ? speedKnobScaler : 1.0), ACCELERATION_TURNING, DECELERATION_TURNING, timeInterval);
  if (USE_SPEED_KNOB) {
    // force the speed to be below the speedKnobScaler (if turning the knob to slow, acceleration doesn't matter, it slows down right away)
    speedToDrive = constrain(speedToDrive, -speedKnobScaler, speedKnobScaler);
    turnToDrive = constrain(turnToDrive, -speedKnobScaler, speedKnobScaler);
  }

  if (USE_ON_OFF_BUTTONS) {
    if (digitalRead(ON_BUTTON) == (ON_OFF_BUTTONS_ACTIVE_HIGH ? HIGH : LOW)) {
      rcFlags.Start_Stop_Buttons_e_stop = false;
    }
    if (digitalRead(OFF_BUTTON) == (ON_OFF_BUTTONS_ACTIVE_HIGH ? HIGH : LOW)) {
      rcFlags.Start_Stop_Buttons_e_stop = true;
    }
  }else{
    rcFlags.Start_Stop_Buttons_e_stop = false; // no e-stop from buttons
  }

  if (USE_STOP_SWITCH) {
    if (digitalRead(STOP_PIN) == (STOP_PIN_HIGH ? HIGH : LOW)) {
      if (NO_STOP_UNTIL_START == false || rcFlags.Start_Switch_Ever_Activated) {
        speedToDrive = 0;
        turnToDrive = 0;
      }
    } else { // switch is saying "go"
      rcFlags.Start_Switch_Ever_Activated = true; // switch has activated the car
    }
  }

  if (rcFlags.RC_make_motors_e_stop) {
    speedToDrive = 0;
    turnToDrive = 0;
  }

  if (rcFlags.Start_Stop_Buttons_e_stop) {
    speedToDrive = 0;
    turnToDrive = 0;
  }

  if(joyOK){
    if (abs(turnToDrive) >= 0.001 || abs(speedToDrive) >= 0.001) {
      LED_ON;
    } else {
      LED_OFF;
    }
  }else{ // joystick not calibrated yet
    if ((millis() % 150) < 75) {
      LED_ON;
    } else {
      LED_OFF;
    }
  }

  ////////////////////////////// PUT THE DRIVE CONTROLLER HERE //////////////////////
  /**
    void DriveController_TwoSideDrive(float turnToDrive, float speedToDrive, int& leftMotorWrite, int& rightMotorWrite, int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST, int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST)

  */
  DriveController_TwoSideDrive(turnToDrive, speedToDrive, leftMotorWriteVal, rightMotorWriteVal, LEFT_MOTOR_CENTER, LEFT_MOTOR_SLOW, LEFT_MOTOR_FAST, RIGHT_MOTOR_CENTER, RIGHT_MOTOR_SLOW, RIGHT_MOTOR_FAST);

  bool delayedStartDone = millis() >= 3000;

  if (!joyOK || !delayedStartDone) {  // wait for joystick to become ok. Also wait for 3 seconds for the ESCs to calibrate
    leftMotorWriteVal = LEFT_MOTOR_CENTER;
    rightMotorWriteVal = RIGHT_MOTOR_CENTER;
    if (JOY_CALIB_COUNT <= 0) {
      joyOK = true;
    }
    if (abs(turnInput) < 0.001 && abs(speedInput) < 0.001) {
      joystickCenterCounter++;
      if (joystickCenterCounter > JOY_CALIB_COUNT) {  // joystick must be centered for this long
        joyOK = true;
      }
    } else {
      joystickCenterCounter = 0;
    }
  }

  if (startupPulse && joyOK && delayedStartDone) {
    startupPulse = false;
    if (movementAllowed) {  // don't pulse if the website says don't move
      delay(10);
      leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER + LEFT_MOTOR_PULSE * (LEFT_MOTOR_SLOW > 0 ? 1 : -1));
      rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER + RIGHT_MOTOR_PULSE * (RIGHT_MOTOR_SLOW > 0 ? 1 : -1));
    }
    delay(START_MOTOR_PULSE_TIME);  // pulse motors to indicate that the car is ready to drive (like classic code did in setup())
    if (movementAllowed) {
      leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
      rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
    }
  } else {
    if (movementAllowed) {
      leftMotorController.writeMicroseconds(leftMotorWriteVal);
      rightMotorController.writeMicroseconds(rightMotorWriteVal);
    } else {
      leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
      rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
    }
  }
  //////////////////////////////////////////////////////////////////////////////////

  if (printVariables(PRINT_VARIABLES_INTERVAL_MILLIS)) {
    // code added here runs right after the line of data is printed
    //    Serial.println(timeInterval, 5);
  }
  delay(1);
}  //end of loop()
