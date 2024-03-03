AsyncWebServer server(80);

WebSocketsServer websocket(81);

void initWifi()
{

    const char *ssid = "GWIFI";
    const char *password = "independenciaGatica38";

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
}
