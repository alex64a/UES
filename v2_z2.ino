//---------------------------------------------------------------
//                           VEŽBA 2
//                          ZADATAK 2
//---------------------------------------------------------------
//    Napisati aplikaciju koja:
//    1. Sinhronizuje vreme na ESP32 pomoću NTP
//    2. Omogućava korisniku unos vremena buđenja preko serijskog porta
//    3. Odlazi u Deep Sleep
//    4. Budi se u trenutku koji je korisnik zadao


#include <WiFi.h>
#include "time.h"

#define uS_TO_S_FACTOR 1000000  //faktor konverzije iz mikrosekundi u sekunde

const char* ssid = "lesswireless";
const char* password = "backamaterina";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;


//user input from serial monitor
int time_of_wakeup[2];

//time structure
struct tm timeinfo;

//variable to store user inputed time converted to seconds
long int user_time_s;

//current time in seconds
long int curr_time_s;

long int wakeup_time;


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);


  printLocalTime();

  

  calculate_time();

  
 
}

void loop() {



  while (Serial.available()) {
    Serial.println("Enter the hour of desired wakeup time (00-23):");
      time_of_wakeup[0] = Serial.parseInt();
    Serial.println("Enter the minute of desired wakeup time (00-59):");
      time_of_wakeup[1] = Serial.parseInt();

      break;
  }
  
}

void calculate_time() {

  curr_time_s = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60;
  user_time_s = time_of_wakeup[0] * 3600 + time_of_wakeup[1] * 60;

  //midnight problem
  if (curr_time_s > user_time_s)
    user_time_s += 24 * 3600;


  wakeup_time = user_time_s - curr_time_s;
}

void printLocalTime(){
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);

  time_t now;
  time(&now);
  Serial.print("UNIX TS: ");
  Serial.println(now);
  
  Serial.println();
}

void goToDeepSleep() {


   esp_sleep_enable_timer_wakeup(wakeup_time * uS_TO_S_FACTOR);
  Serial.println("ESP32 podesen za spavanje na " + String(wakeup_time) + " sekundi");

  //spavaj
  esp_deep_sleep_start();
}
