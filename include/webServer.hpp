

void initServerOn()
{
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
}
