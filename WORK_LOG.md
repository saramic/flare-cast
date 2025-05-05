# WORK_LOG

## Next Up

- [x] get up and running with 433MHz
- [ ] compare distance of 433MHz vs BLE vs LoRA?
  - [ ] fine tune the 433MHz antenna
  - [ ] look at alternative comms with `nRF24` 2 way modules or `LoRA`
- [ ] look at using ASK directly
  - [ ] https://github.com/jecrespo/RadioHead
        see `examples/ask/ask_transmitter/ask_transmitter.pde`
        ```c
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        ```
        should be more efficient, also put speed up to 2,000bps
  - as found here
    https://forum.arduino.cc/t/ask-rf-receiving-hex-instead-of-text/698814
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

## Mon 10 Mar

- couldn't connect an Arduino Nano via USBC
  - tried `brew install --cask wch-ch34x-usb-serial-driver` but not supported
    on MacOS silicon
  - tried various ways of getting `CH34xVCPDriver` via `make setup` but failed
  - finally plugged in via `USBC` -> `USBC` cable -> `USBC port` and it worked
    🎉 seems that things are not all equal when using `USBC` -> `USB3`
- turned on Spectrum analyser and saw some activity on 433MHz
  - assumed it was the receiver
  - soldered on the antennas - short fo
  - as per this
    - https://electronics.stackexchange.com/questions/639746/why-do-my-433-mhz-receiver-and-transmitter-modules-not-work-reliably
    - the reciever module is the wide one and takes the "long" antenna even
      though the double hole looks like it could be a better fit

      ![
       QiaChip 433MHz antenna for Rx and Tx
       ](media/qiachip_433MHz_superhet_tx_rx_antenna.jpg)

    - the transmitter module is the more square one and takes the "short"
      antenna with the wiggle
  - [ ] exploring antennas is a whole other thing that can be done
    - https://www.instructables.com/433-MHz-Coil-loaded-antenna/ seems like a
      good start
    - there is some talk of dipoles online as well
    - this might be another worthwhile tutorial
      - [![
         #368 How to build performing antennas for LoRa, WiFi, 433MHz,
         Airplanes etc.(NanoVNA, MMANA-GAL) -  Andreas Spiess
         ](http://img.youtube.com/vi/6cVYsHCLKq8/0.jpg)
         ](https://youtu.be/6cVYsHCLKq8)
    - and there is probably a whole amount of info on using the **Nano VNA** to
      tune an antenna
      - [![
         #314: How to use the NanoVNA to sweep / measure an antenna system's
         SWR and optimize its tuning - w2aew
         ](http://img.youtube.com/vi/xa6dqx9udcg/0.jpg)
         ](https://youtu.be/xa6dqx9udcg)
  - [ ] looking at [**Great Scott**](https://www.youtube.com/@greatscottlab) he
    seems to prefere the `nRF24` as it allows 2 way comms
    - [ ] as per his comparison of `LoRA`, `nRF24` and `generic 433MHz
      - [![
         LoRa Module VS nRF24 VS Generic RF Module || Range & Power Test -
         GreatScott!
         ](http://img.youtube.com/vi/nP6YuwNVoPU/0.jpg)
         ](https://youtu.be/nP6YuwNVoPU)
    - [ ] might be worth seeing if he comes up with anything usefull in the
      walkie talkie
      - [![
         Creating a Walkie-Talkie with generic 433MHz RF Modules?! -
         GreatScott!
         ](http://img.youtube.com/vi/PKowvbnIxso/0.jpg)
         ](https://youtu.be/PKowvbnIxso)
  - fianally the bug was in the code 🤦‍♂️, and chagning the speed down to
    200 (_although have not checked with faster speed again_)
    ```diff
      #include <RH_ASK.h> // defaults: speed = 2000, rxPin = 11, txPin = 12, pttPin = 10

    - RH_ASK driver(2000, 12, 13); // 200bps, TX on D12, RX on D13 (pin 3)
    + RH_ASK driver(200, 13, 12); // 200bps, RX on D13 (default is 11), TX on D12
    ```
    - ie the 2nd parameter is the `rxPin` and the 3rd is the `txPin` as pwer
      the `#include` line above
- brought back some code idea that I am not sure where I got it from, to use
  the `BOOT` button on pin `0` to trigger an action
  ```c
  #define ONBOARD_BUTTON 0 // ESP32 onboard BOOT button
  int oldButtonState = LOW;

  // setup
  pinMode(ONBOARD_BUTTON, INPUT_PULLUP);

  // loop
  buttonState = digitalRead(ONBOARD_BUTTON);

  if (buttonState != oldButtonState && buttonState == HIGH)
  {
    // button was pressed code
  }
  oldButtonState = buttonState;
  ```

## Tue 25 Feb

### RX470 433MHz radio modules

- first attempt to get working
    - https://randomnerdtutorials.com/rf-433mhz-transmitter-receiver-module-with-arduino/
    - https://qiachip.com/products/qiachip-433mhz-superheterodyne-rf-receiver-and-transmitter-module
- BUT didn't seem to work
- [ ] check if there is any 433MHz signal
- [ ] solder on antennas

```
                  ------------------                WL102-341 433MHz Transmitter
                 | |--|  |--|  |--| |                              -----------
          EN     | |  |--|  |--|  | | GPIO23                      | (=======) |
          GPIO36 |  --------------  | GIPO22                      |      ████ |
          GPIO39 | |              | | GIPO1                       |      ████ |
          GPIO34 | | ESP-WROOM-32 | | GPI03                        -----------
          GPIO35 | |              | | GPI021                           | | | |
          GPIO32 | |              | | GPI019                       EN -  | | |
          GPIO33 | |              | | GPI018                      DAT ---  | |
          GPIO25 | |              | | GPI05                       VCC -----  |
          GPIO26 |  --------------  | GPI017                      GND -------
          GPIO27 |                  | GPI016
          GPIO14 |                  | GPI04
  DAT --- GPIO12 |                  | GPI02
          GPIO13 |                  | GPI015
  GND --- GND    | RST  _____  BOOT | GND
  VCC --- VIN    | [ ] / USB \  [ ] | VDD 3V3
                  ------------------
```

```
                  ------------------                RX470C 433MHz Reciver
                 | |--|  |--|  |--| |               --------------------------
          EN     | |  |--|  |--|  | | GPIO23       |  █████████████ (=======) |
          GPIO36 |  --------------  | GIPO22       |  █████████████           |
          GPIO39 | |              | | GIPO1         --------------------------
          GPIO34 | | ESP-WROOM-32 | | GPI03                            | | | |
          GPIO35 | |              | | GPI021                      VCC -  | | |
          GPIO32 | |              | | GPI019                       DO ---  | |
          GPIO33 | |              | | GPI018                       DO -----  |
          GPIO25 | |              | | GPI05                       GND -------
          GPIO26 |  --------------  | GPI017
          GPIO27 |                  | GPI016
          GPIO14 |                  | GPI04
          GPIO12 |                  | GPI02
  DO ---- GPIO13 |                  | GPI015
  GND --- GND    | RST  _____  BOOT | GND
  VCC --- VIN    | [ ] / USB \  [ ] | VDD 3V3
                  ------------------
```

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
