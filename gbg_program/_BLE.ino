#include <BLE.h>
#include <btstack.h>
#include "BLEServiceGBG.h"

BLEServiceGBG blegbg;

void setupBLE() {
  BLE.begin("GBG");
  BLE.server()->addService(&blegbg);
  sm_set_io_capabilities(IO_CAPABILITY_DISPLAY_ONLY);
  sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION | SM_AUTHREQ_MITM_PROTECTION);
  sm_use_fixed_passkey_in_display_role(123456);
  BLE.startAdvertising();
  blegbg.setAutoflush(20);
}

//int cnt = 0;
//void loop() {
//  while (blegbg.available()) {
//    Serial.println(blegbg.read());
//  }
//  blegbg.println(cnt++);
//  blegbg.println("{\"current values, millis:\":16739, \"joyXVal\":222, \"joyYVal\":223, \"turnInput\":0.6250, \"speedInput\":-0.5042, \"turnProcessed\":0.0937, \"speedProcessed\":-0.1008, \"turnToDrive\":0.0937, \"speedToDrive\":-0.1008, \"leftMotorWriteVal\":1500, \"rightMotorWriteVal\":1500, \"speedKnobVal\":-1.0000, \"movementAllowed\":true, \"joyOK\":false, \"buttons\":0, \"stopBits\":0, \"b_m_p\":\"B\",\"CHECKSUM\":372}");
//  delay(1000);
//}
