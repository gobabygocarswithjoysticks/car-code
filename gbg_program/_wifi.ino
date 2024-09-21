// for rpi pico w

#ifdef ARDUINO_RASPBERRY_PI_PICO_W

#include <WiFi.h>
#include <WebServer.h>

IPAddress apIP(10, 0, 0, 1);
WebServer webServer(80);

// website:
/*
  <!DOCTYPE html><html lang='en'><head>
  <title>GBG CAR</title></head><body>
  <p>Use the buttons below to turn the car's joystick on and off.</p>
  <button onclick='fetch(\"/activate\");' style='font-size: 100px;'>GO</button>
  <button onclick='fetch(\"/deactivate\");' style='font-size: 200px; float: right;'>STOP</button>
  </body></html>
*/
// run through a html minifier (like this one https://kangax.github.io/html-minifier/) to make it one line, and save memory. Then put it in indexHTML

const char indexHTML[] = "<!doctypehtml><html lang=en><title>GBG CAR</title><p>Use the buttons below to turn the car's joystick on and off.</p><button onclick='fetch(\"/activate\");'style=font-size:100px;'>GO</button> <button onclick='fetch(\"/deactivate\");'style=font-size:200px;float:right;'>STOP</button>";

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

  webServer.begin();
}

void runWifi() {
  webServer.handleClient();
}
#endif
