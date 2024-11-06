#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
					   
					   
// Configurações de rede WiFi
const char* ssid = "Gustavo Siega";
const char* password = "gustavo2023";

   
ESP8266WebServer server(80);

const int led = LED_BUILTIN;

// Configurações de IP estático
IPAddress local_IP(172, 20, 10, 10);
IPAddress gateway(172, 20, 10, 1);
IPAddress subnet(255, 255, 255, 240);

void configureWiFi();
void handleRoot();
void turnOnLED();
void turnOffLED();
				  						 
void configureWiFi() {
  WiFi.mode(WIFI_STA);
  
								   
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao configurar IP.");
  }
			  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());													  
}
										   
void handleRoot() {
											   
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #4b5563; color: #f8fafc;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #ef4444;}</style></head>";
  html += "<body><h1>Controle de Luzes</h1>";
  html += "<h2>Buttons</h2>";
  html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/' + state, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
							  
void turnOnLED() {
  digitalWrite(led, LOW);
							   
  server.send(200, "text/plain", "LED Ligado!");
  Serial.println("LED foi ligado.");
															

}

								 
void turnOffLED() {
  digitalWrite(led, HIGH);
								  
  server.send(200, "text/plain", "LED Desligado!");
  Serial.println("LED foi desligado.");
}

void setup(){
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  ArduinoOTA.setPassword("Siega");

  configureWiFi();

  ArduinoOTA.onStart([](){
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([](){
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  /* Função assíncrona que lida com os erros */
  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  });

  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, turnOnLED);
  server.on("/off", HTTP_GET, turnOffLED);
  server.begin();

  ArduinoOTA.begin();
  Serial.println("Pronto para uso.");
}

														   
void loop(){
  ArduinoOTA.handle();
  server.handleClient();
}