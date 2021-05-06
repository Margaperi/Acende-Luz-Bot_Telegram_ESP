/*
  Marga L. Periago/  @Margaperi
  Complete project details at https://RandomNerdTutorials.com/telegram-group-esp32-esp8266/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "POCO3Marga";
const char* password = "ed79dbc3bdfd";

// Initialize Telegram BOT
#define BOTtoken "1744594563:AAG4kMA00jJ_j1Zlal-E1O0UJTkaaHVFWvw"  // o teu token de bot (obtén de Botfather) 

// Usa @myidbot para descubrir o ID de chat dun individuo
// Ten en conta tamén que hai que facer clic en "iniciar" nun bot antes de que poida
// envíate unha mensaxe 
//Para coñecer o ID dun grupo, abre telegram web e copia os números precedidos de un -
#define CHAT_ID "672400945"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//const int ledPin = 2;
const int led12=12;
const int led14=14;
bool estadoled12 = LOW;
bool estadoled14 = LOW;


void xestionarNovasMensaxes (int numMensaxesNovas) {
  Serial.println("xestionar novas mensaxes ");
  Serial.println(String(numMensaxesNovas));

  for (int i=0; i<numMensaxesNovas; i++) {
    // ID de chat do solicitante 
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario non autorizado", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/AcendeLuz1") {
      bot.sendMessage(chat_id, "Luz 1 acesa ", "");
      estadoled14 = HIGH;
      digitalWrite(led14, estadoled14);
    }
    
    if (text == "/ApagaLuz1") {
      bot.sendMessage(chat_id, "Luz 1 Apagada", "");
      estadoled14 = LOW;
      digitalWrite(led14, estadoled14);
    }

     if (text == "/AcendeLuz2") {
      bot.sendMessage(chat_id, "Luz 2 Acesa ", "");
      estadoled12 = HIGH;
      digitalWrite(led12, estadoled12);
    }
    
    if (text == "/ApagaLuz2") {
      bot.sendMessage(chat_id, " Luz 2 Apagada", "");
      estadoled12 = LOW;
      digitalWrite(led12, estadoled12);
    }
    
    if (text == "/Estado") {
      if (digitalRead(estadoled14)){
        bot.sendMessage(chat_id, "Luz 1 acesa", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz 1 apagada", "");
      }
      if (digitalRead(estadoled12)){
        bot.sendMessage(chat_id, "Luz 2 acesa", "");
      }
      else{
        bot.sendMessage(chat_id, "Luz 2 apagada", "");
      }
    }



 if (text == "/Axuda")
    {
      String axuda = "Benvida o sistema de control de luces con Esp32 ou ESP8266, " ".\n";
      axuda += "Estas son as túas opcións.\n\n";
      axuda += "/AcendeLuz1: para acender a Luz 1 \n";
      axuda += "/ApagaLuz1: para apagar a luz 1 \n";
      axuda += "/AcendeLuz2: para acender a Luz 2 \n";
      axuda += "/ApagaLuz2: para apagar a luz 2 \n";
      axuda += "/Estado : devolve o estado actual das luces\n";
      axuda += "/Axuda: Imprime este menú \n";
      axuda += "Lembra co sistema distingue entre maiúculas e minúsculas \n";
      bot.sendMessage(chat_id, axuda, "");
    }


    
  }
  
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); //  Agregar certificado raíz para API.telegram.org
  #endif

  pinMode(led12, OUTPUT);
  pinMode(led14, OUTPUT);
  digitalWrite(led12, estadoled12);
  digitalWrite(led14, estadoled14);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Engade certificado raíz para api.telegram.org 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando á WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot funcionando", "");
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numMensaxesNovas = bot.getUpdates(bot.last_message_received + 1);

    while(numMensaxesNovas) {
      Serial.println("houbo  resposta ");
      xestionarNovasMensaxes (numMensaxesNovas);
      numMensaxesNovas = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
    
