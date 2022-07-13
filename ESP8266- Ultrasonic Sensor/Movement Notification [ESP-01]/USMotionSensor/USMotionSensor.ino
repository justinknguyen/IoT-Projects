#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

const int trigPin = 11;
const int echoPin = 12;
long duration;
int distance;

char auth[] = "blynk auth token";
char ssid[] = "wifi name"; //You can replace the wifi name to your wifi 
char pass[] = "wifi password";  //Type password of your wifi.

SoftwareSerial EspSerial(2, 3); // RX, TX

ESP8266 wifi(&EspSerial);

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);
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
  
  Blynk.run();
  
  if(distance<80)
  {
    //Blynk API call for NOTIFICATION pushing
    Blynk.notify("Motion Sensor - HIDE&SEEK TIME");
    Serial.println("Sending Notification...");
  }
  delay(600);
}
