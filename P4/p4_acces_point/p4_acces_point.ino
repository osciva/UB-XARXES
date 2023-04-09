#include <ESP8266WiFi.h>

const char* ssid = "Red Nova";
const char* password = "123abc123";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("Red WiFi creada");
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.print("Dirección IP Soft-AP = ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();
        Serial.println(request);
        client.println("Hola, soy un servidor de Arduino!");
        break;
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
