// Simple humidity/temperature deviсe. With push btn and LCD1602.
// Based on Arduino Nano.

// Version description:

//      Normally display is not highlighted. When btn pressed - it highlighted.
//      Shows humidity/temp, feels like temp and uptime during highlighting.

//
// Andrew Sotnikov aka LucaBrasi89
//  <andrew.sotnikov@zoho.com>

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#define DHTPIN 5     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int btnPin = 2;   //digital pin button is connected to

long lastUpdated = millis(); //initial time in milliseconds
int updateInterval = 30; //seconds


struct Uptime {
  long days;
  long hours;
  long mins;
};

struct Uptime getUptime()
{
  long days = 0;
  long hours = 0;
  long mins = 0;
  long secs = 0;
  secs = millis() / 1000; //convert milliseconds to seconds
  mins = secs / 60; //convert seconds to minutes
  hours = mins / 60; //convert minutes to hours
  days = hours / 24; //convert hours to days
  secs = secs - (mins * 60); //subtract the coverted seconds to minutes in order to display 59 secs max
  mins = mins - (hours * 60); //subtract the coverted minutes to hours in order to display 59 minutes max
  hours = hours - (days * 24); //subtract the coverted hours to days in order to display 23 hours max
  Uptime uptime = { days, hours, mins };

  return uptime;
};


void printTemp(int interval = 100) {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.println("DHT sensor error!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Hum : ");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" *C");
  delay(interval);
  lastUpdated = millis();

}

void printHeatInd(int interval = 2000) {

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(dht.readTemperature(),
                                   dht.readHumidity(), false);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Now feels like - ");
  lcd.setCursor(0, 1);
  lcd.print(hic);
  lcd.print("");
  lcd.print(" *C");
  delay(interval);

}

void printUptime(int interval = 2000) {

  Uptime uptime = getUptime();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Days: ");
  lcd.print(uptime.days);
  lcd.setCursor(0, 1);
  lcd.print("Hrs: ");
  lcd.print(uptime.hours);
  lcd.setCursor(8, 1);
  lcd.print("Mins: ");
  lcd.print(uptime.mins);
  delay(interval);
  lcd.clear();
}



//Checking btn state
// if the button state has changed:
void checkBtn() {

  int reading = digitalRead(btnPin);
  if (reading == LOW) {
    //btnState = reading;
    lcd.setBacklight(true);
    printTemp(5000);
    printHeatInd(5000);
    printUptime(5000);
    lcd.setBacklight(false);
    digitalWrite(btnPin, HIGH);

  }

}

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();

  // initialize the LCD
  lcd.begin();

  // Turn off the blacklight and print a message.
  lcd.backlight();
  lcd.setBacklight(false);
  pinMode(2, INPUT_PULLUP);

  printTemp();
}

void loop() {

  int secondsPassed = (millis() - lastUpdated) / 1000;
  if ( secondsPassed >= updateInterval ) {
    printTemp();
  }
  checkBtn();

}
