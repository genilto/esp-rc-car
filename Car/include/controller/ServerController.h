#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "debug.h"
#include "AcelerationData.h"

#ifndef ServerController_h
#define ServerController_h

/**************************************************
* Gerencia as requisições HTTP do painel web local
**************************************************/
class ServerController {
  private:
    ESP8266WebServer _server;
    String _steering;
    AcelerationData _aceleration;

    String htmlHeader(String title);
    String htmlFooter();

    void reset();
    void info();

    // Steering
    void center();
    void right();
    void left();

    // Movement
    void aceleration();

  public:
    ServerController();
    void begin();
    void handle();

    String getSteering();
    AcelerationData getAceleration();
};

#endif
