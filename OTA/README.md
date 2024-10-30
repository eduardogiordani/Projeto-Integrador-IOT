##OTA

####A main.cpp do OTA trata de configurar a placa NodeMCU para conectar-se a rede juntamente com o computador utilizando o celular como roteador. 

#####Codigo:

#####1. Inclusão das bibliotecas necessarias.

```c++
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

```
#####2. Criar as constantes que vão armazenar a rede e a senha do roteador(celular).

```c++
const char* ssid="Edu.";
const char* password="eduardofg";

```

#####3.  Dentro da void temos o Serial.begin que inicia a comunicação serial, utilizando wifi e as constantes que são a rede e senha. Posteriormente um while para criar um loop de conexão, caso falhe, aguardando 5s para tentar a reconexão exibindo uma mensagem de erro.

```c++
void setup(){
  Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

  while(WiFi.waitForConnectResult()!=WL_CONNECTED){
    Serial.println("Conexao falhou! rebooting...");
    WiFi.begin(ssid,password);
    delay(5000);
  }
```
#####4. Funções executadas dentro do OTA. Esse segmento de codigo configura callbacks que permite monitorar e responder ao progresso e a eventos de atualização OTA.

```c++
  ArduinoOTA.onStart([](){
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([](){
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total * 100)));
    });
  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]", error);
    if(error == OTA_AUTH_ERROR) Serial.println("auth Failed");
    else if(error == OTA_BEGIN_ERROR) Serial.println("Begin failed");
    else if(error == OTA_CONNECT_ERROR) Serial.println("Connect failed");
    else if(error == OTA_RECEIVE_ERROR) Serial.println("Receive failed");
  });
```
#####5.  A funcionalidade do OTA é inicializada exibindo uma mensagem de status e endereço de IP local da placa. 

```c
  ArduinoOTA.begin();
  Serial.println("pronto");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
```
#####6. Permite que a placa processe atualizaçoes OTA e verifica se a atualizaçoes disponiveis.
```c
void loop(){
  ArduinoOTA.handle();
}
```
