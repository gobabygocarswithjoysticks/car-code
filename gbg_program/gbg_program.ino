
/*
  Wild Things Project
  Blythedale Children's Hospital
    Authors:
      Andres Guerrero
      Joshua Phelps, 2022 summer volunteer (joshuaphelps127@gmail.com)

    This program reads a joystick and uses two motor drivers to control left and right wheels.

  Version 2:
    Now with button control!
    I copied a lot of this code from the jeep's new program.
    All the calculations have been changed to use float variables instead of raw joystick and motor controller values. The program can still run quickly enough and I think it's easier to work with.

*/

/*
    This program is for controlling modified ride on cars for children who need special kinds of controls like joysticks.
    This program has three types of functions that can be combined together to customize how the car drives.
        Input readers       -   get control inputs from the child
                InputReader_JoystickAxis      -   reads an axis of a joystick and scales the input (use two for a two axis joystick)
                InputReader_Buttons     -  reads a set of buttons and edits two values

        Input processors  -    limit speed, acceleration, and add other drive features

        Drive controllers   -  control motors of the car to make it go
                DriveController_TwoSideDrive     -   controls a car with two independent wheel motors
*/

//////////////////////////////////////// CONSTANTS (change to calibrate and customize a car for a child) ///////////////////////////////////////////////
///// joystick input reader constants /////
int CONTROL_RIGHT = 1023; // use to calibrate joystick (value from the X axis of the joystick when all the way to the left)
int CONTROL_CENTER_X = 502; // use to calibrate joystick (value from the X axis of the joystick when it is centered)
int CONTROL_LEFT = 0; // use to calibrate joystick (value from the X axis of the joystick when all the way to the right)
int X_DEADZONE = 25; // radius around center where joystick is considered centered

int CONTROL_UP = 0; // use to calibrate joystick (value from the Y axis of the joystick when all the way to the bottom)
int CONTROL_CENTER_Y = 590; // use to calibrate joystick (value from the Y axis of the joystick when it is centered)
int CONTROL_DOWN = 1023; // use to calibrate joystick (value from the Y axis of the joystick when all the way to the top)
int Y_DEADZONE = 150; // radius around center where joystick is considered centered

///// input processor constants /////
float ACCELERATION_FORWARD  = 9.8;      //change # to change the amount of acceleration when going forward (1/#=seconds to reach max speed)
float DECELERATION_FORWARD  = 1.5;    //change # to change the amount of deceleration when going forward (1/#=seconds to reach max speed)
float ACCELERATION_BACKWARD = 0.5;      //change # to change the amount of acceleration when going backward (1/#=seconds to reach max speed)
float DECELERATION_BACKWARD = 2.5;    //change # to change the amount of deceleration when going backward (1/#=seconds to reach max speed)

float ACCELERATION_TURNING = 1.0; //acceleration of turning speed
float DECELERATION_TURNING = 2.0; //deceleration of turning speed

float FASTEST_FORWARD = 0.45; //change # to limit the forward speed [0.0-1.0]
float FASTEST_BACKWARD = 0.4; //change # to limit the backward speed [0.0-1.0]
float TURN_SPEED = 0.3; //change # to limit the turning speed (greater than 0)

float SCALE_TURNING_WHEN_MOVING = .5; // what amount of TURN_SPEED to use when moving forward or backward (this adjusts what turn radius the car has when the joystick is pushed to a corner)

boolean scaleAccelWithSpeed = true; //set true if using a speed knob and you want to keep time to max speed constant instead of acceleration being constant

boolean reverseTurnInReverse = true; //flip turn axis when backing up so that the car goes in the direction the joystick is pointed when in reverse

// calibrate signals to motor controllers
int LEFT_MOTOR_CENTER = 1500;
int LEFT_MOTOR_SLOW = 65; // CENTER +- what? makes the motor start to turn
int LEFT_MOTOR_FAST = 500; // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)
int RIGHT_MOTOR_CENTER = 1500;
int RIGHT_MOTOR_SLOW = 65; // CENTER +- what? makes the motor start to turn
int RIGHT_MOTOR_FAST = 500; // CENTER +- what? makes the motor go at full speed (if you want to limit the max speed, use FASTEST_FORWARD AND FASTEST_BACKWARD)

