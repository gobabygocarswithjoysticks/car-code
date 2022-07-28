/**
  This function returns the time in seconds since it was last called.
  It uses the micros() function from Arduino, and stores the last value of micros internally with a static variable.
  The first time it is called it returns 0.
*/
float calculateTimeInterval() {
  static unsigned long lastMicros = 0;
  if (lastMicros != 0) {
    unsigned long tempMicros = micros();
    float intervalTemp = ((unsigned long)(tempMicros - lastMicros)) / 1000000.0;
    lastMicros = micros();
    return intervalTemp;
  } else { // lastMicros==0;
    lastMicros = micros();
    return 0.0;
  }
}

/**
  because Arduino only gives integer versions of the map() function
*/
float floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
  like map() but has a center position, and can have different scaling in each direction
  deadzone value is the value returned when x == inMin (since neg_out_min and pos_out_min don't have to be equal)
*/
float twoMap(float x, float neg_in_max, float inMin, float pos_in_max, float neg_out_max, float neg_out_min, float pos_out_min, float pos_out_max, float deadzoneValue) {
  if (x == inMin) {
    return deadzoneValue;
  } else if (x > inMin) {
    return floatMap(x, inMin, pos_in_max, pos_out_min, pos_out_max);
  } else { // (x < inMin)
    return floatMap(x, inMin, neg_in_max, neg_out_min, neg_out_max);
  }
}
