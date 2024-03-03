#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <WebSocketsServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#include "wifiConfig.hpp"
#include "webServer.hpp"
#include "handleServomotor.hpp"

#include "websocketConfig.hpp"

// Replace with your network credentials

void setup()
{
  Serial.begin(115200);

  initWifi();

  initServos();

  if (!LittleFS.begin(true))
  {
    Serial.println("Error al montar LittleFS");
    return;
  }

  initServerOn();

  websocket.begin();
  websocket.onEvent(webSocketEvent);
}

void loop()
{
  websocket.loop();
}
