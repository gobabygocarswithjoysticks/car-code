#if defined(HAS_WIFI)
#include "settings_names.h"

const unsigned long signalLossTimeout = 1100;

#include <WiFi.h>
#include <WebServer.h>

IPAddress apIP(10, 0, 0, 1);
WebServer webServer(80);

char statusBuffer[120];

uint16_t key = 1;

void setupWifi() {
  if (!USE_WIFI) {
    return;
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  char nameBuffer[10];
  char passBuffer[20];
  sprintf(nameBuffer, "gbgcar%d", CAR_WIFI_NAME);
  sprintf(passBuffer, "gobabygo%d", CAR_WIFI_PASSWORD);
  WiFi.softAP(nameBuffer, passBuffer);

  webServer.on("/", []() {
    webServer.send_P(200, "text/html", indexHTML);
  });
  webServer.on("/activate", []() {
    if (webServer.args() == 1 && webServer.hasArg("key") && webServer.arg("key").toInt() == key) {
      activatedByRemote = true;
      webServer.send(200);
    } else {
      webServer.send(403);
    }
  });
  webServer.on("/deactivate", []() {
    activatedByRemote = false;
    webServer.send(200);
  });
  webServer.on("/status", []() {
    if (webServer.args() == 3 && webServer.hasArg("fb") && webServer.hasArg("lr") && webServer.hasArg("key") && webServer.arg("key").toInt() == key) {
      lastRemoteCommandMillis = millis();
      remoteFB = webServer.arg("fb").toFloat();
      remoteLR = webServer.arg("lr").toFloat();
    }
    int reportedDeactivateIfRemoteDisconnects;
    if (remoteMode == 0) {
      reportedDeactivateIfRemoteDisconnects = deactivateIfRemoteDisconnects;
    }
    else {
      reportedDeactivateIfRemoteDisconnects = true;
    }
    sprintf(statusBuffer, "{\"a\":%d,\"d\":%d,\"m\":%d,\"j\":%d,\"k\":%d,\"x\":%d,\"y\":%d,\"l\":%d,\"r\":%d}", activatedByRemote, reportedDeactivateIfRemoteDisconnects, remoteMode, joyOK, key, joyXVal, joyYVal, leftMotorWriteVal, rightMotorWriteVal);
    webServer.send(200, "application/json", statusBuffer);
  });
  webServer.on("/settings", []() {
    printSettings(true);
    webServer.send(200, "application/json", wifiSettingsBuffer);
  });

  webServer.on("/setSetting", []() {
    if (webServer.args() == 1 && webServer.hasArg("setData")) {
      for (int i = 0; i < webServer.arg("setData").length(); i++) {
        settingsSerial(webServer.arg("setData")[i]);
        delay(0);
      }
    }
    webServer.send(200);
  });

  webServer.on("/timeoutOn", []() {
    if (webServer.args() ==  1 && webServer.hasArg("key") && webServer.arg("key").toInt() == key) {
      deactivateIfRemoteDisconnects = true;
      webServer.send(200);
    } else {
      webServer.send(403);
    }
  });
  webServer.on("/timeoutOff", []() {
    if (webServer.args() ==  1 && webServer.hasArg("key") && webServer.arg("key").toInt() == key) {
      deactivateIfRemoteDisconnects = false;
      webServer.send(200);
    } else {
      webServer.send(403);
    }
  });

  webServer.on("/key", []() {
    key += random(10, 10000);
    if (key == 0) key = 1;
    webServer.send(200, "text/plain", String(key));
  });

  webServer.on("/remoteMode", []() {
    if (webServer.args() == 2 && webServer.hasArg("mode") && webServer.hasArg("key") && webServer.arg("key").toInt() == key) {
      remoteMode = webServer.arg("mode").toInt();
      webServer.send(200);
    } else {
      webServer.send(403);
    }
  });

  webServer.begin();

}

void runWifiInput(float& speedInput, float& turnInput) {
  if (!USE_WIFI) {
    return;
  }
  switch (remoteMode) {
    default:
    case 0: // car is driving
      if (deactivateIfRemoteDisconnects && ((millis() - lastRemoteCommandMillis) > signalLossTimeout)) {
        speedInput = 0;
        turnInput = 0;
      }
      break;
    case 1:
      if (true && ((millis() - lastRemoteCommandMillis) > signalLossTimeout)) {
        speedInput = 0;
        turnInput = 0;
      } else {
        speedInput = remoteFB;
        turnInput = remoteLR;
      }
      break;
  }
}

void runWifi() {
  if (!USE_WIFI_val_on_startup) {
    return;
  }
  webServer.handleClient();
}

#endif
