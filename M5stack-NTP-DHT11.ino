#include <NTPClient.h>
#include <WiFi.h>
#include <M5Stack.h>
#include <dht.h>
const char *ssid     = "mySSID";
const char *password = "myPassword";
//-14400 for US/NewYork UTC-04:00
const long utcOffsetInSeconds = -14400;
const long daylightSavingsOffset = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds-daylightSavingsOffset);

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
  displayDate();
  delay(1000);
  M5.update();
}
int displayTemperature(){
  int cTemp = DHT.temperature;
  M5.Lcd.setCursor(12,0,1);
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
  M5.Lcd.setCursor(57,97,1);
  M5.Lcd.setTextSize(10);
  if (timeClient.getHours() < 10){
    M5.Lcd.setCursor(40,97,1);
    M5.Lcd.print(" ");
  }
  M5.Lcd.print(timeClient.getHours());
  M5.Lcd.print(":");
  if (timeClient.getMinutes() < 10){
    M5.Lcd.print("0");
  }
  M5.Lcd.print(timeClient.getMinutes());
  M5.Lcd.print("  ");
}
void displayDate(){
  static int dayOfTheWeek;
  dayOfTheWeek = timeClient.getDay();
  M5.Lcd.setCursor(117,200,1);
  M5.Lcd.setTextSize(5);
  if (dayOfTheWeek == 1){
    M5.Lcd.print("MON");
  }
  else if (dayOfTheWeek == 2){
    M5.Lcd.print("TUE");
  }
  else if (dayOfTheWeek == 3){
    M5.Lcd.print("WED");
  }
  else if (dayOfTheWeek == 4){
    M5.Lcd.print("THU");
  }
  else if (dayOfTheWeek == 5){
    M5.Lcd.print("FRI");
  }
  else if (dayOfTheWeek == 6){
    M5.Lcd.print("SAT");
  }
  else if (dayOfTheWeek == 7){
    M5.Lcd.print("SUN");
  }
}
