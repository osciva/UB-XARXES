#include <ESP8266WiFi.h>

const char* ssid = "Mi 10"; // Your wireless network name (SSID)
const char* password = "123abc123"; // Your Wi-Fi network password

WiFiClient client;

// ThingSpeak Settings
const int channelID = 1976045; // Channel ID
String writeAPIKey = "3PH70H1JK37GNKDV"; // Write API key for your Thingspeak Channel
const char* server = "api.thingspeak.com"; 
const int postingInterval = 20*1000; // Post data every 20 seconds

void connectToWiFi(void){ // Funció per generar un IP i connectar-se a una xarxa WiFi
    int contador = 0;
    if(WiFi.status() != WL_CONNECTED){ // Comprova si estem connectats a una xarxa.
        WiFi.begin(ssid,password); // Inicialitza la configuració de xarxa de la biblioteca Wi-Fi i proporciona l'estat actual.
        while(WiFi.status() != WL_CONNECTED){ // Comprova si estem connectats a una xarxa. Bucle per esperar a connectar-se.
            delay(1000); // Esperem 1s.
            Serial.println("Connecting...");
            contador++;
            if(contador==10){ // Temps d'espera esgotat.
                break;
            }
        }
        Serial.println(WiFi.localIP()); // Imprimim l'adreça IP del WiFi shield.
    }
}

void setup(){
  Serial.begin(9600);  // Baud Rate a 9600
  WiFi.mode(WIFI_STA); // Serveix per deixar únicament el mode STA i desactivar l'AP en cas que ho haguéssim configurat prèviament ja que l'ESP8266 guarda la configuració del WiFi encara que ho reprogramem. 
  WiFi.disconnect(); // Desconnecta el “WiFi shield” de la xarxa actual.
  delay(2000); // Atura el programa durant la quantitat de temps (en ms) indicada.
  Serial.println("Setup done");
}
void loop(){
  connectToWiFi(); // Ens connectem a la xarxa i generem una IP
  if (client.connect(server, 80)) { // Connect to the IP address and port specified. The return value indicates success or failure
    long rssi = WiFi.RSSI(); // Measure Signal Strength (RSSI) of Wi-Fi connection 
    String body="field1=";   // Construct API request body
    body+=String(rssi);
    Serial.print("RSSI: "); 
    Serial.println(rssi); // Imprimim la RSSI de la xarxa
    // Prints per construir la graficació de la variació de la RSSI
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP8266 (nothans)/1.0");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded"); 
    client.println("Content-Length: " + String (body.length()));
    client.println("");
    client.print(body);
  }
  client.stop(); // Stop the client
  delay(postingInterval); // Esperem la cuantitat de segons indicada per representar la següent mesura de la RSSI en la gràfica
}
