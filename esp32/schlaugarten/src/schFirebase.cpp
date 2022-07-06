#include "schFirebase.hpp"
#include "schMain.h"

#include <HardwareSerial.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;

static unsigned long dataMillis = 0;
static bool signupOK = false;

namespace sch {

    void FirebaseInit(void) {
        Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;

        if (Firebase.signUp(&config, &auth, "", "")) {
            Serial.println("Firebase signup ok...");
            signupOK = true;
        } else
            Serial.printf("%s\n", config.signer.signupError.message.c_str());

        config.token_status_callback = tokenStatusCallback;

        fbdo.setResponseSize(2048);

        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);

        FirebaseUpdateDB([=](void) {
            FirebaseWriteBool("device/auto", false);
            FirebaseWriteBool("device/lamp", true);
            FirebaseWriteBool("device/valve", false);

            FirebaseWriteInt("device/mode", 0);
            FirebaseWriteInt("device/water/flowrateMillis", 0);
            FirebaseWriteInt("device/water/totalMillis", 0);

            FirebaseWriteFloat("device/soilIsDry", 0);
            FirebaseWriteFloat("device/temperature", 0);
        });

        Serial.println("Firebase initialization complete...");
    }

    void FirebaseUpdateDB(void (*UpdateTask)(void)) {
        if (Firebase.ready() && (millis() - dataMillis > 20 || dataMillis == 0)) {
            dataMillis = millis();
            UpdateTask();
        }
    }

    void FirebaseWriteInt(const char *path, int value) {
        if (Firebase.RTDB.setInt(&fbdo, path, value))
            Serial.printf("PASSED, PATH %s, TYPE: %s\n", fbdo.dataPath().c_str(), fbdo.dataType().c_str());
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }

    void FirebaseWriteFloat(const char *path, float value) {
        if (Firebase.RTDB.setFloat(&fbdo, path, value))
            Serial.printf("PASSED, PATH: %s, TYPE: %s\n", fbdo.dataPath().c_str(), fbdo.dataType().c_str());
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }

    void FirebaseWriteBool(const char *path, bool value) {
        if (Firebase.RTDB.setBool(&fbdo, path, value))
            Serial.printf("PASSED, PATH: %s, TYPE: %s\n", fbdo.dataPath().c_str(), fbdo.dataType().c_str());
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }

    void FirebaseReadInt(const char *path, int *value) {
        if (Firebase.RTDB.getInt(&fbdo, path))
            *value = fbdo.intData();
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }

    void FirebaseReadFloat(const char *path, float *value) {
        if (Firebase.RTDB.getFloat(&fbdo, path))
            *value = fbdo.floatData();
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }
    
    void FirebaseReadBool(const char *path, bool *value) {
        if (Firebase.RTDB.getBool(&fbdo, path))
            *value = fbdo.boolData();
        else
            Serial.printf("FAILED, REASON: %s\n", fbdo.errorReason().c_str());
    }

}