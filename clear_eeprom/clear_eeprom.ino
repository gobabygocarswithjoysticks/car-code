#include <EEPROM.h>
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  EEPROM.begin(1024);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 255);
    delay(1);
  }
#if defined(ARDUINO_ARCH_MBED_RP2040)|| defined(ARDUINO_ARCH_RP2040)
  EEPROM.commit(); // rp2040 EEPROM library requires this to be used to write the updated data to the flash that is simulating EEPROM (flash has more limited cycles)
#endif
  digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
}
