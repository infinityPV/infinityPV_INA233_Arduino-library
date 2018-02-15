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
  
 

}

void loop() 
{
 uint16_t value=0;
 
 IC1.wireSendByte(RESTORE_DEFAULT_ALL);
 IC1.wireReadWord(MFR_CALIBRATION,&value);
 Serial.print(" Default Calibration IC1:   "); Serial.println(value,HEX);
 IC1.wireWriteWord(MFR_CALIBRATION, 0x0034);
 IC1.wireReadWord(MFR_CALIBRATION,&value);
 Serial.print(" New Calibration IC1:   "); Serial.println(value,HEX);
 delay(2000);
}
