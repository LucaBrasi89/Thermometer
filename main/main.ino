// Simple humidity/temperature deviсe. With push btn and LCD1602.
// Based on Arduino Nano.
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


void tempChangeNotifier(int curTemp){
//    Serial.println("Cur temp:" + String(curTemp));
//    Serial.println("Prev temp:" + String(prevTemp) + "\n");
   if (prevTemp != curTemp) {
      lcd.setBacklight(true);
      delay(10000);
      lcd.setBacklight(false);
      prevTemp = curTemp;
    }       
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

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
 
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

  //checking temp changing
  tempChangeNotifier(int(t));

  //Checking btn state
  // if the button state has changed:
  int reading = digitalRead(btnPin);
  if (reading != btnState) {

    btnState = reading;
    backlightState = !backlightState;
    lcd.setBacklight(backlightState);
    delay(15000);
  }
      
  
}