int speedKnobSlowVal = 1060; // can be slightly out of range, so that it just gets really slow instead of stopping
int speedKnobFastVal = 0; //analogRead value when knob is turned fully towards "fast" setting

int printVariablesIntervalMillis = 100; // or -1 makes it not print variables to the serial monitor

//////////////////////////////////////////// PINS /////////////////////////////////////
///// joystick reader pins /////
const byte joyXPin = A0; // Analog input pin that the left-right potentiometer is attached to
const byte joyYPin = A1; // Analog input pin that the forwards-backwards potentiometer is attached to

///// drive controller pins /////
const byte leftMotorControllerPin = 5;
const byte rightMotorControllerPin = 6;

const byte speedKnobPin = A2;
const byte buttonControlPin = 7;

/////////////////////////////////////////// BUTTON CONTROL /////////////////////////////////
boolean BUTTON_CONTROL_PIN_ENABLE_STATE = LOW; // is the button control pin HIGH or LOW when button control should be enabled? change if you want to reverse the action of the button control switch
// needed for button control settings
struct ButtonDriveConfig {
  byte pin;
  float speed;
  float turn;
};
// button control settings

const byte numDriveButtons = 5;
ButtonDriveConfig driveButtons[numDriveButtons] = {
  //pin, speed, turn (there must be numDriveButtons number of rows)
  {11, 0, 1}, //right
  {10, .9, 1}, //RF
  {9, 1, 0}, //forwards
  {12, .9, -1}, //LF
  {8, 0, -1} //left
};

//////////////////////////////////////////////////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////////////
//variables below this line aren't settings, don't change them.
///// joystick reader variables /////
int joyXVal = 0; // value read from the joystick
int joyYVal = 0; // value read from the joystick

///// variables set by input reader /////
float speedInput = 0;
float turnInput = 0;


///// variables to send to drive controller (sent through input processors)
float speedToDrive = 0;
float turnToDrive = 0;

float speedProcessed = 0;
float turnProcessed = 0;

float speedKnobScaler = 1;


///// servo signal values sent to motor controllers /////
int leftMotorWriteVal = LEFT_MOTOR_CENTER;
int rightMotorWriteVal = RIGHT_MOTOR_CENTER;

float timeInterval = 0; // time in seconds (usually a small fraction of a second) between runs of loop(), used for limiting acceleration

unsigned long lastMillisPrintedValues = 0;

///// common types of motor controllers (ESCs) can be controlled with the Servo library /////
#include <Servo.h>  // https://www.arduino.cc/reference/en/libraries/servo/
Servo leftMotorController;
Servo rightMotorController;


void setup()
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); // NOTE: this is a "hack" to make an extra ground pin to use for the button enable switch

  Serial.begin(115200);
  /////////////////////////////////////////////////////// SET UP INPUTS AND OUTPUTS /////////////////////////////
  ///// joystick and knob pins should be set to the INPUT pinMode, switches and buttons should be set to INPUT_PULLUP
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  pinMode(buttonControlPin, INPUT_PULLUP);
  pinMode(speedKnobPin, INPUT);

  for (byte i = 0; i < numDriveButtons; i++) {
    pinMode(driveButtons[i].pin, INPUT_PULLUP);
  }

  ///// ESCs controlled with the Servo library need to be "attached" to the pin the ESC is wired to
  leftMotorController.attach(leftMotorControllerPin);
  rightMotorController.attach(rightMotorControllerPin);


  leftMotorController.writeMicroseconds(LEFT_MOTOR_CENTER);//tell the motor controller to not move
  rightMotorController.writeMicroseconds(RIGHT_MOTOR_CENTER);//tell the motor controller to not move

  printSettings();  ///// print settings and any other info ///// (useful for if you don't have a record of the settings on a car)
}

