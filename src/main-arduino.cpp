#include <Arduino.h>

// TODO: seems above Arduino.h is from ESP32 libs and has not builtin LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 13 // Define LED_BUILTIN if not already defined
#endif

long last = 0;
boolean light = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("flare-cast Arduino server is UP");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1000);                     // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay(1000);                     // wait for a second

  // NOTE: more accurate below
  // long now = millis();
  // if (now - last > 1000) // every 1 second
  // {
  //   if (light)
  //   {
  //     digitalWrite(LED_BUILTIN, LOW);
  //     Serial.print(">led:");
  //     Serial.println(0);
  //   }
  //   else
  //   {
  //     digitalWrite(LED_BUILTIN, HIGH);
  //     Serial.print(">led:");
  //     Serial.println(1);
  //   }
  //   last = now;
  // }
  // delay(20);
}