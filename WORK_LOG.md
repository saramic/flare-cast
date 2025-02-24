# WORK_LOG

## Next Up

- [ ] get up and running with 433Mhz
- [ ] compare distance of 433Mhz vs BLE vs LoRA?
- [ ] circuit to turn on/off arc lighter
- [ ] state diagram for safety with undirectional comms.
    1. waiting for driver - slow flash
    1. driver connected - faster flash
    1. driver ready - fast flash
    1. press button to disarm
    1. driver set - permanantly on
    1. driver activate
    1. fire arc lighter 3 times? what duration? can it go again?
    1. driver done - slow flash
        1. is there a keep alive to make sure driver is in distance?
- [ ] get a better way to strip/trim c strings
- [ ] check who is connected and attempt to reconnect?
- [ ] look at Chrome web working with my ESP32 project
  https://github.com/RuiSantosdotme/esp32-web-ble
- [ ] look at these videos for more robuts solution
    - https://www.instructables.com/ESP32-BLE-Android-App-Arduino-IDE-AWESOME/
    - https://youtu.be/RvbWl8rZOoQ
    - https://youtu.be/aM2ktMKAunw

## Mon 24 Feb

### Arduino C string carriage and line break \r\n

- there has to be a better way??? like `.trim()` or `.rtrim()`?

```c
std::string stringValue = "some string\r\n";
if (!stringValue.empty() && stringValue[stringValue.size() - 1] == '\n')
{
stringValue.erase(stringValue.size() - 1);
}
if (!stringValue.empty() && stringValue[stringValue.size() - 1] == '\r')
{
stringValue.erase(stringValue.size() - 1);
}
```

### Arduino C check the characters of a string

```c
std::string stringValue = "some string";
Serial.println("stringValue as characters:");
for (char c : stringValue)
{
  Serial.print((int)c);
  Serial.print(" ");
}
Serial.println();
```

### Arduino C String vs char*

- this one seems to be solved with

```c
(char*)stringValue.c_str()

// id
std::string stringValue = "the string";
// OR
String stringValue = "the string";
Serial.println((char*)stringValue.c_str());
```

### ESP32 Bluetooth Attempt 2

- followed a more complex tutorila
    - https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
- involving setting up Bluetooth "characteristics"

```c
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  ...
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
```

- setting up bluetooth advertising with special bits to help with iPhone
  connection issues

```c
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
```

- and finally setting and receiving values

```c
  // set value and send notification
  pCharacteristic->setValue("Bluetooth fired up");
  pCharacteristic->notify();

  // get value
  std::string rxValue = pCharacteristic->getValue();

  // print it out
  Serial.println((char*)rxValue.c_str());

  // or access character by character
  for (int i = 0; i < rxValue.length(); i++) {
    Serial.print(rxValue[i]);
  }
```

- BUT comparison didn't seem to work due to the end character? or something to
  do with a differenc in std::string and String?

```c
   // didn't work
   if(rxValue == "hi")

  // this for rxValue = "hi"
  for (int i = 0; i < rxValue.length(); i++) {
    Serial.print(i);
    Serial.print(rxValue[i]);
  }
  // output
  1h2i3

  // following worked
  if(rxValue[0] == 'h' && rxValue[1] == 'i')
```

- technically `trim()` should be able to fix this? if it was a `String` but as
  it's `std::string` it seemed to crash?

  ```c
  String stringOne = "Hello!       ";
  stringOne.trim(); // trim off whitespace
  ```

- you could move in the `null terminator`

  ```c
  std::string msg = "Hello\n";
  msg[strlen(msg) - 1] = 0;  // move null-terminator in
  ```
- also this one from StackOverflow
  - https://stackoverflow.com/a/1488815

```c
#include <algorithm>
#include <string>

std::string str;

str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
```

- the OTHER ISSUE was very flaky connectivity from the iPhone
    - I ended up buying the following apps to connect to BLE via a terminal and
      send some messages
    - https://apps.apple.com/au/app/esp32-ble-terminal/id1567601472
    - https://apps.apple.com/au/app/ble-terminal-hm-10/id1398703795
    - both require further payment to remove ads
- seemed to have best chance to connect if I restarted the ESP32 and asked the
  app to connect straight afterwards. Feels as if something in the
  `void setup()` should continue running to find or re-establish connections
  - [ ] update `loop()` to track who is connected and attempt a reconnect?

- more useful tutorials on Bluetooth
  - seems like the `#include "BluetoothSerial.h"` approach is bluetooth classic
    - https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/
  - and BLE is
    - https://randomnerdtutorials.com/esp32-web-bluetooth/
    - [ ] using website https://ruisantosdotme.github.io/esp32-web-ble/
    - [ ] and the source https://github.com/RuiSantosdotme/esp32-web-ble
  - for future there is also **WiFi** provisioning via **BLE**
    - [ ] https://randomnerdtutorials.com/esp32-wi-fi-provisioning-ble-arduino/
  - could also build your own app
    - [ ] https://www.instructables.com/ESP32-BLE-Android-App-Arduino-IDE-AWESOME/
    - [ ] Create a BLE app for your mobile phone! Control an ESP32 with BLE -
          MoThunderz

          [![Create a BLE app for your mobile phone! Control an ESP32 with BLE -
             MoThunderz
            ](http://img.youtube.com/vi/RvbWl8rZOoQ/0.jpg)
            ](https://youtu.be/RvbWl8rZOoQ)

    - [ ] Create your own App! Control an ESP32 (Arduino) via Bluetooth - Part 1
          MoThunderz

          [![Create your own App! Control an ESP32 (Arduino) via Bluetooth -
             Part 1 - MoThunderz
            ](http://img.youtube.com/vi/aM2ktMKAunw/0.jpg)
            ](https://youtu.be/aM2ktMKAunw)

### ESP32 Bluetooth Unique ID

- using the legacy Bluetooth approach below, I could get the unique MAC address
  as part of the name, but needed to `.begin` once and only then could I get it

```c
  SerialBT.begin("ESP32");
  String address = SerialBT.getBtAddressString();
  SerialBT.begin("ESP32:" + address);
```

### ESP32 Bluetooth Attempt 1

- attempted Bluetooth with a simple snippet using `BluetoothSerial.h` via
    - https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/
    - https://www.electronicwings.com/esp32/esp32-bluetooth-getting-started
    - https://lastminuteengineers.com/esp32-bluetooth-classic-tutorial/
- this did show up on a scan using a Mac but not using a Mobile
- the mentioned app “Serial Bluetooth Terminal” no longer seems to exist

```c
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
```