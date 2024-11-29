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
  Serial.print("- Connecting to Home Router: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  const int maxAttempts = 50;
  int attempt = 0;

  while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts) {
    Serial.print(".");
    delay(500);
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Successfully connected: " + WiFi.localIP().toString());
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    client.setInsecure();
  } else {
    Serial.println("- Connection Timeout. Restarting...");
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    WiFi.disconnect();
    delay(1000);
    ESP.restart();
  }
}

bool sendTelegramMessage(const char* chatId, const String& message) {
  Serial.println("- Sending Telegram message...");
  const int maxRetries = 3;
  int attempt = 0;

  while (attempt < maxRetries) {
    digitalWrite(RED_PIN, HIGH);
    if (bot.sendMessage(chatId, message, "Markdown")) {
      Serial.print("- Successfully sent message to ");
      Serial.println(chatId);
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      delay(500);
      digitalWrite(GREEN_PIN, LOW);
      return true;
    } else {
      Serial.print("- Failed to send message to ");
      Serial.print(chatId);
      Serial.print(" (Attempt ");
      Serial.print(attempt);
      Serial.println(")");
      digitalWrite(RED_PIN, LOW);
      delay(500);
    }
    attempt++;
    if (attempt == maxRetries) {
      Serial.println("- All attempts to send message failed.");
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  delay(3000);
  initWiFi();
  Serial.println("- Ready. Waiting for button press...");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("- Button pressed!");
    bool msg1 = sendTelegramMessage(CHAT_ID1, CHAT_MSG);
    bool msg2 = sendTelegramMessage(CHAT_ID2, CHAT_MSG);
    if (!msg1 || !msg2) {
      WiFi.disconnect();
      delay(1000);
      ESP.restart();
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("- Wi-Fi disconnected! Reconnecting...");
    initWiFi();
  }
  delay(100);
}
