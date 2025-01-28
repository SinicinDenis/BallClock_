#include <Arduino.h>
#include <GyverNTP.h>
#include <Looper.h>
#include <RunningGFX.h>
#include <WiFiConnector.h>
#include <Wire.h>
#include <iarduino_RTC.h>
#include <iarduino_I2C_Software.h>	
iarduino_RTC time_rtc(RTC_DS1302, 19, 22, 21);
#include "config.h"
#include "matrix.h"
#include "settings.h"


void runString(String str) {
    RunningGFX run(matrix);
    matrix.clear();
    matrix.setModeDiag();
    run.setSpeed(10);
    run.setWindow(0, matrix.width(), 1);
    run.setColor24(0x00ff00);
    run.setFont(gfx_font_3x5);
    run.setText(str);
    run.start();

    while (run.tick() != RG_FINISH) {
        delay(1);
        yield();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n" PROJECT_NAME " v" PROJECT_VER);

    matrix.begin();
    time_rtc.begin();
    
    
    //time_rtc.settime(1,1,1,1,1,1);
    //db[kk::rtc_set_m] = time_rtc.minutes;
    //db[kk::rtc_set_h] = time_rtc.Hours;
    
    WiFiConnector.setName(PROJECT_NAME);

    WiFiConnector.onConnect([]() {
        NTP.begin();

        Serial.print("Connected: ");
        Serial.println(WiFi.localIP());
        if (db[kk::show_ip]) runString(WiFi.localIP().toString());
        
        ota.checkUpdate();
    });

    WiFiConnector.onError([]() {
        String str("Error! AP: ");
        str += WiFi.softAPSSID();
        Serial.println(str);
        runString(str);
    });
    
}



void loop() {
    
    Serial.println("Основной loop");
    Looper.loop();
    
}
