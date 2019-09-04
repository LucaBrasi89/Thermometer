// Simple humidity/temperature deviсe. With push btn and LCD1602.
// Based on Arduino Nano.


// Version description:

//      If temperature have been changed - display higlights.
//      When btn pressed - it highlights. Shows humidity/temp and uptime during highlighting.



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

int btnState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
boolean backlightState = true;   // the previous reading from the input pin
int prevTemp = 22; // previous temp
int curTemp; // current temp

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


void tempChangeNotifier(){
//    Serial.println("Cur temp:" + String(curTemp));
//    Serial.println("Prev temp:" + String(prevTemp) + "\n");
   if (prevTemp != curTemp) {
      lcd.setBacklight(true);
      delay(5000);
      printUptime();
      lcd.setBacklight(false);
      prevTemp = curTemp;
    }       
}

void printUptime() {

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
    delay(5000);
    lcd.clear();
}

void printTemp() {

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    curTemp = t;
   
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      lcd.setCursor(0,0);
      lcd.println("DHT sensor error!");
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    lcd.setCursor(0,0);
    lcd.print("Hum : ");
    lcd.print(h);
    lcd.print(" %");
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" *C");
  
}

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();

    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  pinMode(2,INPUT_PULLUP);
  }

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  
  printTemp(); // just get humidity and temp data  
  tempChangeNotifier(); //checking temp changing

  //Checking btn state
  // if the button state has changed:
  int reading = digitalRead(btnPin);
  if (reading != btnState) {

    btnState = reading;
    backlightState = !backlightState;
    lcd.setBacklight(backlightState);
    delay(5000);
    printUptime();


    
  }
      
  
}
