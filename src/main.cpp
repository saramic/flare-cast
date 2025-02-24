#include <Arduino.h>

#define LED_BUILTIN 2 // ESP32 equivalent of Arduino 13

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// randomly generated using `uuidgen`
#define SERVICE_UUID "EC25C9C7-5A7D-4E83-A465-ECFC724BBD49"
#define CHARACTERISTIC_UUID "D824F131-3893-4593-85AA-CE1DBB8E0337"

BLECharacteristic *pCharacteristic;

long last = 0;
boolean light = false;
std::string rxOldValue;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("0");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // TODO: probably check phone is connected prior to going into loop
}

void loop()
{
  long now = millis();
  if (now - last > 1000) // every 1 second
  {
    pCharacteristic->notify();
    last = now;
  }
  std::string rxValue = pCharacteristic->getValue();

  // TODO: how to strip strip carriage return (ascii 13 \r)
  //       and line feed (ascii 10 \n)
  if (!rxValue.empty() && rxValue[rxValue.size() - 1] == '\n')
  {
    rxValue.erase(rxValue.size() - 1);
  }
  if (!rxValue.empty() && rxValue[rxValue.size() - 1] == '\r')
  {
    rxValue.erase(rxValue.size() - 1);
  }

  if (rxValue != rxOldValue)
  {
    if (rxValue == "0") // a string match on "0" the string (ie not number)
    {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print(">led:");
      Serial.println(0);
      pCharacteristic->notify();
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print(">led:");
      Serial.println(1);
      pCharacteristic->notify();
    }
    rxOldValue = rxValue;
  }

  delay(20);
}