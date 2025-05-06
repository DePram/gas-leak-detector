// you will get your own from your Blynk
#define BLYNK_TEMPLATE_ID      "TMPL6V-Kx6Z6F"
#define BLYNK_TEMPLATE_NAME    "Gas Leak Detector"
#define BLYNK_AUTH_TOKEN       "ti-XMpCk6FzPUKux9MP20B_tns9nEBXA"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = ""; // your Wifi
char pass[] = ""; // your Wifi's password

const int GAS_SENSOR = 34;
const int LED_PIN = 25;
const int BUZZER_PIN = 27;
const int RELAY_PIN = 32;
int GAS_THRESHOLD = 300;

BlynkTimer timer;
bool gasFlowActive = false;
bool alarmActive = false;
bool relayLocked = false;
bool alarmLocked = false;

bool enableNotif = true;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0, V1, V3, V4, V5, V6, V7);
}

// Tombol di Blynk untuk membuka relay
BLYNK_WRITE(V3) {  
  gasFlowActive = param.asInt() == 0;
  if (gasFlowActive) {
    relayLocked = false;  // Jika dinyalakan dari Blynk, relay kembali bekerja normal
  } else {
    relayLocked = true;
  }
  digitalWrite(RELAY_PIN, gasFlowActive ? LOW : HIGH);
}

// Tombol di Blynk untuk mematikan alarm
BLYNK_WRITE(V4) {
  alarmActive = param.asInt();
  if (!alarmActive) {
    alarmLocked = true;  // Jika dimatikan dari Blynk, alarm tetap mati sampai dinyalakan lagi
  } else {
    alarmLocked = false; // Jika dinyalakan lagi, alarm kembali bekerja normal
  }
  digitalWrite(BUZZER_PIN, alarmActive ? HIGH : LOW);
  digitalWrite(LED_PIN, alarmActive ? HIGH : LOW);
}

BLYNK_WRITE(V6) {
  GAS_THRESHOLD = param.asInt();
}

BLYNK_WRITE(V7) {
  enableNotif = param.asInt() == 1;
}

void sendData() {
  int gasValue = analogRead(GAS_SENSOR);
  Blynk.virtualWrite(V1, gasValue);
  Blynk.virtualWrite(V0, gasValue);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue >= GAS_THRESHOLD) {
    Blynk.virtualWrite(V5, 1);
    if (enableNotif) {
      Blynk.logEvent("gas_alert");
    }

    if (!alarmLocked) {  // Alarm hanya menyala kalau tidak dikunci oleh Blynk
      digitalWrite(BUZZER_PIN, HIGH);
      static bool ledState = false;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      Blynk.virtualWrite(V4, 1);
    }

    if (!relayLocked) {  // Relay terkunci setelah kebocoran gas terdeteksi
      digitalWrite(RELAY_PIN, LOW);
      Blynk.virtualWrite(V3, 0);
      relayLocked = true;
    }

  } else {
    Blynk.virtualWrite(V5, 0);

    if (!alarmLocked) {  // Jika alarm tidak dikunci, matikan otomatis
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      Blynk.virtualWrite(V4, 0);
    }

    if (!relayLocked) {  // Jika relay tidak dikunci, matikan otomatis
      digitalWrite(RELAY_PIN, HIGH);
      Blynk.virtualWrite(V3, 1);
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);

  Blynk.config(BLYNK_AUTH_TOKEN);

  pinMode(GAS_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);

  timer.setInterval(1000L, sendData);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.reconnect();
    delay(2000);
  }

  if (!Blynk.connected()) {
    Blynk.connect();
  }

  Blynk.run();
  timer.run();
}
