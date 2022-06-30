#include "schFirebase.hpp"
#include "schMain.h"

#include <HardwareSerial.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

static const char *s_tag = "schFB";

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;

static unsigned long dataMillis = 0;
static int count = 0;
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
        Serial.println("Firebase initialization complete...");
    }

}