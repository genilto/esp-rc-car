#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "debug.h"
#include "controller/OTAUpdateController.h"
#include "controller/ServerController.h"
#include "controller/DirectionController.h"
#include "controller/AcelerationController.h"

// #define SD1 8
// #define CLK 6
// #define SDO 7
// #define CMD 11

OTAUpdateController OTAUpdate;
ServerController server;
DirectionController direction;
AcelerationController aceleration;

void setup()
{
  // Inicia a Serial
  DebugBegin(115200);

  // WiFiManager
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(debugOn);
  wifiManager.autoConnect(WIFI_CONFIG_AP_NAME);

  // Start MDNS and OTA Updates
  OTAUpdate.begin();

  // Start Webserver
  server.begin();

  // Movement Controller
  direction.begin();
  aceleration.begin();

  pinMode(LED_INFO_PIN, OUTPUT);
  digitalWrite(LED_INFO_PIN, HIGH); //inverted, off

  pinMode(LED_INFO_PIN, OUTPUT);
}

void loop()
{

  OTAUpdate.handle();
  server.handle();
  direction.handle(server.getSteering());
  aceleration.handle(server.getAceleration());

  // delay (1000);
  // digitalWrite(LED_INFO_PIN, LOW);
  // delay (1000);
  // digitalWrite(LED_INFO_PIN, HIGH);

}
