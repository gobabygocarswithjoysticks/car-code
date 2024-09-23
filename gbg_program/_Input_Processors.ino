// returns a value between 0 and 1 after reading a knob
float InputProcessor_ReadKnob(byte SPEED_KNOB_PIN, int slowVal, int fastVal, int& raw) {
  raw = analogRead(SPEED_KNOB_PIN);
  return constrain(floatMap(raw, slowVal, fastVal, 0, 1), 0, 1);
}

void InputProcessor_ScaleInput(float speedKnobScaler, float &turnInput, float &speedInput, float FASTEST_FORWARD, float FASTEST_BACKWARD, float TURN_SPEED) {
  FASTEST_BACKWARD = abs(FASTEST_BACKWARD);
  FASTEST_FORWARD = abs(FASTEST_FORWARD);
  TURN_SPEED = abs(TURN_SPEED);
  speedInput = twoMap(speedInput, -1, 0, 1, /**/ -FASTEST_BACKWARD * speedKnobScaler, 0, 0, FASTEST_FORWARD * speedKnobScaler, 0);
  speedInput = constrain(speedInput, -FASTEST_BACKWARD * speedKnobScaler, FASTEST_FORWARD * speedKnobScaler);

  turnInput = constrain(turnInput * TURN_SPEED * speedKnobScaler, -TURN_SPEED * speedKnobScaler, TURN_SPEED * speedKnobScaler);
}

/**
  /////  limit change in speed for gentler movements
              multipliying all accleration and deceleration values by scaler keeps time to max speed constant instead of acceleration being constant if a speed knob is being used to scale speed
*/

float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scaler, float ACCELERATION_FORWARD, float DECELERATION_FORWARD, float ACCELERATION_BACKWARD, float DECELERATION_BACKWARD, float timeInterval) {
  ACCELERATION_FORWARD = max(ACCELERATION_FORWARD, (float)0.0) * scaler;
  DECELERATION_FORWARD = max(DECELERATION_FORWARD, (float)0.0) * scaler;
  ACCELERATION_BACKWARD = max(ACCELERATION_BACKWARD, (float)0.0) * scaler;
  DECELERATION_BACKWARD = max(DECELERATION_BACKWARD, (float)0.0) * scaler;
  if (velocity == 0) {
    velocity += constrain(velocityTarget - velocity, -ACCELERATION_BACKWARD * timeInterval, ACCELERATION_FORWARD * timeInterval);
  } else if (velocity > 0) {
    velocity += constrain(velocityTarget - velocity, -DECELERATION_FORWARD * timeInterval, ACCELERATION_FORWARD * timeInterval);
    if (velocity < 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  } else {  //velocity < 0
    velocity += constrain(velocityTarget - velocity, -ACCELERATION_BACKWARD * timeInterval, DECELERATION_BACKWARD * timeInterval);
    if (velocity > 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  }
  return velocity;
}
float InputProcessor_LimitAccelerationTwoSettings(float velocity, float velocityTarget, float scaler, float ACCELERATION, float DECELERATION, float timeInterval) {
  ACCELERATION = max(ACCELERATION, (float)0.0) * scaler;
  DECELERATION = max(DECELERATION, (float)0.0) * scaler;
  if (velocity == 0) {
    velocity += constrain(velocityTarget - velocity, -ACCELERATION * timeInterval, ACCELERATION * timeInterval);
  } else if (velocity > 0) {
    velocity += constrain(velocityTarget - velocity, -DECELERATION * timeInterval, ACCELERATION * timeInterval);
    if (velocity < 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  } else {  //velocity < 0
    velocity += constrain(velocityTarget - velocity, -ACCELERATION * timeInterval, DECELERATION * timeInterval);
    if (velocity > 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  }
  return velocity;
}

float InputProcessor_LimitAccelerationOneSetting(float velocity, float velocityTarget, float scaler, float CELERATION, float timeInterval) {
  CELERATION = max(CELERATION, (float)0.0) * scaler;
  velocity += constrain(velocityTarget - velocity, -CELERATION * timeInterval, CELERATION * timeInterval);
  return velocity;
}

float InputProcessor_ScaleTurningWhenMoving(float stwm, float x, float y, float maxy) {
  return x * max(1.0 - (1.0 - stwm) * abs(y) / (abs(maxy) + 0.0001), 0.0);
}
