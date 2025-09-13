#if defined(HAS_WIFI)
#include "settings_names.h"

const unsigned long signalLossTimeout = 1100;

#include <WiFi.h>
#include <WebServer.h>

IPAddress apIP(10, 0, 0, 1);
WebServer webServer(80);

char statusBuffer[40];

uint16_t key=1;

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
    if(webServer.args()==1 && webServer.argName(0)=="key"&& webServer.arg(0).toInt()==key){
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
    if (webServer.args() == 3 && webServer.argName(0) == "fb" && webServer.argName(1) == "lr" && webServer.argName(2) == "key" && webServer.arg(2).toInt() == key) {
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
    sprintf(statusBuffer, "{\"a\":%d,\"d\":%d,\"m\":%d,\"j\":%d,\"k\":%d}", activatedByRemote, reportedDeactivateIfRemoteDisconnects, remoteMode, joyOK, key);
    webServer.send(200, "application/json", statusBuffer);
  });
  webServer.on("/settings", []() {
    printSettings(true);
    webServer.send(200,"application/json", wifiSettingsBuffer);
  });

  webServer.on("/setSetting", []() {
    if (webServer.args() == 1 && webServer.argName(0) == "setData") {
      for(int i = 0; i < webServer.arg(0).length(); i++){
        settingsSerial(webServer.arg(0)[i]);
      } 
    }
    webServer.send(200);
  });

  webServer.on("/timeoutOn", []() {
    if(webServer.args() ==  1 && webServer.argName(0) == "key" && webServer.arg(0).toInt() == key){
      deactivateIfRemoteDisconnects = true;
      webServer.send(200);
    }else{
      webServer.send(403);
    }
  });
  webServer.on("/timeoutOff", []() {
    if(webServer.args() ==  1 && webServer.argName(0) == "key" && webServer.arg(0).toInt() == key){
      deactivateIfRemoteDisconnects = false;
      webServer.send(200);
    }else{
      webServer.send(403);
    }
  });

  webServer.on("/key",[](){
    key+=random(10,10000);
    webServer.send(200,"text/plain",String(key));
  });

  webServer.on("/remoteMode", []() {
    if (webServer.args() == 2 && webServer.argName(0) == "mode" && webServer.argName(1)=="key" && webServer.arg(1).toInt()==key) {
      remoteMode = webServer.arg(0).toInt();
      webServer.send(200);
    }else{
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
  if (!USE_WIFI) {
    return;
  }
  webServer.handleClient();
}

#endif
