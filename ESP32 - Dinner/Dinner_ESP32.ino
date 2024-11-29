#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <string.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// WiFi Credentials
const char* ssid = "ssid";
const char* password = "pass";

// Static IP Configuration
WiFiClientSecure client;

// Telegram Credentials
#define BOT_TOKEN "token"
#define CHAT_ID1 "id"
#define CHAT_ID2 "id"
#define CHAT_MSG "Food is ready, come down."

// GPIO Pins
#define RED_PIN 2
#define GREEN_PIN 17
#define BUTTON_PIN 33

// Initialize Telegram Bot
UniversalTelegramBot bot(BOT_TOKEN, client);

void initWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
      return;
  }

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);

  if (ssid != nullptr) {
      Serial.print("- Connecting to Home Router: ");
      Serial.println(ssid);
  } else {
      Serial.println("- SSID not set!");
      return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempt = 0;
  const int maxAttempts = 50;

  while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts) {
      delay(500);
      attempt++;

      if (attempt % 5 == 0) {
          if (WiFi.status() == WL_CONNECTED) {
              Serial.println("RSSI: " + String(WiFi.RSSI()) + " dBm");
          } else {
              Serial.println("Connecting...");
          }
      }
  }

  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n- Successfully connected: " + WiFi.localIP().toString());
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
  } else {
      Serial.println("\n- Connection Timeout. Restarting...");
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      delay(1000);
      ESP.restart();
  }
}

void sendTelegramMessage(const char* chatId, const String& message) {
  digitalWrite(RED_PIN, HIGH);
  Serial.println("Sending Telegram message...");

  if (bot.sendMessage(chatId, message, "Markdown")) {
    Serial.print("Successfully sent message to ");
    Serial.println(chatId);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
    delay(1000);
    digitalWrite(GREEN_PIN, LOW);
  } else {
    Serial.print("Failed to send message to ");
    Serial.println(chatId);
    digitalWrite(RED_PIN, LOW);
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  initWiFi();
  client.setInsecure();

  sendTelegramMessage(CHAT_ID1, CHAT_MSG);
  sendTelegramMessage(CHAT_ID2, CHAT_MSG);

  Serial.println("Ready. Waiting for button press...");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button pressed!");
    sendTelegramMessage(CHAT_ID1, CHAT_MSG);
    sendTelegramMessage(CHAT_ID2, CHAT_MSG);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected! Reconnecting...");
    initWiFi();
  }

  delay(200);
}
