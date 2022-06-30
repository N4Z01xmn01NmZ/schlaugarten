#include "schFirebase.h"
#include "schMain.h"

#include <esp_log.h>

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

static const char *s_tag = "schFB";

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;

static unsigned long dataMillis = 0;
static int count = 0;

void FirebaseInit(void) {
    ESP_LOGI(s_tag, "Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    config.api_key = API_KEY;

    auth.user.email    = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    config.token_status_callback = tokenStatusCallback;

    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}
