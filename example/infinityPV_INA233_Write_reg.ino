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
// We show here how to write the INA233 registers through the I2C
// bus, using PMBUS compatible commands. 
// We have implemented 3 different functions to cover  the 
// possible kind of writings to INA233 :send 1 byte (just the PMBUS command)
// write 1 byte or 1 word:
//   -wireSendCmd("PMBUS_COMMAND")
//   -wireWriteByte("PMBUS_COMMAND", data8)
//   -wireWriteWord("PMBUS_COMMAND", data16)
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
 Serial.println("Writing and reading calibration register from INA233 ...");
 IC1.begin();
 IC2.begin();
 uint16_t value16=0;
 uint8_t value8=0;
 uint8_t data8=0x06;
 uint16_t data16=0x0034;
 
 //Example of sending just the command
 IC1.wireSendCmd(RESTORE_DEFAULT_ALL);
 
 IC1.wireReadByte(MFR_DEVICE_CONFIG,&value8);
 IC1.wireReadWord(MFR_CALIBRATION,&value16);
 Serial.print(" Default Config IC1:   "); Serial.println(value8,HEX);
 Serial.print(" Default Calibration IC1:   "); Serial.println(value16,HEX);
 
 //Sending a byte and word
 IC1.wireWriteByte(MFR_DEVICE_CONFIG, data8);
 IC1.wireWriteWord(MFR_CALIBRATION, data16);
 
 //checking that the new values are loaded to the registers
 IC1.wireReadByte(MFR_DEVICE_CONFIG,&value8);
 IC1.wireReadWord(MFR_CALIBRATION,&value16);
 Serial.print(" New Config IC1:   "); Serial.println(value8,HEX);
 Serial.print(" New Calibration IC1:   "); Serial.println(value16,HEX); 
 
}

void loop() 
{
// place your code here
}
