#include <Wire.h>
//wire library

/* 
 Sourced from
 http://www.instructables.com/id/Thermostat-Microcontroller-with-an-Arduino-and-a-T/step4/TC74-Arduino-code/
 and modified a bit!
 */

//works better when you connect SDA and SCLK

#define address B1001010
#define addressDAC B1000001
#define addressADC B0110100
#define baudrate 9600
//baudrate for communication

void setVoltage(float voltage)
{
  int voltageInInt = ((voltage*100.0)/11.5);
  Wire.beginTransmission(addressDAC);
  Wire.write(0x30);
  Wire.write(voltageInInt);
  Wire.write(0x00);
  Wire.endTransmission();  
}

float getVoltage()
{
  Wire.beginTransmission(addressADC);
  Wire.write(0xB9);  //B1 channel 2 B9 channel 3
  Wire.write(0x80);
  Wire.endTransmission(false);

  Wire.requestFrom(addressADC, 3);
  while (Wire.available() < 3) ;
  
  delay(200);
  
  Wire.requestFrom(addressADC, 3);
  while (Wire.available() < 3) ;
  
  unsigned long int  value = (Wire.read()-0x80);
  value = value << 16;
  value += ((long int)Wire.read() << 8);
  value += Wire.read();
  value = value >> 6;
  float test = ((float)value / 65536.0 * 2.5);
  
  //calculate voltage over 4.7K, calculate current through 4.7K, use to calc voltage over 39K
  float voltageAfterCorrection = (5-(test * 2));
  long int currentR19 = (( voltageAfterCorrection  * 10000) / 47);
  long int voltagemv = currentR19 * 39;
  test = voltagemv - ((test * 2)*1000);
  test = test / 1000;
  
  Serial.print("Voltage measured: ");
  Serial.print(test);
  Serial.println("V");

  return test;
}

void setCurrent(float current)
{
  float voltageToSet = current * 3.33;
  int voltageInInt = voltageToSet * 51;
  Wire.beginTransmission(addressDAC);
  Wire.write(0x31);
  Wire.write(voltageInInt);
  Wire.write(0x00);
  Wire.endTransmission();  
}

float getCurrent()
{
  Wire.beginTransmission(addressADC);
  Wire.write(0xB1);  //B1 channel 2 B9 channel 3
  Wire.write(0x80);  //Why fix the measurement amp whe you can just turn on the gain?
  Wire.endTransmission(false);

  Wire.requestFrom(addressADC, 3);
  while (Wire.available() < 3) ;

  
  delay(200);
  
  Wire.requestFrom(addressADC, 3);
  while (Wire.available() < 3) ;
  
  long unsigned int value = (Wire.read()-0x80);
  value = value << 16;
  value += ((long int)Wire.read() << 8);
  value += Wire.read();
  value = value >> 6;
  float test = ((float)value / 65536.0 * 2.5);
  test = test * 1000;
  if(test <= 18)
  {
    test = 0;
  }
  else
  {
    test = test - 18;
  }
  test = test / 5.8;
  test = test / 0.22;
  
  Serial.print("Current measured: ");
  Serial.print(test);
  Serial.println("mA");

  return test;
}

byte val = 0;
byte voltage = 0;
void setup()
{
  Wire.begin();
  Serial.begin(baudrate);
}

void loop()
{
  Serial.print("temperature in Celsius: ");
  //let's signal we're about to do something

  int temperature;
  //temperature in a byte

    Wire.beginTransmission(address);
  //start the transmission

  Wire.write(val);

  Wire.requestFrom(address, 1);
  if (Wire.available()) {
    temperature = Wire.read();
    Serial.println(temperature);
  }

  else {
    Serial.println("---");
  }

  delay(1);

  Serial.println("Setting voltage to: 12.5");
  setVoltage(12.5);

  delay(1);
  Serial.println("Setting current to: 100mA");
  setCurrent(0.1);

  delay(1);
  getVoltage();
  delay(200);
  getCurrent();
  
  Serial.println();
  delay(2000);
}

