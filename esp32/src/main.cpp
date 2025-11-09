#include <Arduino.h>
#include "NimBLEDevice.h"
#include "pitchs.h"
#include "songs.h"

#define BUZZER_PIN 18 // GPIO18 pin connected to piezo buzzer
#define BUZZER_CHANNEL 0 // PWM Channel to use

// Using proper 16-bit UUIDs (add 0x and pad to 4 chars)
const char* SERVICE_UUID = "180F";  // Standard battery service UUID as example
const char* CHAR_UUID_TX = "2A19";  // Standard battery level char UUID
const char* CHAR_UUID_RX = "2A20";  // Standard temp char UUID
NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pTxCharacteristic = nullptr;

bool deviceConnected = false;
bool buzzerRequested = false;
int findMyBeepPattern = 0;
int jingleBells = 1;
int chosenSong = jingleBells;

void playTone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  if (frequency == 0) {
    noTone(pin);
    return;
  }
  ledcSetup(BUZZER_CHANNEL, frequency, 8); // 8-bit resolution 
  ledcAttachPin(pin, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, frequency);
  delay(duration);
  ledcWriteTone(BUZZER_CHANNEL, 0); // figure this out
}

void buzz() {
  Serial.println("Buzzer turned on...");

  // Configure PWM for the buzzer
  ledcSetup(BUZZER_CHANNEL, 5000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  // Iterate over the notes of the song choice
  int size = get_song_length(chosenSong);
  Serial.printf("Playing %d notes... \n", size);

  for (int thisNote = 0; thisNote < size && buzzerRequested; thisNote++) {
    Serial.printf("Playing note %d: frequency=%d\n", thisNote, get_song_notes(chosenSong)[thisNote]);

    // to calculate the note duration, take one second divided by the note type.
    int noteDuration = 1000 / get_song_durations(chosenSong)[thisNote];
    playTone(BUZZER_PIN, get_song_notes(chosenSong)[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    deviceConnected = true;
    // More conservative connection parameters
    pServer->updateConnParams(connInfo.getConnHandle(), 
                            12,  // min connection interval 15ms (12 * 1.25ms = 15ms) 
                            12,  // faster response, more power, slower response, better battery life
                            0,   // slave latency (how many connection events can be skipped)
                            60); // timeout 600ms (how long before connection is considered lost)
    Serial.print("Client connected from: ");
    Serial.println(connInfo.getAddress().toString().c_str());
  }

  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    deviceConnected = false; 
    Serial.println("Client disconnected - started advertising");
    NimBLEDevice::startAdvertising();
  }
};

class RxCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    std::string value = pCharacteristic->getValue();
    if (!value.empty()) {
      Serial.print("Received: ");
      if(sizeof(value) == 1) {
        value[0] = (char) value[0];
        Serial.printf("%d", value);
      }
      Serial.println(value.c_str());
    }
    if (value[0] == (char) 1) {
      buzzerRequested = true;
    }
    if (value[0] == (char) 0) {
      if(buzzerRequested) {
        Serial.println("Buzzer turned off");
      }
      buzzerRequested = false;
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to settle
  Serial.println("\n=== Starting NimBLE Server ===");

  // Initialize BLE
  NimBLEDevice::init("Jayden's tag");
  // figure this out
  NimBLEDevice::setSecurityAuth(/*bond*/ false, 
                               /*mitm*/ false,
                               /*sc*/ false);
 
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks()); // figure this out

  NimBLEService *pService = pServer->createService(SERVICE_UUID);

  // Create characteristics and save TX to global pointer
  NimBLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHAR_UUID_RX, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
  pRxCharacteristic->setCallbacks(new RxCallbacks());

  pTxCharacteristic = pService->createCharacteristic(CHAR_UUID_TX, NIMBLE_PROPERTY::NOTIFY);
  if (!pTxCharacteristic) {
    Serial.println("Failed to create TX characteristic!");
  }

  pService->start();
  pTxCharacteristic->setValue("Hello BLE");

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setName("Jayden's tag");
  //pAdvertising->setScanResponse(true); what is this?
  pAdvertising->start();

  Serial.println("Advertising started. Waiting for connection...");
}

void loop() {
  if (deviceConnected) {
    static unsigned long lastNotify = 0;
    if (millis() - lastNotify > 3000) {
      lastNotify = millis();
      String msg = "Hello Owen!";
      pTxCharacteristic->setValue(msg.c_str());
      pTxCharacteristic->notify();
      //Serial.println("Notified client");
    }
  }
  if (buzzerRequested) {
    static unsigned long lastPlay = 0;
    if (millis() - lastPlay > 1000) {
      Serial.println("Playing melody again...");
      buzz();  // This will play the melody again
      lastPlay = millis();
    }
  }
}