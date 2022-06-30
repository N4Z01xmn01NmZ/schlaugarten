#include "schMain.h"
#include "schFirebase.h"

#include <Arduino.h>
#include <WiFi.h>

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    sch::FirebaseInit();
}

void loop() {}