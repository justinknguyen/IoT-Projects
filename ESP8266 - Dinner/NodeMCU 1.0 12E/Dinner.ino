
#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "vE5C44958EC71191";
const char* logServer = "api.pushingbox.com";

const char* ssid = "JN";
const char* password = "NGUYEN0623";

#define redPin D4
#define grePin D2
#define butPin D1
int buttonState;
int flag = 0;

void initWiFi()
{
  Serial.println("- connecting to Home Router SID: " + String(ssid));

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void setup() 
{
  Serial.begin(74880);

  initWiFi();
  
  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(butPin, INPUT);
}

void loop() 
{
  buttonState = digitalRead(butPin);
  
  if(buttonState == HIGH && flag == 0) 
  { 
    Serial.println("Food is ready");
    sendNotification("Food is ready");
  }  
  else if(buttonState == LOW) 
  { 
    Serial.println("Food is not ready");
    flag = 0;
  }  

  delay(200);
}

void sendNotification(String message){
  digitalWrite(redPin, HIGH);

  Serial.println("- starting client");
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  
  if (client.connect(logServer, 80)) {
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
    flag = 1;
  }
  client.stop();
  Serial.println("- stopping the client");
  
  digitalWrite(redPin, LOW);
}
