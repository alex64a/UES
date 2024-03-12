//---------------------------------------------------------------
//                           VEŽBA 2
//                          ZADATAK 1
//---------------------------------------------------------------
//  Modifikovati aplikaciju za sinhronizaciju vremena tako da se
//  aktuelno vreme i datum ispisuju na OLED displeju.

#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include "heltec.h"

// Replace with your network credentials
const char* ssid     = "enter_wifi";
const char* password = "enter_password";
const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3"; // YOUR TIMEZONE INFORMATION This example is for Belgrade, CET


void setup() {

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  Heltec.display->init();
  Heltec.display->clear();
  Heltec.display->flipScreenVertically();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(2, 15, "Connecting to ssid: ");
  Heltec.display->drawString(28, 30, ssid);
  Heltec.display->display();
  delay(100);
  
  WiFi.begin(ssid, password);
  delay(100);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  // Display IP address
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(2, 0, "< WiFi connected >");
  Heltec.display->drawString(2, 15, "IP address: ");
  Heltec.display->drawString(2, 30, WiFi.localIP().toString());
  Heltec.display->drawString(2, 45, "ssid>");
  Heltec.display->drawString(28, 45, ssid);
  Serial.println(WiFi.localIP());
  Heltec.display->display();
  Heltec.display->clear();
  delay(2000);

  // Initialize NTPClient
  configTime(0, 0, "0.ca.pool.ntp.org");
  printLocalTime();
  setenv("TZ", Timezone, 1);
  delay(8000); // Wait for time services, displays UTC time for 8 sec then corrects to your TZ
  WiFi.disconnect();
}

void loop() {
  
  Heltec.display -> clear();
  printLocalTime();
  delay(100);
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
 
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(2, 25, "FAIL connect NTP Server");
  Heltec.display->display();
  Heltec.display->clear();
  return;
  }
  //this routine makes the strings for the digital display
  Serial.println(&timeinfo, "%d.%m.%Y %H:%M:%S");
  char timeStringBuff[50];
  char dateStringBuff[50];
  char secStringBuff[4];
  char minStringBuff[3];
  char hourStringBuff[3];
  char ampmStringBuff[3];
  char tzStringBuff[4];
  char offsetStringBuff[4];
  
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M ", &timeinfo); // %I shows 12hr clock replace with %H for 24hr clock 
  strftime(dateStringBuff, sizeof(dateStringBuff), "#%j   %a %b %d, %y", &timeinfo); 
  //[%a] shows short day of week  | %b shows short month change to %m for month number | %j shows day of the year
  strftime(secStringBuff, sizeof(secStringBuff), ":%S", &timeinfo);
  strftime(minStringBuff, sizeof(minStringBuff), "%M", &timeinfo);
  strftime(hourStringBuff, sizeof(hourStringBuff), "%H", &timeinfo);
  strftime(ampmStringBuff, sizeof(ampmStringBuff), "%p", &timeinfo); // comment out line if you change to 24 hr clock (removes am/pm)
  strftime(tzStringBuff, sizeof(tzStringBuff), "%Z", &timeinfo); // Shows time zone info if available
  strftime(offsetStringBuff, sizeof(offsetStringBuff), "%z", &timeinfo); // Shows UTC time offset

  String TimeAsString(timeStringBuff);
  String DateAsString(dateStringBuff);
  String SecAsString(secStringBuff);
  String MinAsString(minStringBuff);
  String HourAsString(hourStringBuff);
  String AmpmAsString(ampmStringBuff);
  String TzAsString(tzStringBuff);
  String OffsetAsString(offsetStringBuff);

    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(64, 20, TimeAsString);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(64, 2, DateAsString);
    Heltec.display->drawString(99, 21, SecAsString);
    Heltec.display->drawString(99, 32, AmpmAsString); 
    Heltec.display->drawString(64, 53, WiFi.macAddress()); // mac address shown
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(0, 17, TzAsString);
    Heltec.display->drawString(2, 28, OffsetAsString);
    Heltec.display->drawString(0, 39, "UTC");
    Heltec.display->display();
  
  }
