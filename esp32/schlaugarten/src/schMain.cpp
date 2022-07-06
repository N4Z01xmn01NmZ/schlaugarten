#include "schMain.h"
#include "schFirebase.hpp"

#include <stdint.h>

#include <Arduino.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

static bool lampState = false;

static float soilIsDry = 0;

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);
static uint16_t count       = 0;
static float    temperature = 0;

static uint32_t currentMillis      = 0;
static uint32_t previousMillis     = 0;
static uint8_t  pulseSec           = 0;
static volatile uint8_t pulseCount = 0;
static float    flowrate           = 0;
static uint16_t flowrateMillis     = 0;
static uint32_t totalMillis        = 0;

static bool     valveOpen      = false;

void IRAM_ATTR pulseCounter() {
    pulseCount++;
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        digitalWrite(2, HIGH);
        Serial.print(".");
        delay(300);
        digitalWrite(2, LOW);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    sch::FirebaseInit();

    pinMode(SOIL_PIN, INPUT);
    pinMode(FLOWRATE_PIN, INPUT);
    pinMode(TEMP_PIN, INPUT);
    pinMode(VALVE_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    sensors.begin();

    attachInterrupt(digitalPinToInterrupt(FLOWRATE_PIN), pulseCounter, FALLING);

}

void loop() {
    sch::FirebaseUpdateDB([]() {
        sch::FirebaseReadBool("device/lamp", &lampState);
    });

    digitalWrite(LED_BUILTIN, (lampState)? HIGH : LOW);

    soilIsDry = (analogRead(SOIL_PIN) / 4095.0);
    Serial.println(soilIsDry);

    // sensors.requestTemperatures();
    // temperature = sensors.getTempCByIndex(0);
    temperature = (temperatureRead() - 14);
    count++;
    sensors.setUserDataByIndex(0, count);

    currentMillis = millis();
    if (currentMillis - previousMillis > FLOW_PULSE_INTERVAL) {
        pulseSec = pulseCount;
        pulseCount = 0;

        flowrate = ((1000.0 / (millis() - previousMillis)) * pulseSec) / FLOW_CALIB_FACTOR;
        previousMillis = millis();

        flowrateMillis =  (flowrate / 60) * 1000;
        totalMillis    += flowrateMillis;
    }

    /* Update database with sensor readings */
    sch::FirebaseUpdateDB([]() {
        sch::FirebaseWriteFloat("device/soilIsDry", soilIsDry);
        sch::FirebaseWriteInt("device/temperature", (int)temperature);
        sch::FirebaseWriteInt("device/water/flowrateMillis", flowrateMillis);
        sch::FirebaseWriteInt("device/water/totalMillis", totalMillis);
    });
}