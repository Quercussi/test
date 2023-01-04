// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void lcdDisplay(DateTime time){
    lcd.setCursor(0, 0); // Or setting the cursor in the desired position.
    lcd.print(time.year());
    lcd.setCursor(4, 0); // Or setting the cursor in the desired position.
    lcd.print('/');
    lcd.setCursor(5, 0); // Or setting the cursor in the desired position.
    lcd.print(time.month());
    lcd.setCursor(7, 0); // Or setting the cursor in the desired position.
    lcd.print('/');
    lcd.setCursor(8, 0); // Or setting the cursor in the desired position.
    lcd.print(time.day());

    lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
    lcd.print(time.hour());
    lcd.setCursor(2, 1); // Or setting the cursor in the desired position.
    lcd.print(':');
    lcd.setCursor(3, 1); // Or setting the cursor in the desired position.
    lcd.print(time.minute());
    lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd.print(':');
    lcd.setCursor(6, 1); // Or setting the cursor in the desired position.
    lcd.print(time.second());
}

void setup () {
  Serial.begin(9600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!i2c_dev.begin()) {
    Serial.print("Did not find device at 0x");
    Serial.println(i2c_dev.address(), HEX);
    while (1);
  }
  Serial.print("Device found on address 0x");
  Serial.println(i2c_dev.address(), HEX);

  // Config PCF8574
  // buffer[0]=0b00000000;      // config all pins to 
  // i2c_dev.write(buffer,1);   // send the config to PCF8574

  lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                 // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, and 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    lcdDisplay(now);

    delay(500);
}