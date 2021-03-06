/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * Simple tweet example
 *
 * App dashboard setup:
 *   Twitter widget (connect it to your Twitter account!)
 *
 * Connect a button to pin 2 and GND...
 * Pressing this button will also tweet a message! ;)
 *
 **************************************************************/
//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <SimpleTimer.h>

// Set ESP8266 Serial object
#define EspSerial Serial1

ESP8266 wifi(EspSerial);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  // Set ESP8266 baud rate
  EspSerial.begin(115200);
  delay(10);

  Blynk.begin(auth, wifi, "Makermodule", "microduino");

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  // Tweet immediately on startup
  Blynk.tweet("My Arduino project is tweeting using @blynk_app and it’s awesome!\n #arduino #IoT #blynk");

  // Setup a function to be called every 10 minutes
  timer.setInterval(10L * 60000L, tweetUptime);

  // Setup twitter button on pin 2
  pinMode(2, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler
  attachInterrupt(digitalPinToInterrupt(2), tweetOnButtonPress, CHANGE);
}

void tweetUptime()
{
  long uptime = millis() / 60000L;
  BLYNK_LOG("Tweeting every 10 minutes ;)");

  // Actually send the message.
  // Note:
  //   We allow 1 tweet per minute for now.
  //   Twitter doesn't allow identical subsequent messages.
  Blynk.tweet(String("Running for ") + uptime + " minutes.");
}

void tweetOnButtonPress()
{
  // Invert state, since button is "Active LOW"
  int isButtonPressed = !digitalRead(2);
  if (isButtonPressed) {
    BLYNK_LOG("Button is pressed.");

    Blynk.tweet("Yaaay... button is pressed! :)\n #arduino #IoT #blynk @blynk_app");
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}

