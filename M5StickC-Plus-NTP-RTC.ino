// RTC and NTP time test. Set NTP time in RTC and read back to confirm.
// DANGER WILL ROBINSON:
// Check you Ardunio library. In v0.0.8 in the IDE 1.X, there are typos.
// Edit (your arduino sketchbook)/libraries/M5StickCPlus/src/RTC.h and RTC.cpp
// and change occurrences of "GetData" and "SetData" to "GetDate" and "SetDate"

#include <M5StickCPlus.h>
#include <WiFi.h>

//OBFUSCATE!
const char* ssid     = "YOURSSID";
const char* password = "YOURPASS";

// UTC avoids hassle of DST changes
const char* ntpServer = "us.pool.ntp.org";
const long gmtOffset_sec     = 0;
const int daylightOffset_sec = 0;

void printNTPTime() {  // Output current time.
    // tm_sec,tm_min,tm_hr,tm_mday,tm_wday,tm_mon,tm_year,tm_yday,tm_isdst
    struct tm timeinfo;
    // Unfortunate name, we are really getting NTP time
    if (!getLocalTime(&timeinfo)) {  // Return 1 when the time is successfully
                                     // obtained.  成功获取到时间返回1
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    // I have no idea why this works:
    M5.Lcd.println(&timeinfo, "NTP TIME: %A, %B %d \n%Y %H:%M:%S");
}

void setRTCTime() {  
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    // tm_sec,tm_min,tm_hr,tm_mday,tm_wday,tm_mon,tm_year,tm_yday,tm_isdst
    struct tm timeinfo;
    // Unfortunate name, we are really getting NTP time
    if (!getLocalTime(&timeinfo)) {  
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    RTC_TimeStruct.Hours = timeinfo.tm_hour;
    RTC_TimeStruct.Minutes = timeinfo.tm_min;
    RTC_TimeStruct.Seconds = timeinfo.tm_sec;
    RTC_DateStruct.WeekDay = timeinfo.tm_wday;
    RTC_DateStruct.Month = timeinfo.tm_mon;
    RTC_DateStruct.Date = timeinfo.tm_mday;
    RTC_DateStruct.Year = timeinfo.tm_year;
    M5.Rtc.SetTime(&RTC_TimeStruct);
    M5.Rtc.SetDate(&RTC_DateStruct);
    // I have no idea why this works:
    M5.Lcd.println(&timeinfo, "RTC SET: %A, %B %d \n%Y %H:%M:%S");
}

void printRTCTime() {  
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    struct tm RTCtime;
    // tm_sec,tm_min,tm_hr,tm_mday,tm_wday,tm_mon,tm_year,tm_yday,tm_isdst
    M5.Rtc.GetTime(&RTC_TimeStruct);
    M5.Rtc.GetDate(&RTC_DateStruct);
    RTCtime.tm_hour = RTC_TimeStruct.Hours;
    RTCtime.tm_min = RTC_TimeStruct.Minutes;
    RTCtime.tm_sec = RTC_TimeStruct.Seconds;
    RTCtime.tm_wday = RTC_DateStruct.WeekDay;
    RTCtime.tm_mon = RTC_DateStruct.Month;
    RTCtime.tm_mday = RTC_DateStruct.Date;
    RTCtime.tm_year = RTC_DateStruct.Year;
    // I have no idea why this works:
    M5.Lcd.println(&RTCtime, "RTC TIME IS : %A, %B %d \n%Y %H:%M:%S");
}

void setup() {

    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.printf("\nConnecting to %s", ssid);
    WiFi.begin(ssid, password);  // Connect wifi and return connection status.
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);         // delay 0.5s.  延迟0.5s
        M5.Lcd.print(".");
    }
    M5.Lcd.println("\nCONNECTED!");
    configTime(gmtOffset_sec, daylightOffset_sec,
               ntpServer);  // init and get the time
    printNTPTime();
    setRTCTime();
    WiFi.disconnect(true); 
    WiFi.mode(WIFI_OFF);
    delay(20);

  //  RTC.begin();
}

void loop() {
    delay(1000);
    M5.Lcd.setCursor(0, 105);  
    printRTCTime();
}
