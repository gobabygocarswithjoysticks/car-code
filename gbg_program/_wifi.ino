// for rpi pico w

#ifdef ARDUINO_RASPBERRY_PI_PICO_W

#include <WiFi.h>
#include <WebServer.h>

IPAddress apIP(10, 0, 0, 1);
WebServer webServer(80);

void setupWifi() {
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
    webServer.send(200, "text/plain", activatedByRemote ? "on" : "off");
  });

  webServer.begin();
}

void runWifi() {
  webServer.handleClient();
}
#endif
