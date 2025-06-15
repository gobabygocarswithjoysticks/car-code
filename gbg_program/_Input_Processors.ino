// returns a value between 0 and 1 after reading a knob
float InputProcessor_ReadKnob(byte _SPEED_KNOB_PIN, int slowVal, int fastVal, int& raw) {
  raw = analogRead(_SPEED_KNOB_PIN);
  return constrain(floatMap(raw, slowVal, fastVal, 0, 1), 0, 1);
}

void InputProcessor_ScaleInput(float speedKnobScaler, float &turnInput, float &speedInput, float _FASTEST_FORWARD, float _FASTEST_BACKWARD, float TURN_SPEED) {
  _FASTEST_BACKWARD = abs(_FASTEST_BACKWARD);
  _FASTEST_FORWARD = abs(_FASTEST_FORWARD);
  TURN_SPEED = abs(TURN_SPEED);
  speedInput = twoMap(speedInput, -1, 0, 1, /**/ -_FASTEST_BACKWARD * speedKnobScaler, 0, 0, _FASTEST_FORWARD * speedKnobScaler, 0);
  speedInput = constrain(speedInput, -_FASTEST_BACKWARD * speedKnobScaler, _FASTEST_FORWARD * speedKnobScaler);

  turnInput = constrain(turnInput * TURN_SPEED * speedKnobScaler, -TURN_SPEED * speedKnobScaler, TURN_SPEED * speedKnobScaler);
}

/**
  /////  limit change in speed for gentler movements
              multipliying all accleration and deceleration values by scaler keeps time to max speed constant instead of acceleration being constant if a speed knob is being used to scale speed
*/

float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scaler, float ACCELERATION__FORWARD, float DECELERATION__FORWARD, float ACCELERATION__BACKWARD, float DECELERATION__BACKWARD, float timeInterval) {
  ACCELERATION__FORWARD = max(ACCELERATION__FORWARD, (float)0.0) * scaler;
  DECELERATION__FORWARD = max(DECELERATION__FORWARD, (float)0.0) * scaler;
  ACCELERATION__BACKWARD = max(ACCELERATION__BACKWARD, (float)0.0) * scaler;
  DECELERATION__BACKWARD = max(DECELERATION__BACKWARD, (float)0.0) * scaler;
  if (velocity == 0) {
    velocity += constrain(velocityTarget - velocity, -ACCELERATION__BACKWARD * timeInterval, ACCELERATION__FORWARD * timeInterval);
  } else if (velocity > 0) {
    velocity += constrain(velocityTarget - velocity, -DECELERATION__FORWARD * timeInterval, ACCELERATION__FORWARD * timeInterval);
    if (velocity < 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  } else {  //velocity < 0
    velocity += constrain(velocityTarget - velocity, -ACCELERATION__BACKWARD * timeInterval, DECELERATION__BACKWARD * timeInterval);
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
