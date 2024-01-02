#include <iostream>
#include "controller/ServerController.h"

ServerController::ServerController()
{
    this->center();
}
void ServerController::begin()
{
    this->_server.onNotFound([this]() {                          // If the client requests any URI
        this->_server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });

    // Routes
    this->_server.on("/", [this](){ this->info(); });

    this->_server.on("/steering/center", [this]() { this->center(); });
    this->_server.on("/steering/right", [this]() { this->right(); });
    this->_server.on("/steering/left", [this]() { this->left(); });

    this->_server.on("/aceleration", HTTP_POST, [this]() { this->aceleration(); });

    this->_server.on("/reset", [this](){ this->reset(); });

    // Actually start the server, defaults to port 80
    this->_server.begin();
}
void ServerController::handle()
{
    this->_server.handleClient();
}

String ServerController::htmlHeader(String title)
{
    return "<html><head><title>" + title + "</title></head><body>";
}
String ServerController::htmlFooter()
{
    return "</body></html>";
}
void ServerController::reset()
{
    // Reset saved settings
    //wifiManager.resetSettings();
    DebugPrintln("Reseting Settings...");
    WiFi.disconnect(true);
    ESP.eraseConfig();
    DebugPrintln("Rebooting...");
    delay(2000);
    ESP.reset();
}
void ServerController::info()
{
    // Calculate uptime
    long millisecs = millis() / 1000;
    int systemUpTimeSc = millisecs % 60;
    int systemUpTimeMn = (millisecs / 60) % 60;
    int systemUpTimeHr = (millisecs / (60 * 60)) % 24;
    int systemUpTimeDy = (millisecs / (60 * 60 * 24));

    // compose info string
    String info = htmlHeader("Jumper Info") + "<h1>Jumper Info</h1>" +
                  "<b>Firmware Version:</b> " + VERSION + "</br></br>" +
                  "<b>ESP8266 Core Version:</b> " +
                  ESP.getCoreVersion() + "</br>" +
                  "<b>ESP8266 SDK Version:</b> " + ESP.getSdkVersion() + "</br>" +
                  "<b>ESP8266 Chip ID:</b> " + ESP.getChipId() + "</br>" +
                  "<b>ESP8266 Flash Chip ID:</b> " + ESP.getFlashChipId() + "</br></br>" +

                  "<b>Free Heap:</b> " + ESP.getFreeHeap() + "bytes (" +
                  ESP.getHeapFragmentation() + "% fragmentation)</br>" +
                  "<b>Flash Chip Size:</b> " + ESP.getFlashChipRealSize() + " bytes (" +
                  ESP.getFlashChipSize() + " bytes seen by SDK) </br>" +
                  "<b>Sketch Size:</b> " + ESP.getSketchSize() + " bytes used of " +
                  ESP.getFreeSketchSpace() + " bytes available</br></br>" +

                  "<b>WiFi SSID:</b> " + WiFi.SSID() + "</br>" +
                  "<b>WiFi RSSI:</b> " + WiFi.RSSI() + "dBm</br>" +
                  "<b>WiFi Local IP:</b> " + WiFi.localIP().toString() + "</br></br>" +

                  "<b>Reset Reason:</b> " + ESP.getResetReason() + "</br>" +
                  "<b>System Uptime:</b> " + String(systemUpTimeDy) + " day(s), " +
                  systemUpTimeHr + " hour(s), " + systemUpTimeMn + " minute(s), " +
                  systemUpTimeSc + " second(s)" + htmlFooter();

    this->_server.send(200, "text/html", info);
}

// Steering
void ServerController::center () {
    this->_steering = "C";
    this->_server.send(200, "text/html", "OK Center");
}
void ServerController::right () {
    this->_steering = "R";
    this->_server.send(200, "text/html", "OK Right");
}
void ServerController::left () {
    this->_steering = "L";
    this->_server.send(200, "text/html", "OK Left");
}
String ServerController::getSteering() {
    return this->_steering;
}

// Direction
void ServerController::aceleration() {
    this->_aceleration.direction = "S";
    this->_aceleration.power = 0;

    if ( this->_server.hasArg("direction") ) {
        String direction = this->_server.arg("direction");

        if (direction.equalsIgnoreCase("F")) {
            this->_aceleration.direction = "F";
        } else if (direction.equalsIgnoreCase("B")) {
            this->_aceleration.direction = "B";
        }
    }

    if ( this->_server.hasArg("power") ) {
        this->_aceleration.power = this->_server.arg("power").toInt();

        if (this->_aceleration.power > 255) {
            this->_aceleration.power = 255;
        }
    }
    this->_server.send(200, "text/html", "OK");
}
AcelerationData ServerController::getAceleration () {
    return this->_aceleration;
}
