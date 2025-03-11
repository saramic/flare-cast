#include <Arduino.h>

#include <RH_ASK.h> // defaults: speed = 2000, rxPin = 11, txPin = 12, pttPin = 10
#include <SPI.h>    // Not actually used but needed to compile

// TODO: make generic between ESP32 pin 13 and Arduino default pin 11
// #define RX_PIN 13
#define RX_PIN 11
RH_ASK driver(200, RX_PIN, 12); // 200bps, rxPin (11 default), txPin (12 default)

// TODO: seems above Arduino.h is from ESP32 libs and has not builtin LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 13 // Define LED_BUILTIN if not already defined
#endif

long last = 0;
boolean light = false;
uint8_t oldBuffer[32]; // 32 character limit?

void setup()
{
  Serial.begin(115200);
  Serial.println("flare-cast Arduino client is UP");
  pinMode(LED_BUILTIN, OUTPUT);

  // RF 433Mhz module
  if (!driver.init())
  {
    Serial.println("init failed");
  }
  else
  {
    Serial.println("init success");
  }
}

void loop()
{
  long now = millis();
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    Serial.println((char *)buf);

    // NOTE: test for spurious characers, but none found?
    //       Message: Hello World!���Е�
    //       H 72 e 101 l 108 l 108 o 111   32 W 87 o 111 r 114 l 108 d 100 ! 33 >led:1
    //       >led:0
    //       Message: Hello World!����
    //       ��H 72 e 101 l 108 l 108 o 111   32 W 87 o 111 r 114 l 108 d 100 ! 33 >led:1
    //       >led:0
    //       Message: Hello World!����W�
    //       H 72 e 101 l 108 l 108 o 111   32 W 87 o 111 r 114 l 108 d 100 ! 33 >led:1
    for (char c : buf)
    {
      Serial.print(c);
      Serial.print(" ");
      Serial.print((int)c);
      Serial.print(" ");
    }

    // only if got a message, turn on the LED
    light = true;
    last = now;
    digitalWrite(LED_BUILTIN, HIGH);

    // TODO: identify the node
    // if (buf != oldBuffer)
    // {
    //   if (oldBuffer[0] == '0') // only checking first character being a '0' character
    //     // oldBuffer[0] = *buf; // TODO: would this do?
    //     memcpy(oldBuffer, &buf, sizeof(buf));
    // }
  }

  if (now - last > 1000) // every 1 second
  {
    Serial.print(">led:");
    Serial.println(light ? "1" : "0");
    if (light)
    {
      digitalWrite(LED_BUILTIN, LOW);
      light = false;
    }
    last = now;
  }
  delay(20);
}