void loop()
{
  timeInterval = calculateTimeInterval();
  ////////////////////////////// PUT INPUT READERS HERE /////////////////////////
  /**
    InputReader_JoystickAxis(value, left/down, center, right/up, deadzone)
    InputReader_Buttons(boolean enable, boolean resetValues, byte numDriveButtons, ButtonDriveConfig driveButtons[], float& turnInput, float& speedInput, boolean buttonState) // speedInput and turnInput are edited by this function
  */
  joyXVal = analogRead(joyXPin);
  joyYVal = analogRead(joyYPin);
  turnInput  = InputReader_JoystickAxis(joyXVal, CONTROL_LEFT, CONTROL_CENTER_X, CONTROL_RIGHT, X_DEADZONE);
  speedInput = InputReader_JoystickAxis(joyYVal, CONTROL_DOWN, CONTROL_CENTER_Y, CONTROL_UP, Y_DEADZONE);

  InputReader_Buttons((digitalRead(buttonControlPin) == BUTTON_CONTROL_PIN_ENABLE_STATE), true, numDriveButtons, driveButtons, turnInput, speedInput, LOW);


  ////////////////////////////// PUT INPUT PROCESSORS HERE ///////////////////////
  /**
    float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scale, float ACCELERATION_FORWARD, float DECELERATION_FORWARD, float ACCELERATION_BACKWARD, float DECELERATION_BACKWARD, float timeInterval)
    float InputProcessor_LimitAccelerationTwoSettings(float velocity, float velocityTarget, float scale, float ACCELERATION, float DECELERATION, float timeInterval)
    float InputProcessor_LimitAccelerationOneSetting(float velocity, float velocityTarget, float scale, float ACCEL, float timeInterval)

    float InputProcessor_ReadSpeedKnob(byte speedKnobPin, int slowVal, int fastVal)

    float InputProcessor_ScaleTurningWhenMoving(float stwm, float x, float y)

    void InputProcessor_ScaleInput(float speedKnobScaler, float &turnInput, float &speedInput, float FASTEST_FORWARD, float FASTEST_BACKWARD, float TURN_SPEED) // speedInput and turnInput are edited by this function

  */
  turnProcessed = turnInput;
  speedProcessed = speedInput;

  //reduce the turning when moving forward or backward. change SCALE_TURNING_WHEN_MOVING to change the turn radius when the joystick is pushed to a corner.
  turnProcessed = InputProcessor_ScaleTurningWhenMoving(SCALE_TURNING_WHEN_MOVING, turnProcessed, speedProcessed);

  /// scale based on speed knob and speed limits ///
  speedKnobScaler = InputProcessor_ReadKnob(speedKnobPin, speedKnobSlowVal, speedKnobFastVal);
  InputProcessor_ScaleInput(speedKnobScaler, turnProcessed, speedProcessed, FASTEST_FORWARD, FASTEST_BACKWARD, TURN_SPEED);

  // option to make the car go in the direction the joystick is pointed when in reverse
  // if the joystick is pulled to the back right corner, the front can turn towards the left (the reverse of normal for when the joystick is to the right) so the back of the car steers to the right
  if (speedProcessed < 0 && reverseTurnInReverse) {
    turnProcessed = -turnProcessed;
  }

  speedToDrive = InputProcessor_LimitAccelerationFourSettings(speedToDrive, speedProcessed, (scaleAccelWithSpeed ? speedKnobScaler : 1.0) , ACCELERATION_FORWARD, DECELERATION_FORWARD, ACCELERATION_BACKWARD, DECELERATION_BACKWARD, timeInterval);
  turnToDrive = InputProcessor_LimitAccelerationTwoSettings(turnToDrive, turnProcessed, speedKnobScaler, ACCELERATION_TURNING, DECELERATION_TURNING, timeInterval);

  ////////////////////////////// PUT THE DRIVE CONTROLLER HERE //////////////////////
  /**
    void DriveController_TwoSideDrive(float turnToDrive, float speedToDrive, int& leftMotorWrite, int& rightMotorWrite, int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST, int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST)

  */
  DriveController_TwoSideDrive(turnToDrive, speedToDrive, leftMotorWriteVal, rightMotorWriteVal, LEFT_MOTOR_CENTER, LEFT_MOTOR_SLOW, LEFT_MOTOR_FAST, RIGHT_MOTOR_CENTER, RIGHT_MOTOR_SLOW, RIGHT_MOTOR_FAST);
  leftMotorController.writeMicroseconds(leftMotorWriteVal);
  rightMotorController.writeMicroseconds(rightMotorWriteVal);

  //////////////////////////////////////////////////////////////////////////////////

  if (  printVariables(printVariablesIntervalMillis)) {
    // code added here runs right after the line of data is printed
    //    Serial.println(timeInterval, 5);
  }

} //end of loop()
