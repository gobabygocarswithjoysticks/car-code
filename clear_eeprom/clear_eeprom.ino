#include <EEPROM.h>
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 255);
  }
  digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
}
