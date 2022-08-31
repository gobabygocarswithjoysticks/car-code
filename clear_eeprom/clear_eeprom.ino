#include <EEPROM.h>
void setup() {
  pinMode(13, OUTPUT);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 255);
  }
  digitalWrite(13, HIGH);
}
void loop() {
}
