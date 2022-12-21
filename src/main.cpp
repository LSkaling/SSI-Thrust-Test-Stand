#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 2

#include <Arduino.h>
#include <SD.h>
#include <Adafruit_NAU7802.h>
#include <Wire.h>
#include "TeensyTimerInterrupt.h"

const int ledPin = 13;
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 2;
const int button = 5;
const int igniter = 6;

Adafruit_NAU7802 nau;

#define TIMER0_INTERVAL_MS 50L
#define TIMER0_DURATION_MS 5000L

TeensyTimer ITimer0(TEENSY_TIMER_1);

bool SDErrorFlag = false;
bool dataCollectionActive = false;

// create a function to write data to an sd card on teensy 4.1
void writeSDCard(String data)
{
  if (dataCollectionActive)
  {
    // open the file for writing
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile)
    {
      dataFile.println(data);
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else
    {
      Serial.println("error opening datalog.txt");
      SDErrorFlag = true;
    }
  }
}

void timer(void)
{
  int32_t val = nau.read();
  Serial.print("Read ");
  Serial.println(val);
  writeSDCard(String(val));
}

// create a function that returns data from a load cell

void setup()
{

  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(igniter, OUTPUT);

  digitalWrite(igniter, LOW);

  SD.begin(BUILTIN_SDCARD);
  Serial.begin(115200);

  if (!nau.begin())
  {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");
  nau.setLDO(NAU7802_3V0);
  Serial.print("LDO voltage set to ");
  switch (nau.getLDO())
  {
  case NAU7802_4V5:
    Serial.println("4.5V");
    break;
  case NAU7802_4V2:
    Serial.println("4.2V");
    break;
  case NAU7802_3V9:
    Serial.println("3.9V");
    break;
  case NAU7802_3V6:
    Serial.println("3.6V");
    break;
  case NAU7802_3V3:
    Serial.println("3.3V");
    break;
  case NAU7802_3V0:
    Serial.println("3.0V");
    break;
  case NAU7802_2V7:
    Serial.println("2.7V");
    break;
  case NAU7802_2V4:
    Serial.println("2.4V");
    break;
  case NAU7802_EXTERNAL:
    Serial.println("External");
    break;
  }

  nau.setGain(NAU7802_GAIN_128);
  Serial.print("Gain set to ");
  switch (nau.getGain())
  {
  case NAU7802_GAIN_1:
    Serial.println("1x");
    break;
  case NAU7802_GAIN_2:
    Serial.println("2x");
    break;
  case NAU7802_GAIN_4:
    Serial.println("4x");
    break;
  case NAU7802_GAIN_8:
    Serial.println("8x");
    break;
  case NAU7802_GAIN_16:
    Serial.println("16x");
    break;
  case NAU7802_GAIN_32:
    Serial.println("32x");
    break;
  case NAU7802_GAIN_64:
    Serial.println("64x");
    break;
  case NAU7802_GAIN_128:
    Serial.println("128x");
    break;
  }

  nau.setRate(NAU7802_RATE_10SPS);
  Serial.print("Conversion rate set to ");
  switch (nau.getRate())
  {
  case NAU7802_RATE_10SPS:
    Serial.println("10 SPS");
    break;
  case NAU7802_RATE_20SPS:
    Serial.println("20 SPS");
    break;
  case NAU7802_RATE_40SPS:
    Serial.println("40 SPS");
    break;
  case NAU7802_RATE_80SPS:
    Serial.println("80 SPS");
    break;
  case NAU7802_RATE_320SPS:
    Serial.println("320 SPS");
    break;
  }

  // Take 10 readings to flush out readings
  for (uint8_t i = 0; i < 10; i++)
  {
    while (!nau.available())
      delay(1);
    nau.read();
  }

  while (!nau.calibrate(NAU7802_CALMOD_INTERNAL))
  {
    Serial.println("Failed to calibrate internal offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated internal offset");

  while (!nau.calibrate(NAU7802_CALMOD_OFFSET))
  {
    Serial.println("Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated system offset");

  tone(buzzer, 200, 200);
}

void loop()
{
  if (digitalRead(button) == LOW)
  {
    dataCollectionActive = true;

    if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, timer))
    {
      Serial.print(F("Starting ITimer0 OK, millis() = "));
      Serial.println(millis());
    }
    else
      tone(buzzer, 800, 500);

    for (int i = 10; i > 0; i--)
    {
      tone(buzzer, 200, 100);
      delay(i * 100);
    }
    tone(buzzer, 200, 800);
    delay(800);
    // Ignite
    if (SDErrorFlag)
    {
      tone(buzzer, 2000, 1000);
      delay(2000);
    }
    else
    {
      writeSDCard("Ignition");
      Serial.println("Igniting");
      digitalWrite(igniter, HIGH);
      delay(10000);
      digitalWrite(igniter, LOW);
    }
    // End test
    dataCollectionActive = false;
    tone(buzzer, 200, 800);
  }
}
