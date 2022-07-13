
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "JN_2G_Guest";
const char* password = "NGUYEN0623";

const char* host = "api.pushbullet.com"; 
const int httpsPort = 443;
const char* PushBulletAPIKEY = "o.mGVPyo4ceBY7u8aiCQlvH0prXZPotErf";

#define redPin D5
#define grePin D2
#define butPin D1
int buttonState;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void initWiFi()
{
  digitalWrite(redPin, LOW);
  
  Serial.println("- connecting to Home Router SID: " + String(ssid));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
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
}

void loop() 
{
  digitalWrite(2, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  
  buttonState = digitalRead(butPin);
  
  if(buttonState == HIGH) 
  { 
    Serial.println("Food is ready");
    sendNotification();
  }  
  else if(buttonState == LOW) 
  { 
    Serial.println("Food is not ready");
  }  
  if(millis() > 86400000) //restart every 12 hours
  {
    ESP.restart();
    Serial.println("Restarting...");
  }

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) 
  {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
  
  delay(200);
}

void sendNotification(){
  digitalWrite(redPin, HIGH);

  WiFiClientSecure client;
  client.setInsecure();
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    digitalWrite(redPin, LOW);
    ESP.restart();
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
    
    digitalWrite(grePin, HIGH);
    delay(800);
    digitalWrite(grePin, LOW);
  }

  digitalWrite(redPin, LOW);
  Serial.println("request sent");
  //print the response

  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}
