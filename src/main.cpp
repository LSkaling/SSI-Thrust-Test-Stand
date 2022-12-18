#include <Arduino.h>
#include <SD.h>


const int ledPin = 13;
const int chipSelect = BUILTIN_SDCARD;

//create a function to write data to an sd card on teensy 4.1
void writeSDCard(String data) {
  //open the file for writing
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  //if the file is available, write to it:
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  }
  //if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

//create a function that returns data from a load cell


void setup()
{
  SD.begin(BUILTIN_SDCARD);
  Serial.begin(115200);
  // put your setup code here, to run once:
}

void loop()
{
  digitalWrite(ledPin, HIGH); // turn the LED on
  delay(1000);                // wait for 1 second
  digitalWrite(ledPin, LOW);  // turn the LED off
  delay(1000);                // wait for 1 second
  Serial.println("Testing SD Card Write to datalog.tx");
  writeSDCard("Hello World");
}


