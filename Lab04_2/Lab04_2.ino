#include <Adafruit_I2CDevice.h>

#define PCF8574_Address 0x20

Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(PCF8574_Address);
uint8_t buffer[32];

void setup() {

  Serial.begin(9600);
  Serial.println("I2C address detection test");
  if (!i2c_dev.begin()) {
    Serial.print("Did not find device at 0x");
    Serial.println(i2c_dev.address(), HEX);
    while (1);
  }
  Serial.print("Device found on address 0x");
  Serial.println(i2c_dev.address(), HEX);

  // Config PCF8574
  buffer[0]=0b01000000;      // config all pins to 
  i2c_dev.write(buffer,1);   // send the config to PCF8574

}
void loop() {
  i2c_dev.read(buffer,1); 

  buffer[0] = (buffer[0]>>6 & 1) ? 0b00000000 : 0b10000000;

  i2c_dev.write(buffer,1); 

  delay(100);
}