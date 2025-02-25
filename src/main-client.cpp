#include <Arduino.h>

#define LED_BUILTIN 2 // ESP32 equivalent of Arduino 13

#include <RH_ASK.h> // defaults: speed = 2000, rxPin = 11, txPin = 12, pttPin = 10
#include <SPI.h>    // Not actually used but needed to compile

// overwrite the RX pin as D11 is missing on some ESP32's
RH_ASK driver(2000, 12, 13); // 200bps, TX on D12, RX on D13 (pin 3)

long last = 0;
boolean light = false;
uint8_t oldBuffer[12];

void setup()
{
  Serial.begin(115200);
  Serial.println("flare-cast client is UP");
  pinMode(LED_BUILTIN, OUTPUT);

  // RF 433Mhz module
  if (!driver.init())
  {
    Serial.println("init failed");
  }
}

void loop()
{
  // TODO: attempted to comment out full code and just get the client up without
  //       dealy(20) nor any other things
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    Serial.println((char*)buf);
  }

  // long now = millis();
  // if (now - last > 1000) // every 1 second
  // {
  //   Serial.print(">led:");
  //   Serial.println(light ? "1" : "0");
  //   last = now;
  // }
  // uint8_t buf[12];
  // uint8_t buflen = sizeof(buf);
  // if (driver.recv(buf, &buflen)) // Non-blocking
  // {
  //   int i;
  //   // Message with a good checksum received, dump it.
  //   Serial.print("Message: ");
  //   Serial.println((char *)buf);
  //   if (buf != oldBuffer)
  //   {
  //     if (oldBuffer[0] == '0') // only checking first character being a '0' character
  //     {
  //       digitalWrite(LED_BUILTIN, LOW);
  //       light = false;
  //     }
  //     else
  //     {
  //       digitalWrite(LED_BUILTIN, HIGH);
  //       light = true;
  //     }
  //     // oldBuffer[0] = *buf; // TODO: would this do?
  //     memcpy(oldBuffer, &buf, sizeof(buf));
  //   }
  // }
  // delay(20);
}