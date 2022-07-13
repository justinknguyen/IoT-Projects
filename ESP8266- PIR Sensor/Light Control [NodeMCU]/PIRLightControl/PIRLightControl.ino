#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;       

int pirPin = 16;
int ledPin = 4;

bool state = false;

const char* ssid = "wifi name";
const char* password = "wifi password";
const char* endpoint1 = "api link";
const char* endpoint2 = "api link";

void setup(void) 
{
  Serial.begin(9600);
  
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");

  digitalWrite(pirPin, LOW);
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++)
    {
      Serial.print(".");
      delay(1000);
    }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void loop() 
{
  int sensorValue = digitalRead(pirPin);
  
  if(sensorValue == 1 && state == false)
  {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    
    Serial.println("Movement Detected");
    Serial.println("Sending ON");
    sendHttpRequest(true);
    state = true;
    delay(50);
  }        
  
  else if(sensorValue == 1 && state == true)
  {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
   
    Serial.println("Movement Detected");
    Serial.println("Sending OFF");
    sendHttpRequest(false);
    state = false;
    delay(50);
  }        
   
  else if(sensorValue == 0)
  {      
    digitalWrite(ledPin, LOW);   //the led visualizes the sensors output pin state
    Serial.println("No Movement Detected");      //output
  }

 delay(1000);
}

void sendHttpRequest(bool turnOnOff) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(endpoint1);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (turnOnOff==true)
    {
      http.PUT("{\"on\":true}");
    } 
    else if (turnOnOff==false)
    {
      http.PUT("{\"on\":false}");
    }
    http.end();

    http.begin(endpoint2);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (turnOnOff==true)
    {
      http.PUT("{\"on\":true}");
    } 
    else if (turnOnOff==false)
    {
      http.PUT("{\"on\":false}");
    }
    http.end();
  } 
  else 
  {
    Serial.println("Request not sent: Not connected");
  }
}
