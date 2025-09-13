#if defined(HAS_WIFI)
#include "settings_names.h"

const unsigned long signalLossTimeout = 1100;

#include <WiFi.h>
#include <WebServer.h>

IPAddress apIP(10, 0, 0, 1);
WebServer webServer(80);

char statusBuffer[40];

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
    webServer.send(200, "text/html", indexHTML);
  });
  webServer.on("/activate", []() {
    activatedByRemote = true;
    webServer.send(200);
  });
  webServer.on("/deactivate", []() {
    activatedByRemote = false;
    webServer.send(200);
  });
  webServer.on("/status", []() {
    if (webServer.args() == 2 && webServer.argName(0) == "fb" && webServer.argName(1) == "lr") {
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
    sprintf(statusBuffer, "{\"a\":%d,\"d\":%d,\"m\":%d,\"j\":%d}", activatedByRemote, reportedDeactivateIfRemoteDisconnects, remoteMode, joyOK);
    webServer.send(200, "application/json", statusBuffer);
  });
  webServer.on("/settings", []() {
    printSettings(true);
    webServer.send(200,"application/json", wifiSettingsBuffer);
  });

  webServer.on("/favicon.ico", []() {
    webServer.send(200,"image/x-icon",F("AAABAAEAEBACAAAAAACwAAAAFgAAACgAAAAQAAAAIAAAAAEAAQAAAAAAQAAAAAAAAAAAAAAAAgAAAAAAAAD///8AqA5JAP//AADLWwAAuEcAALtbAAC7WwAAzMcAAP//AAD//wAA//8AAP//AADEcQAAta0AAKRpAAC9rwAAxHEAAP//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
  });

  webServer.on("/timeoutOn", []() {
    deactivateIfRemoteDisconnects = true;
    webServer.send(200);
  });
  webServer.on("/timeoutOff", []() {
    deactivateIfRemoteDisconnects = false;
    webServer.send(200);
  });

  webServer.on("/remoteMode", []() {
    if (webServer.args() == 1 && webServer.argName(0) == "mode") {
      remoteMode = webServer.arg(0).toInt();
    }
    webServer.send(200, "text/html", indexHTML);
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
  if (!USE_WIFI) {
    return;
  }
  webServer.handleClient();
}

#endif
