/*
    This program is for controlling modified ride on cars for children who need different kinds of controls like joysticks.
    https://github.com/gobabygocarswithjoysticks/car-code
    Questions or comments? Please email gobabygocarswithjoysticks@gmail.com.
    Website that can upload and configure this code: https://gobabygocarswithjoysticks.github.io/programmer/

    This program has three types of functions that can be combined together to customize how the car drives.
        Input readers       -   get control inputs from the child
                InputReader_JoystickAxis      -   reads an axis of a joystick and scales the input (use two for a two axis joystick)
                InputReader_Buttons     -  reads a set of buttons and edits two values

        Input processors  -    limit speed, acceleration, and add other drive features

        Drive controllers   -  control motors of the car to make it go
                DriveController_TwoSideDrive     -   controls a car with two independent wheel motors
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// "CONSTANTS" (change to calibrate and customize a car for a child) ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///// joystick input reader constants /////
int CONTROL_RIGHT = 632;     // use to calibrate joystick (value from the X axis of the joystick when all the way to the left)
int CONTROL_CENTER_X = 509;  // use to calibrate joystick (value from the X axis of the joystick when it is centered)
int CONTROL_LEFT = 385;      // use to calibrate joystick (value from the X axis of the joystick when all the way to the right)
int X_DEADZONE = 25;         // radius around center where joystick is considered centered

int CONTROL_UP = 638;        // use to calibrate joystick (value from the Y axis of the joystick when all the way to the bottom)
int CONTROL_CENTER_Y = 511;  // use to calibrate joystick (value from the Y axis of the joystick when it is centered)
int CONTROL_DOWN = 380;      // use to calibrate joystick (value from the Y axis of the joystick when all the way to the top)
int Y_DEADZONE = 25;         // radius around center where joystick is considered centered

///// input processor constants /////
float ACCELERATION_FORWARD = 2.0;   //change # to change the amount of acceleration when going forward (1/#=seconds to reach max speed)
float DECELERATION_FORWARD = 1.5;   //change # to change the amount of deceleration when going forward (1/#=seconds to reach max speed)
float ACCELERATION_BACKWARD = 0.5;  //change # to change the amount of acceleration when going backward (1/#=seconds to reach max speed)
float DECELERATION_BACKWARD = 2.5;  //change # to change the amount of deceleration when going backward (1/#=seconds to reach max speed)

float ACCELERATION_TURNING = 1.0;  //acceleration of turning speed
float DECELERATION_TURNING = 2.0;  //deceleration of turning speed

float FASTEST_FORWARD = 0.7;   //change # to limit the forward speed [0.0-1.0]
float FASTEST_BACKWARD = 0.6;  //change # to limit the backward speed [0.0-1.0]
float TURN_SPEED = 0.5;        //change # to limit the turning speed (greater than 0)

float SCALE_TURNING_WHEN_MOVING = .5;  // what amount of TURN_SPEED to use when moving forward or backward (this adjusts what turn radius the car has when the joystick is pushed to a corner)

boolean SCALE_ACCEL_WITH_SPEED = true;  //set true if using a speed knob and you want to keep time to max speed constant instead of acceleration being constant

boolean REVERSE_TURN_IN_REVERSE = false;  //flip turn axis when backing up so that the car goes in the direction the joystick is pointed when in reverse

// calibrate signals to motor controllers
int LEFT_MOTOR_CENTER = 1500;
int LEFT_MOTOR_SLOW = 25;   // CENTER +- what? makes the motor start to turn
int LEFT_MOTOR_FAST = 500;  // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)
int RIGHT_MOTOR_CENTER = 1500;
int RIGHT_MOTOR_SLOW = 25;   // CENTER +- what? makes the motor start to turn
int RIGHT_MOTOR_FAST = 500;  // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)

boolean USE_SPEED_KNOB = false;  // true = use speed knob, false = don't read the speed knob (see FASTEST_FORWARD, FASTEST_BACKWARD and TURN_SPEED to limit speed)
int SPEED_KNOB_SLOW_VAL = 1060;  // can be slightly out of range, so that it just gets really slow instead of stopping
int SPEED_KNOB_FAST_VAL = 0;     //analogRead value when knob is turned fully towards "fast" setting

int PRINT_VARIABLES_INTERVAL_MILLIS = 100;  // or -1 makes it not print variables to the serial monitor


//////////////////////////////////////////// PINS /////////////////////////////////////
///// joystick reader pins /////
byte JOY_X_PIN = A2;  // Analog input pin that the left-right potentiometer is attached to
byte JOY_Y_PIN = A0;  // Analog input pin that the forwards-backwards potentiometer is attached to

///// drive controller pins /////
byte LEFT_MOTOR_CONTROLLER_PIN = 3;
byte RIGHT_MOTOR_CONTROLLER_PIN = 5;

byte SPEED_KNOB_PIN = A4;

//const byte buttonControlPin = 12;
//
///////////////////////////////////////////// BUTTON CONTROL /////////////////////////////////
//boolean BUTTON_CONTROL_PIN_ENABLE_STATE = LOW; // is the button control pin HIGH or LOW when button control should be enabled? change if you want to reverse the action of the button control switch
//// needed for button control settings
struct ButtonDriveConfig {
  byte pin;
  float speed;
  float turn;
};
//// button control settings
//const byte maxNumDriveButtons = 5;
//byte numDriveButtons = 5;
//ButtonDriveConfig driveButtons[maxNumDriveButtons] = {
//  //pin, speed, turn (there must be numDriveButtons number of rows)
//  {11, 0, 1}, //right
//  {10, .9, 1}, //RF
//  {9, 1, 0}, //forwards
//  {12, .9, -1}, //LF
//  {8, 0, -1} //left
//};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// END OF CONSTANTS SECTION //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////////////
//variables below this line aren't settings, don't change them.
///// joystick reader variables /////
int joyXVal = 0;  // value read from the joystick
int joyYVal = 0;  // value read from the joystick

///// variables set by input reader /////
float speedInput = 0;
float turnInput = 0;


///// variables to send to drive controller (sent through input processors)
float speedToDrive = 0;
float turnToDrive = 0;

float speedProcessed = 0;
float turnProcessed = 0;

int speedKnobVal = -1;  //raw value
float speedKnobScaler = 1;

///// servo signal values sent to motor controllers /////
int leftMotorWriteVal = LEFT_MOTOR_CENTER;
int rightMotorWriteVal = RIGHT_MOTOR_CENTER;

float timeInterval = 0;  // time in seconds (usually a small fraction of a second) between runs of loop(), used for limiting acceleration

unsigned long lastMillisPrintedValues = 0;

///// common types of motor controllers (ESCs) can be controlled with the Servo library /////
#include <Servo.h>  // https://www.arduino.cc/reference/en/libraries/servo/
Servo leftMotorController;
Servo rightMotorController;

//if the 0th eeprom value isn't this key, the hardcoded values are saved to EEPROM.
//new unprogrammed EEPROM defaults to 255, so this way the car will use the hardcoded values on first boot instead of unreasonable ones (all variables made from bytes of 255).
//change this key if you want changes to the hardcoded settings to be used. (don't use a value of 255)
const byte settings_memory_key = 5;
#include <EEPROM.h>

const int version_number = 1;  // for interaction with website

const boolean use_memory = true;  // recall and save settings from EEPROM, and allow for changing settings using the serial monitor.

boolean movementAllowed = true;

boolean joyOK = false;  // has the joystick input held steadily inside the deadzone? movement is disabled otherwise
long joystickCenterCounter = 0;

void setup() {
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, LOW);
  if (use_memory)
    settingsMemory();

  printSettings();  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)

  setupPins();

}
void setupPins() {
  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);
  pinMode(SPEED_KNOB_PIN, INPUT);

  ///// ESCs controlled with the Servo library need to be "attached" to the pin the ESC is wired to
  leftMotorController.attach(LEFT_MOTOR_CONTROLLER_PIN);
  rightMotorController.attach(RIGHT_MOTOR_CONTROLLER_PIN);

  leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);//tell the motor controller to not move
  rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);//tell the motor controller to not move
  delay(100);
}
void loop()
{
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
  turnInput = InputReader_JoystickAxis(joyXVal, CONTROL_LEFT, CONTROL_CENTER_X, CONTROL_RIGHT, X_DEADZONE);
  speedInput = InputReader_JoystickAxis(joyYVal, CONTROL_DOWN, CONTROL_CENTER_Y, CONTROL_UP, Y_DEADZONE);

  // TODO: buttons

  ////////////////////////////// PUT INPUT PROCESSORS HERE ///////////////////////
  /**
    float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scale, float ACCELERATION_FORWARD, float DECELERATION_FORWARD, float ACCELERATION_BACKWARD, float DECELERATION_BACKWARD, float timeInterval)
    float InputProcessor_LimitAccelerationTwoSettings(float velocity, float velocityTarget, float scale, float ACCELERATION, float DECELERATION, float timeInterval)
    float InputProcessor_LimitAccelerationOneSetting(float velocity, float velocityTarget, float scale, float ACCEL, float timeInterval)

    float InputProcessor_ReadSpeedKnob(byte SPEED_KNOB_PIN, int slowVal, int fastVal, int& raw)

    float InputProcessor_ScaleTurningWhenMoving(float stwm, float x, float y)

    void InputProcessor_ScaleInput(float speedKnobScaler, float &turnInput, float &speedInput, float FASTEST_FORWARD, float FASTEST_BACKWARD, float TURN_SPEED) // speedInput and turnInput are edited by this function

  */
  turnProcessed = turnInput;
  speedProcessed = speedInput;

  //reduce the turning when moving forward or backward. change SCALE_TURNING_WHEN_MOVING to change the turn radius when the joystick is pushed to a corner.
  turnProcessed = InputProcessor_ScaleTurningWhenMoving(SCALE_TURNING_WHEN_MOVING, turnProcessed, speedProcessed);

  /// scale based on speed knob and speed limits ///
  if (USE_SPEED_KNOB) {
    speedKnobScaler = InputProcessor_ReadKnob(SPEED_KNOB_PIN, SPEED_KNOB_SLOW_VAL, SPEED_KNOB_FAST_VAL, speedKnobVal);
  } else {
    speedKnobScaler = 1.0;
  }
  InputProcessor_ScaleInput(speedKnobScaler, turnProcessed, speedProcessed, FASTEST_FORWARD, FASTEST_BACKWARD, TURN_SPEED);

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
  ////////////////////////////// PUT THE DRIVE CONTROLLER HERE //////////////////////
  /**
    void DriveController_TwoSideDrive(float turnToDrive, float speedToDrive, int& leftMotorWrite, int& rightMotorWrite, int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST, int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST)

  */
  DriveController_TwoSideDrive(turnToDrive, speedToDrive, leftMotorWriteVal, rightMotorWriteVal, LEFT_MOTOR_CENTER, LEFT_MOTOR_SLOW, LEFT_MOTOR_FAST, RIGHT_MOTOR_CENTER, RIGHT_MOTOR_SLOW, RIGHT_MOTOR_FAST);

  bool delayedStartDone = millis() >= 3000;

  if (!joyOK || !delayedStartDone) {  // wait for joystick to become ok. Also wait for 3 seconds for the ESCs to calibrate
    leftMotorWriteVal = LEFT_MOTOR_CENTER;
    rightMotorWriteVal = RIGHT_MOTOR_CENTER;
    if (abs(turnInput) < 0.001 && abs(speedInput) < 0.001) {
      joystickCenterCounter++;
      if (joystickCenterCounter > 800) {  // joystick must be centered for this long
        joyOK = true;
      }
    } else {
      joystickCenterCounter = 0;
    }
  }

  if (startupPulse && joyOK && delayedStartDone) {
    startupPulse = false;
    if (movementAllowed) {  // don't pulse if the website says don't move
      leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER + 90);
      rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER + 90);
    }
    delay(150);  // pulse motors to indicate that the car is ready to drive (like classic code did in setup())
    if (movementAllowed) {
      leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);
      rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);
    }
  }

  if (movementAllowed) {
    leftMotorController.writeMicroseconds(leftMotorWriteVal);
    rightMotorController.writeMicroseconds(rightMotorWriteVal);
  }
  //////////////////////////////////////////////////////////////////////////////////

  if (printVariables(PRINT_VARIABLES_INTERVAL_MILLIS)) {
    // code added here runs right after the line of data is printed
    //    Serial.println(timeInterval, 5);
  }
  delay(1);
}  //end of loop()
