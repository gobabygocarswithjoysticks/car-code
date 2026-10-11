#ifdef HAS_BLE
#include <BLE.h>
#include <btstack.h>
#include "BLEServiceGBG.h"

BLEServiceGBG blegbg;

void setupBLE() {
  char nameBuffer[20];
  sprintf(nameBuffer, "gbgcar%d", CAR_BLE_NAME);
  BLE.begin(nameBuffer);
  BLE.server()->addService(&blegbg);
  sm_set_io_capabilities(IO_CAPABILITY_DISPLAY_ONLY);
  sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION | SM_AUTHREQ_MITM_PROTECTION);
  sm_use_fixed_passkey_in_display_role(CAR_BLE_PASSWORD);
  BLE.startAdvertising();
  blegbg.setAutoflush(20);
}
#endif
