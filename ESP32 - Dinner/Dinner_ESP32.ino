
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <string.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "name";
const char* password = "pass";

WiFiClientSecure client;
IPAddress local_IP(x, x, x, x);
IPAddress gateway(x, x, x, x);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// Pushbullet
const char* server = "api.pushbullet.com";  // Server URL
const char* accessToken="token";  // your pushbullet access token
const char* pushTitle="Food is ready";
const char* pushMsg="come down";
  
// IFTTT
const char* serverName = "link";

// Telegram
#define BOT_TOKEN "token"
#define CHAT_ID1 "id"
#define CHAT_ID2 "id"

#define redPin 2
#define grePin 17
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void initWiFi()
{
  digitalWrite(redPin, HIGH);

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
  delay(500);
  print_wakeup_reason();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); //1 = High, 0 = Low

  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);

  initWiFi();
  delay(500);
  client.setInsecure();
  Pushbullet();
  TelegramJustin();

  /////////////////////////////////////////

  Serial.println("\nGoing to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{ }

// Pushbullet
void Pushbullet()
{
  digitalWrite(redPin, HIGH);
  
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443))
  {
    Serial.println("Connection failed!");
    digitalWrite(redPin, LOW);
    TelegramJacob();
  }
  else
  {
    Serial.println("Connected to server!");

    //HTTP REQUEST
    client.println("POST /v2/pushes HTTP/1.0");

    //HTTP HEADER
    client.println("Host: api.pushbullet.com");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Access-Token:");
    client.println(accessToken);
    client.print("Content-Length: ");
    client.println(41+strlen(pushTitle)+strlen(pushMsg));
    client.println();

    //HTTP BODY
    client.print("{\"type\": \"note\", \"title\": \"");
    client.print(pushTitle);
    client.print("\", \"body\": \"");
    client.print(pushMsg);
    client.println("\"}");
    
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        digitalWrite(grePin, HIGH);
        digitalWrite(redPin, LOW);
        delay(2000);
        digitalWrite(grePin, LOW);
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }
    client.stop();
  }
  
  digitalWrite(redPin, LOW);
}

// TELEGRAM - JACOB
void TelegramJacob()
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting Telegram...");

  UniversalTelegramBot bot(BOT_TOKEN, client);
  String message = "Food is ready, come down.";

  if (bot.sendMessage(CHAT_ID2, message, "Markdown")) {
    Serial.println("Telegram (Jacob) - Success!");
    digitalWrite(grePin, HIGH);
    digitalWrite(redPin, LOW);
    delay(2000);
    digitalWrite(grePin, LOW);
  }
  else if (!bot.sendMessage(CHAT_ID2, message, "Markdown")) {
    Serial.println("Telegram (Jacob) - Failed!");
    digitalWrite(redPin, LOW);
    ESP.restart();
  }

  digitalWrite(redPin, LOW);
}

// TELEGRAM - JUSTIN
void TelegramJustin()
{
  digitalWrite(redPin, HIGH);

  Serial.println("attempting Telegram...");

  UniversalTelegramBot bot(BOT_TOKEN, client);
  String message = "Food is ready, come down.";

  if (bot.sendMessage(CHAT_ID1, message, "Markdown")) {
    Serial.println("Telegram (Justin) - Success!");
    digitalWrite(grePin, HIGH);
    digitalWrite(redPin, LOW);
    delay(2000);
    digitalWrite(grePin, LOW);
  }
  else if (!bot.sendMessage(CHAT_ID1, message, "Markdown")) {
    Serial.println("Telegram (Justin) - Failed!");
    digitalWrite(redPin, LOW);
    IFTTT();
  }

  digitalWrite(redPin, LOW);
}

// IFTTT
void IFTTT()
{
  digitalWrite(redPin, HIGH);
  
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Data to send with HTTP POST
  String httpRequestData = "value1=" + String(random(40)) + "&value2=" + String(random(40)) + "&value3=" + String(random(40));
  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if(httpResponseCode >= 200 && httpResponseCode <= 299)
  {
    Serial.print("Success!");
    digitalWrite(grePin, HIGH);
    digitalWrite(redPin, LOW);
    delay(2000);
    digitalWrite(grePin, LOW);
  }
  else if(httpResponseCode >= 400 && httpResponseCode <= 499)
  {
    Serial.print("Failure!");
    digitalWrite(redPin, LOW);
    ESP.restart();
  }
  else if(httpResponseCode >= 500 && httpResponseCode <= 599)
  {
    Serial.print("Failure!");
    digitalWrite(redPin, LOW);
    ESP.restart();
  }

  // Free resources
  digitalWrite(redPin, LOW);
  http.end();
}
