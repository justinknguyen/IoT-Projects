#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;       

int pirPin = 7;

char auth[] = "blynk auth token"; 
char ssid[] = "wifi name"; //You can replace the wifi name to your wifi 
char pass[] = "wifi password";  //Type password of your wifi.

SoftwareSerial EspSerial(2, 3); // RX, TX

ESP8266 wifi(&EspSerial);

void setup(void) 
{
  Serial.begin(9600);
  
  pinMode(pirPin, INPUT);

  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);

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
  Blynk.run();
  
  int sensorValue = digitalRead(pirPin);
  
  if(sensorValue == HIGH)
  {
    //Blynk API call for NOTIFICATION pushing
    Blynk.notify("Motion Sensor - HIDE&SEEK TIME");
    Serial.println("Movement Detected");
    delay(50);
  }        
  else if(sensorValue == LOW)
  {      
    Serial.println("No Movement Detected");      //output
  }

 delay(1000);
}
