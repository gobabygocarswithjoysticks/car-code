/**
  Given a speed and turning value, this function edits the values to be sent to the motors.
  This function scales the input for the motor controllers if it is given a value for what makes the motor controller stop, when it starts moving, and when it moves at its max speed.
*/
void DriveController_TwoSideDrive(float turnToDrive, float speedToDrive, int& leftMotorWrite, int& rightMotorWrite,
                                  int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST,
                                  int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST) {
  leftMotorWrite = twoMap(turnToDrive + speedToDrive, -1, 0, 1, LEFT_MOTOR_CENTER - LEFT_MOTOR_FAST, LEFT_MOTOR_CENTER - LEFT_MOTOR_SLOW, LEFT_MOTOR_CENTER + LEFT_MOTOR_SLOW, LEFT_MOTOR_CENTER + LEFT_MOTOR_FAST, LEFT_MOTOR_CENTER);
  leftMotorWrite = constrain(leftMotorWrite, LEFT_MOTOR_CENTER - _abs(LEFT_MOTOR_FAST), LEFT_MOTOR_CENTER + _abs(LEFT_MOTOR_FAST));

  rightMotorWrite = twoMap(-turnToDrive + speedToDrive, -1, 0, 1, RIGHT_MOTOR_CENTER - RIGHT_MOTOR_FAST, RIGHT_MOTOR_CENTER - RIGHT_MOTOR_SLOW, RIGHT_MOTOR_CENTER + RIGHT_MOTOR_SLOW, RIGHT_MOTOR_CENTER + RIGHT_MOTOR_FAST, RIGHT_MOTOR_CENTER);
  rightMotorWrite = constrain(rightMotorWrite, RIGHT_MOTOR_CENTER - _abs(RIGHT_MOTOR_FAST), RIGHT_MOTOR_CENTER + _abs(RIGHT_MOTOR_FAST));
}
