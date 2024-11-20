#Integração com Alexa

##### Este projeto tem o objetivo de conectar a ESP8266 com o uma rede Wi-Fi e ser controlado remotamente via a API SinricPro. Essa que faz a integração com a placa e a Alexa.

## Código

1. **Bibliotecas Necessárias**:

   ```cpp
   #include <ESP8266WiFi.h>
   #include <SinricPro.h>
   #include <SinricProSwitch.h>
   ```

   Essas bibliotecas permitem a conexão com Wi-Fi e a integração com a API SinricPro.

2. **Credenciais de Rede e SinricPro**:

   - Defina o nome e a senha da rede Wi-Fi, bem como as chaves `APP_KEY`, `APP_SECRET` e `SWITCH_ID` obtidas no SinricPro.

   ```cpp
   #define WIFI_SSID "Nome_da_Rede"
   #define WIFI_PASS "Senha_da_Rede"
   #define APP_KEY "sua_app_key"
   #define APP_SECRET "seu_app_secret"
   #define SWITCH_ID "seu_switch_id"
   ```

### Conexão Wi-Fi

A função `setupWiFi` realiza a conexão do ESP8266 com a rede Wi-Fi e exibe o endereço IP no monitor serial ao estabelecer a conexão.

```cpp
void setupWiFi() {
  Serial.printf("\r\n[WiFi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}
```

### Configuração SinricPro

A função `setupSinricPro` inicializa a conexão com o SinricPro e define os callbacks para controlar o dispositivo via comandos remotos.

```cpp
void setupSinricPro() {
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
```

### Controle Manual (Botão)

A função `handleButtonPress` lida com a pressão do botão físico, alternando o estado do dispositivo e enviando o novo estado para o SinricPro.

```cpp
void handleButtonPress() {
  unsigned long actualMillis = millis();
  if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000) {
    myPowerState = !myPowerState;
    digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
    SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
    mySwitch.sendPowerStateEvent(myPowerState);
    lastBtnPress = actualMillis;
  }
}
```

## Estrutura do Código

1. **setup()**:

   - Configura os pinos para o botão e o relé.
   - Inicializa a conexão serial e configura o Wi-Fi e o SinricPro.

2. **loop()**:
   - Chamadas para `handleButtonPress()` e `SinricPro.handle()` são feitas para monitorar o botão e gerenciar os comandos da plataforma SinricPro.

```cpp
void loop() {
  handleButtonPress();
  SinricPro.handle();
}
```

## Arquitetura

<p align="center">
  <img alt="Arquitetura Integração Alexa" src="imgs/arquitetura.jpeg">
</p>

## Imagens do Desenvolvimento do Projeto

<p align="center">

</p>
