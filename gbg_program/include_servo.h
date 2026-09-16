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
      pinMode(pin1, OUTPUT);
      analogWrite(pin1, 0);
      pinMode(pin2, OUTPUT);
      analogWrite(pin2, 0);
      pinMode(pinEn, OUTPUT);
      digitalWrite(pinEn, HIGH);
      delay(5);
      digitalWrite(pinEn, LOW);
      delayMicroseconds(12); // reset drv8245 and turn off nsleep
      digitalWrite(pinEn, HIGH);
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
