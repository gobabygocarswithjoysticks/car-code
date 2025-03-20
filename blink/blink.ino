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

void setup(){
    LED_SETUP;
    LED_OFF;
}
void loop(){
    LED_ON;
    delay(250);
    LED_OFF;
    delay(750);
}
