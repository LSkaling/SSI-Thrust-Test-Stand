#include <Arduino.h>
#include <SD.h>
#include <Adafruit_NAU7802.h>
#include <Wire.h>

const int ledPin = 13;
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 2;
const int button = 3;

Adafruit_NAU7802 nau;

// create a function to write data to an sd card on teensy 4.1
void writeSDCard(String data)
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
  }
}

// create a function that returns data from a load cell

void setup()
{

  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);

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
}

void loop()
{
  // digitalWrite(ledPin, HIGH); // turn the LED on
  // delay(1000);                // wait for 1 second
  // digitalWrite(ledPin, LOW);  // turn the LED off
  // delay(1000);                // wait for 1 second
  // Serial.println("Testing SD Card Write to datalog.tx");
  // writeSDCard("Hello World");

  while (! nau.available()) {
    delay(1);
  }
  int32_t val = nau.read();
  Serial.print("Read "); Serial.println(val);

  tone(buzzer, 200, 500);


  // scan the i2c bus for devices
}
