#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;       

int trigPin = 16;
int echoPin = 5;
int ledPin = 4;
long duration;
int distance;

bool state = false;

const char* ssid = "wifi name";
const char* password = "wifi password";
const char* endpoint1 = "api link";
const char* endpoint2 = "api link";


void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");

  digitalWrite(trigPin, LOW);
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
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(60);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if(distance < 40 && state == false)
  {
    Serial.println("Movement Detected");
    Serial.println("Sending ON");
    sendHttpRequest(true);
    state = true;
  }
  else if(distance < 40 && state == true)
  {
    Serial.println("Movement Detected");
    Serial.println("Sending OFF");
    sendHttpRequest(false);
    state = false;
  }
  else
  {
    Serial.println("No Movement Detected");
  }

  delay(500);
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
