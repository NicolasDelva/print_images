#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>


const char* ssid = "SSID";
const char* password = "PASSWORD";
// Create an instance of the server
// specify the port to listen on as an argument

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients(String message) {
  ws.textAll(message);
}

//Useless in this version, kept for coming update 
StaticJsonDocument<256> doc;
JsonObject object = doc.to<JsonObject>();


//Transmits data to webSocket clients
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println((char*)data);
    notifyClients((char*) data);
  }
}


//Reacts to an event (connection / disconnection / messages)
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

//Initialize a webSocket connection
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}


//Setup
void setup() {
  Serial.begin(9600);

    //IP Statique
  IPAddress ip(192, 168, 1, 15);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 1, 1);
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  //
  initWebSocket();

  //Relay mode
  WiFi.mode(WIFI_STA);
  delay(500);
    if(!SPIFFS.begin())  /* Démarrage du gestionnaire de fichiers SPIFFS */
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }
  /* Détection des fichiers présents sur l'Esp32 */
  File root = SPIFFS.open("/");  /* Ouverture de la racine */
  File file = root.openNextFile();  /* Ouverture du 1er fichier */
  while(file)  /* Boucle de test de présence des fichiers - Si plus de fichiers la boucle s'arrête*/

  {
    Serial.print("File: "); 
    Serial.println(file.name());  
    file.close();
    file = root.openNextFile();  /* Lecture du fichier suivant */
  }
  
  /* Start the server
  //Initialize the answers depending on the requests
  */

  //Root 
  server.on ( "/",[](AsyncWebServerRequest *request) 
  {
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html", "text/html");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
  });

  /* Lorsque le serveur est actif , le fichier w3.css est utilsÃ© 
  //Useless for now, kept for css improvement*/  
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  /* Test de l'ajout d'une bibliothÃ¨que JS */
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS,"/jquery.min.js","text/css");
  });
  
  /* Lorsque le serveur est actif , le fichier script.js est utilisÃ© */  
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/script.js", "text/javascript");
    });
  
  /* Lorsque le serveur est actif , le fichier script2.js est utilisÃ© */  
    server.on("/script2.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/script2.js", "text/javascript");
    });

  /* Lorsque le serveur est actif , le fichier script.js est utilisÃ© */  
  server.on("/script.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.css", "text/css");
  });

  server.on ( "/image",[](AsyncWebServerRequest *request) 
  {
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/repository.html", "text/html");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
  // put your main code here, to run repeatedly:

}
