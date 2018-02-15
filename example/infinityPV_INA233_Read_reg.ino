//////////////////////////////////////////////////////////////////////////
// 
// This is an example for using the LiBAT5V2A Energy Management shield    
// from infinityPV with Arduino.                                    
// The board contains 2 INA233 Analog to I2C converters             
// to monitor energy input and output in the application:                      
// IC2. Energy input to the battery 
// IC1. Energy output to the application (5V output)
// 
// The INA233 is able to monitor voltage, current, power and 
// accumulated power (energy) at the measurement point. 
//
// We show here how to read the INA233 registers through the I2C
// bus, using PMBUS compatible commands. 
// We have implemented 3 different functions to cover all the 
// possible kind of readings from INA233 :1 byte, 1 word (2bytes) or 
// a block of 6 bytes:
//   -wireReadByte("PMBUS_COMMAND", &r_data8)
//   -wireReadWord("PMBUS_COMMAND", &r_data16)
//   -wireReadBlock("PMBUS_COMMAND",r_dataB)
//
//  The available PMBus Commands can be checked on the INA233 datasheet
//  or in the file infinityPV_INA233.h
//
// (*) The example uses the infinityPV_INA233 library, which licensed 
// under BSD license. 
//
//////////////////////////////////////////////////////////////////////////////


#include <Wire.h>
#include <infinityPV_INA233.h>

//I2C Adresses for the 2 INA233 on board 
INA233 IC1(0x45);  // IC1 address 0x45 1000101 
INA233 IC2(0x44);  // IC2 address 0x44 1000100 

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hej!");
  
  Serial.println("Reading registers from with INA233 ...");
  IC1.begin();
  IC2.begin();
  
}

void loop(void) 
{
  uint16_t r_data16=0;
  uint8_t r_data8=0;
  uint8_t r_dataB[6];

  IC1.wireReadWord(MFR_ADC_CONFIG, &r_data16);
  IC2.wireReadByte(MFR_DEVICE_CONFIG, &r_data8);
  IC2.wireReadBlock(MFR_MODEL,r_dataB);
    
  Serial.print(" Read Word from IC1:   "); Serial.print(r_data16,HEX);Serial.println("h");
  Serial.print(" Read Byte from IC2:   "); Serial.print(r_data8,HEX);Serial.println("h");
  Serial.print(" Read Block from IC2:   "); 
  Serial.print(r_dataB[0],HEX);Serial.print("h, ");
  Serial.print(r_dataB[1],HEX);Serial.print("h, ");
  Serial.print(r_dataB[2],HEX);Serial.print("h, ");
  Serial.print(r_dataB[3],HEX);Serial.print("h, ");
  Serial.print(r_dataB[4],HEX);Serial.print("h, ");
  Serial.print(r_dataB[5],HEX);Serial.println("h, ");
  delay(2000);
}
