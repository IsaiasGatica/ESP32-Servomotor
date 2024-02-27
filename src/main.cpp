#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <WebSocketsServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>

// Replace with your network credentials
const char *ssid = "GWIFI";
const char *password = "independenciaGatica38";
int direccionEEPROM = 10;
int ang = 0;

AsyncWebServer server(80);

WebSocketsServer websocket(81);

Servo myservo;

int SPosicion = 0;
int SPosicioncomp = 0;
int Giro = 0;

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = websocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

    // send message to client
    websocket.sendTXT(num, "Connected");
  }
  break;
  case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);

    // send message to client
    // webSocket.sendTXT(num, "message here");

    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;

  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  myservo.attach(12);

  EEPROM.begin(512);
  ang = EEPROM.read(direccionEEPROM);

  Serial.print("Valor leído de la EEPROM: ");
  Serial.println(ang);

  // WiFi.softAP("Isa", "");
  // Serial.println("\nsoftAP");
  // Serial.println(WiFi.softAPIP());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin(true))
  {
    Serial.println("Error al montar LittleFS");
    return;
  }

  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/bootstrap.min.css", "text/css");
        response->addHeader("Cache-Control","max-age=86400"); // 1 día
        request->send(response); });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/bootstrap.bundle.min.js", "application/javascript");
    response->addHeader("Cache-Control", "max-age=86400"); // 1 día
        request->send(response); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/script.js", "application/javascript");
    response->addHeader("Cache-Control", "max-age=86400"); // 1 día
        request->send(response); });

  server.begin();

  websocket.begin();
  websocket.onEvent(webSocketEvent);
}

void loop()
{
  websocket.loop();

  if (Serial.available() > 0)
  {
    SPosicion = Serial.parseInt(); // Leer un valor entero desde el puerto serie

    // Asegurarse de que la posición esté en el rango adecuado (0-180)
    SPosicion = constrain(SPosicion, 0, 180);

    if (SPosicion > ang)
    {

      for (ang; ang < SPosicion; ang++)
      {
        myservo.write(ang);
        delay(8);
      }
    }
    else
    {
      for (ang; ang > SPosicion; ang--)
      {
        myservo.write(ang);
        delay(8);
      }
    }

    ang = SPosicion;
    EEPROM.write(direccionEEPROM, ang); // Almacena el valor en la dirección dada
    EEPROM.commit();                    // Guarda los cambios en la memoria EEPROM
  }
}
