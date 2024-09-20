// for rpi pico w

#ifdef PICO_W
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
WiFiServer server(80);
DNSServer dnsServer;
IPAddress apIP(172, 217, 28, 1);

String indexHTML = ""
                   "<!DOCTYPE html><html lang='en'><head>"
                   "<meta name='viewport' content='width=device-width'>"
                   "<title>CaptivePortal</title></head><body>"
                   "<h1>Hello World!</h1><p>This is a captive portal example."
                   " All requests will be redirected here.</p></body></html>";


void setupWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("gbgcar", "123456");

  dnsServer.start(80, "*", apIP);

  webServer.onNotFound([]() {
    webServer.send(200, "text/html", index_HTML);
  });
  webServer.begin();
}

void runWifi() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
#endif
