void handleWebSocketMessage(uint8_t num, uint8_t *payload, size_t length)
{
    // Crear un objeto JSON para analizar el mensaje
    StaticJsonDocument<200> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, payload, length);

    // Verificar si hay un error al analizar el JSON
    if (error)
    {
        Serial.printf("[%u] Error parsing JSON: %s\n", num, error.c_str());
        return;
    }

    // Verificar si el JSON contiene la clave 'dedo' y 'posicion'
    if (jsonDocument.containsKey("dedo") && jsonDocument.containsKey("angulo"))
    {
        // Obtener los valores de 'dedo' y 'posicion' del JSON
        const char *dedo = jsonDocument["dedo"];
        int angulo = jsonDocument["angulo"];

        selectServo(dedo, angulo);

        // Hacer algo con los datos...
        Serial.printf("[%u] Dedo: %s, Angulo: %d\n", num, dedo, angulo);
    }
    else
    {
        Serial.printf("[%u] JSON incompleto, falta 'dedo' o 'posicion'\n", num);
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

        handleWebSocketMessage(num, payload, length);

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
