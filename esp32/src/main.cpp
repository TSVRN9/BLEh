#include <Arduino.h>
#include "NimBLEDevice.h"

// Using proper 16-bit UUIDs (add 0x and pad to 4 chars)
const char* SERVICE_UUID = "180F";  // Standard battery service UUID as example
const char* CHAR_UUID_TX = "2A19";  // Standard battery level char UUID
const char* CHAR_UUID_RX = "2A20";  // Standard temp char UUID
NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pTxCharacteristic = nullptr;

bool deviceConnected = false;

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    deviceConnected = true;
    // More conservative connection parameters --figure this out
    pServer->updateConnParams(connInfo.getConnHandle(), 
                            12,  // min interval 15ms
                            12,  // max interval 15ms
                            0,   // slave latency
                            60); // timeout 600ms
    Serial.print("Client connected from: ");
    Serial.println(connInfo.getAddress().toString().c_str());
  }

  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    deviceConnected = false; 
    Serial.println("Client disconnected - start advertising");
    NimBLEDevice::startAdvertising();
  }
};

class RxCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    std::string value = pCharacteristic->getValue();
    if (!value.empty()) {
      Serial.print("Received: ");
      Serial.println(value.c_str());
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to settle
  Serial.println("\n=== Starting NimBLE Server ===");

  // Initialize BLE
  NimBLEDevice::init("Jayden's tag");
  //figure this out
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

}