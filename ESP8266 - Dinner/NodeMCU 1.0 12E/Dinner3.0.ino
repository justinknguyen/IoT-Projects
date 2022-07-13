
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "JN";
const char* password = "NGUYEN0623";

// PushBullet
const char* host = "api.pushbullet.com";
const int httpsPort = 443;
const char* PushBulletAPIKEY = "o.mGVPyo4ceBY7u8aiCQlvH0prXZPotErf";

// PushingBox
String deviceId = "vE5C44958EC71191";
const char* logServer = "api.pushingbox.com";

// Telegram
#define BOT_TOKEN "1819217434:AAFr9OvmUQhDR-d612y4WWtMdXXSEGQad2I"
#define CHAT_ID1 "1582478973"
#define CHAT_ID2 "1891579569"

WiFiClientSecure client;
IPAddress local_IP(192, 168, 50, 243);
IPAddress gateway(192, 168, 50, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

#define redPin 0
#define grePin 4
#define butPin 5
int buttonState;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void initWiFi()
{
  digitalWrite(redPin, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("- connecting to Home Router SID: " + String(ssid));

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(butPin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  initWiFi();
  delay(5000);
  client.setInsecure();
  sendNotification1();
  sendNotification4();
}

void loop()
{
  digitalWrite(2, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  buttonState = digitalRead(butPin);

  client.setInsecure();

  if (buttonState == HIGH)
  {
    Serial.println("Food is ready");
    sendNotification1();
    sendNotification4();
  }
  else if (buttonState == LOW)
  {
    Serial.println("Food is not ready");
  }

  //if (millis() > 43200000) //restart every 12 hours
  //{
   // ESP.restart();
   // Serial.println("Restarting...");
  //}

  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
  {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

  delay(200);
}


// PUSHBULLET

void sendNotification1()
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting PushBullet...");

  if (!client.connect(host, httpsPort)) {
    Serial.println("PushBullet connection failed");
    Serial.println("switching to PushingBox...");
    digitalWrite(redPin, LOW);
    delay(200);
    sendNotification2("Food is ready");
  }
  else if (client.connect(host, httpsPort)) {
    String url = "/v2/pushes";
    String messagebody_note = "{\"type\": \"note\", \"title\": \"Food is ready\", \"body\": \"come down.\"}\r\n";
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " +
                 String(messagebody_note.length()) + "\r\n\r\n");
    client.print(messagebody_note);

    Serial.println("request sent");

    digitalWrite(grePin, HIGH);
    delay(800);
    digitalWrite(grePin, LOW);
  }

  digitalWrite(redPin, LOW);
}


// PUSHINGBOX

void sendNotification2(String message)
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting PushingBox...");

  if (!client.connect(logServer, 80)) {
    Serial.println("PushingBox connection failed");
    Serial.println("switching to Telegram...");
    digitalWrite(redPin, LOW);
    delay(200);
    sendNotification3();
  }
  else if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");

    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";

    Serial.println("- sending data...");

    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    digitalWrite(grePin, HIGH);
    delay(800);
    digitalWrite(grePin, LOW);
  }

  digitalWrite(redPin, LOW);
}


// TELEGRAM - JACOB

void sendNotification3()
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting Telegram...");

  UniversalTelegramBot bot(BOT_TOKEN, client);
  String message = "Food is ready, come down.";

  if (bot.sendMessage(CHAT_ID2, message, "Markdown")) {
    Serial.println("Telegram2 Successfully sent");
    digitalWrite(grePin, HIGH);
    delay(800);
    digitalWrite(grePin, LOW);
  }
  else if (!bot.sendMessage(CHAT_ID2, message, "Markdown")) {
    Serial.println("Telegram2 Failed");
    digitalWrite(redPin, LOW);
    ESP.restart();
    Serial.println("Restarting...");
  }

  digitalWrite(redPin, LOW);
}

// TELEGRAM - JUSTIN

void sendNotification4()
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting Telegram...");

  UniversalTelegramBot bot(BOT_TOKEN, client);
  String message = "Food is ready, come down.";

  if (bot.sendMessage(CHAT_ID1, message, "Markdown")) {
    Serial.println("Telegram1 Successfully sent");
    digitalWrite(grePin, HIGH);
    delay(800);
    digitalWrite(grePin, LOW);
  }
  else if (!bot.sendMessage(CHAT_ID1, message, "Markdown")) {
    Serial.println("Telegram1 Failed");
    digitalWrite(redPin, LOW);
    ESP.restart();
    Serial.println("Restarting...");
  }

  digitalWrite(redPin, LOW);
}
