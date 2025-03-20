#ifdef ESP32
#define LED_SETUP pinMode(2, OUTPUT);
#define LED_ON digitalWrite(2, HIGH);
#define LED_OFF digitalWrite(2, LOW);
#elif defined(IS_PICO) // pico
#ifdef HAS_WIFI // pico 1W or 2W
#define LED_SETUP pinMode(LED_BUILTIN, OUTPUT);
#define LED_ON digitalWrite(LED_BUILTIN, HIGH);
#define LED_OFF digitalWrite(LED_BUILTIN, LOW);
#else // pico 1 or 2
#define LED_SETUP pinMode(25, OUTPUT);
#define LED_ON digitalWrite(25, HIGH);
#define LED_OFF digitalWrite(25, LOW);
#endif // end of pico
#else // nano or uno
#define LED_SETUP pinMode(LED_BUILTIN, OUTPUT);
#define LED_ON digitalWrite(LED_BUILTIN, HIGH);
#define LED_OFF digitalWrite(LED_BUILTIN, LOW);
#endif

#include <EEPROM.h>
void setup()
{
    LED_SETUP;
    LED_ON;
#if defined(FAKE_EEPROM)
    EEPROM.begin(1024);

#endif
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 255);
        delay(1);
    }
#if defined(FAKE_EEPROM)
    EEPROM.commit(); // rp2040 and esp32 EEPROM libraries require this to be used to write the updated data to the flash that is simulating EEPROM (flash has more limited cycles)
#endif
    LED_OFF;
}
void loop()
{
}

