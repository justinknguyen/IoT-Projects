#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int button1Pin = 16;
int button2Pin = 4;
int button3Pin = 13;
int buttonB1Pin = 14;
int buttonB2Pin = 5;
int buttonB3Pin = 12;

bool lampstate = false;
bool roomstate = false;
bool stripstate = false;

int lamplvl = 1;
int roomlvl = 1;
int striplvl = 1;

const char* ssid = "wifi username";
const char* password = "wifi password";
const char* endpoint1 = "http://<ip address>/api/<user id>/lights/<light number>/state";
const char* endpoint2 = "http://<ip address>/api/<user id>/lights/<light number>/state";
const char* endpoint3 = "http://<ip address>/api/<user id>/lights/<light number>/state";

void setup(void) 
{
  Serial.begin(74880);
  
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(buttonB1Pin, INPUT);
  pinMode(buttonB2Pin, INPUT);
  pinMode(buttonB3Pin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
}

void loop() 
{
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);
  int buttonB1State = digitalRead(buttonB1Pin);
  int buttonB2State = digitalRead(buttonB2Pin);
  int buttonB3State = digitalRead(buttonB3Pin);
  
  if(button1State == HIGH && lampstate == false)  // lamp on
  { 
    Serial.println("Turning lamp ON");
    lamp(true);
    lampstate = true;
    delay(50);
  }        
  else if(button1State == HIGH && lampstate == true)  // lamp off
  {
    Serial.println("Turning lamp OFF");
    lamp(false);
    lampstate = false;
    delay(50);
  }        
   
  if(button2State == HIGH && roomstate == false)  // room on
  { 
    Serial.println("Turning room ON");
    room(true);
    roomstate = true;
    delay(50);
  }        
  else if(button2State == HIGH && roomstate == true)  // room off
  {
    Serial.println("Turning room OFF");
    room(false);
    roomstate = false;
    delay(50);
  } 

  if(button3State == HIGH && stripstate == false)  // strip on
  { 
    Serial.println("Turning strip ON");
    strip(true);
    stripstate = true;
    delay(50);
  }        
  else if(button3State == HIGH && stripstate == true)  // strip off
  {
    Serial.println("Turning strip OFF");
    strip(false);
    stripstate = false;
    delay(50);
  }        

  if(buttonB1State == HIGH)  // brightness switcher
  { 
    Serial.println("Switching lamp brightness");
    lampbrightness(lamplvl);
    lamplvl++;
    if(lamplvl == 5)
    {
      lamplvl = 1;
    }
    delay(50);
  } 
  
  if(buttonB2State == HIGH)  // brightness switcher
  { 
    Serial.println("Switching room brightness");
    roombrightness(roomlvl);
    roomlvl++;
    if(roomlvl == 5)
    {
      roomlvl = 1;
    }
    delay(50);
  } 

  if(buttonB3State == HIGH)  // brightness switcher
  { 
    Serial.println("Switching strip brightness");
    stripbrightness(striplvl);
    striplvl++;
    if(striplvl == 5)
    {
      striplvl = 1;
    }
    delay(50);
  } 

 delay(100);
}

void lamp(bool turnOnOff) 
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
  } 
  else 
  {
    Serial.println("Request not sent: Not connected");
  }
}

void lampbrightness(int bright) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(endpoint1);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (bright==1)
    {
      http.PUT("{\"bri\":50}");
    } 
    else if (bright==2)
    {
      http.PUT("{\"bri\":100}");
    }
    else if (bright==3)
    {
      http.PUT("{\"bri\":150}");
    }
    else if (bright==4)
    {
      http.PUT("{\"bri\":200}");
    }
    else if (bright==5)
    {
      http.PUT("{\"bri\":254}");
    }
    http.end();
  } 
  else 
  {
    Serial.println("Request not sent: Not connected");
  }
}

void room(bool turnOnOff) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
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

void roombrightness(int bright) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(endpoint2);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (bright==1)
    {
      http.PUT("{\"bri\":50}");
    } 
    else if (bright==2)
    {
      http.PUT("{\"bri\":100}");
    }
    else if (bright==3)
    {
      http.PUT("{\"bri\":150}");
    }
    else if (bright==4)
    {
      http.PUT("{\"bri\":200}");
    }
    else if (bright==5)
    {
      http.PUT("{\"bri\":254}");
    }
    http.end();
  } 
  else 
  {
    Serial.println("Request not sent: Not connected");
  }
}

void strip(bool turnOnOff) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(endpoint3);
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

void stripbrightness(int bright) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    http.begin(endpoint3);
    http.addHeader("Content-Type", "text/plain;charset=UTF-8");
    if (bright==1)
    {
      http.PUT("{\"bri\":50}");
    } 
    else if (bright==2)
    {
      http.PUT("{\"bri\":100}");
    }
    else if (bright==3)
    {
      http.PUT("{\"bri\":150}");
    }
    else if (bright==4)
    {
      http.PUT("{\"bri\":200}");
    }
    else if (bright==5)
    {
      http.PUT("{\"bri\":254}");
    }
    http.end();
  } 
  else 
  {
    Serial.println("Request not sent: Not connected");
  }
}
