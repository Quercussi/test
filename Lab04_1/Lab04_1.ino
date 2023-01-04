#include <Adafruit_PCF8574.h>

#define PCF8574_Address 0x20
#define PCFO 7
#define SDA 21
#define SCL 22

Adafruit_PCF8574 i2c_dev = Adafruit_I2CDevice(PCF8574_Address);

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit PCF8574 LED blink test");
  if(!pcf.begin(PCF8574_Address, &Wire))
    Serial.println("Couldn't find PCF8574");
  pcf.pinMode(PCFO, OUTPUT);
}

void loop() {
  pcf.digitalWrite(PCFO, LOW);  // turn LED on by sinking current to ground
  delay(1000);
  pcf.digitalWrite(PCFO, HIGH); // turn LED off by turning off sinking transistor
  delay(1000);
}
