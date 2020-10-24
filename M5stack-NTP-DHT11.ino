#include <NTPClient.h>
#include <WiFi.h>
#include <M5Stack.h>
#include <dht.h>
const char *ssid     = "YourSSID";
const char *password = "YourPassword";
//-14400 for US/NewYork UTC-04:00
const long utcOffsetInSeconds = -14400;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//dht reading input to gpio 19/MISO on M5Stack
//I only used this pin for convenience, adjust below if using a different gpio pin
#define dht_apin 19
dht DHT;

void setup() {
  Serial.begin(115200);
  M5.begin();
  WiFi.begin(ssid, password);
  M5.Power.begin();
  timeClient.begin();
  
  // Lcd boot test
  M5.Lcd.fillScreen(WHITE);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
}

void loop() {
  timeClient.update();
  DHT.read11(dht_apin);
  displayTemperature();
  displayHumidity();
  displayTime();
  delay(1000);
  M5.update();
}
int displayTemperature(){
  int cTemp = DHT.temperature;
  M5.Lcd.setCursor(0,0,1);
  M5.Lcd.setTextSize(5);
  M5.Lcd.print((int)round(1.8*cTemp+32));
  M5.Lcd.print("F");
  M5.Lcd.setTextSize(4);
  M5.Lcd.print(" ");
  M5.Lcd.print(cTemp);
  M5.Lcd.print("C");
  M5.Lcd.print(" ");
}

void displayHumidity(){
  int humidity = DHT.humidity;
  M5.Lcd.setTextSize(5);
  M5.Lcd.print(humidity);
  M5.Lcd.print("%");
}

void displayTime(){
  M5.Lcd.setCursor(57,100,1);
  M5.Lcd.setTextSize(10);
  M5.Lcd.print(timeClient.getHours());
  M5.Lcd.print(":");
  if (timeClient.getMinutes() < 10){
    M5.Lcd.print("0");
  }
  M5.Lcd.print(timeClient.getMinutes());
  M5.Lcd.print("  ");
}
