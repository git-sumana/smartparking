#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
char ssid[] = "iQOO Z5"; //SSID here
char pass[] = "Tanuja@2003";
String apiKey = "MGIGUB3GKIH49DW1";
const char* server = "api.thingspeak.com";
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
const int trig_1 = 0;
const int echo_1 = 2;
const int trig_2 = 14;
const int echo_2 = 12;
 
float distanceCM_1 = 0, resultCM_1 = 0;
float distanceCM_2 = 0, resultCM_2 = 0;
 long Time_1, Time_2;
float car_1, car_2; 
float Dist_1 = 8.0, Dist_2 = 8.0;
int total = 0;

WiFiClient client;
void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting to ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 
  pinMode(trig_1, OUTPUT);
  pinMode(trig_2, OUTPUT);
   
  pinMode(echo_1, INPUT);
  pinMode(echo_2, INPUT);
  
  digitalWrite(trig_1, LOW);
  digitalWrite(trig_2, LOW);
   
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  IoT CAR PARK");
  lcd.setCursor(0, 1);
  lcd.print(" MONITOR SYSTEM");
  delay(2000);
  lcd.clear();
}

void loop()
{
  total = 0;
  car_1 = sensor_1();
  car_2 = sensor_2();
  
  lcd.setCursor(0, 0);
  lcd.print("CAR1:");
  if (car_1 <= Dist_1)
  {
    lcd.print("OK ");
  }
  else
  {
    total += 1;
  }
  if (car_1 > Dist_1)   lcd.print("NO ");
  lcd.print("CAR2:");
  if (car_2 <= Dist_2)
  {
    lcd.print("OK ");
  }
  else
  {
    total += 1;
  }
  if (car_2 > Dist_2)   lcd.print("NO ");
  lcd.setCursor(0, 1);
   
   
   if(client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(total);
                              
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             
                             Serial.print(total);
                             
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
   
  delay(1000);
}

float sensor_1(void)
{
  digitalWrite(trig_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_1, LOW);
  Time_1 = pulseIn(echo_1, HIGH);
  distanceCM_1 = Time_1 * 0.034;
  return resultCM_1 = distanceCM_1 / 2;
}

float sensor_2(void)
{
  digitalWrite(trig_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_2, LOW);
  Time_2 = pulseIn(echo_2, HIGH);
  distanceCM_2 = Time_2 * 0.034;
  return resultCM_2 = distanceCM_2 / 2;
}