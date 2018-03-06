/////////////////////////////////////////////////////////////////////
//
//  infinityPV_INA233_Setting_Calibration.ino
//
//  This is an example for using the LiBAT5V2A Energy Management shield    
//  from infinityPV with Arduino.                                    
//  The board contains 2 INA233 Analog to I2C converters             
//  to monitor energy input and output in the application:                      
//    IC2. Energy input to the battery 
//    IC1. Energy output to the application (5V output)
// 
//  The INA233 is able to monitor voltage, current, power and 
//  accumulated power (energy) at the measurement point.
//  
//  Here we show how to use the function setCalibration()
//  implemented in the library infintyPV_INA233.h
//  This function is needed to set the internal calibration 
//  in INA233 for a correct measurement of the current and 
//  power.
//  We have tried to make the calibration easy for the user, 
//  who has only to introduce the value of the shunt resistor (Ohms)
//  and the maximum expected current (A). Introducing these 2 values
//  the function calculates the optimun value to load into the 
//  CALIBRATION register of the INA233.
//
//  The function setCalibration() must be called in the setup area,
//  before starts to measure in the loop area. 
//
//  The function shows also the outputs from the calibration, 
//  which are for advanced users who want to know the exact
//  calibration values, which follow the PMBUS telemetry 
//  standard. For more detailed info check the INA233 datasheet. 
//
//  Finally, we implemented an extra output called Set_ERROR which returns 1, 
//  when the calculated value for CALIBRATION is out of range,
//  i.e. >65535. In this case the calibration is not valid, not loaded and 
//  you should change the shunt_resistor or the max. expected current values until
//  the CALIBRATION value is in range and Set_ERROR returns 0. 
//
// (*) This example uses the infinityPV_INA233 library, which licensed 
// under BSD license. 
//
//////////////////////////////////////////////////////////////////////


#include <Wire.h>
#include <infinityPV_INA233.h>

//I2C Adresses for the 2 INA233 on board 
INA233 IC1(0x45);  // IC1 address 0x45 1000101 
INA233 IC2(0x44);  // IC2 address 0x44 1000100 

void setup() {
  //values to be introduced by the user
  float R_shunt=0.05;
  float I_max=2;

  //variables to catch the outputs from set_Calibration()
  uint16_t CAL=0;
  int16_t m_c=0;
  int16_t m_p=0;
  int8_t R_c=0;
  int8_t R_p=0;
  uint8_t Set_ERROR=0;
  float Current_LSB=0;
  float Power_LSB=0;
  //variable to check the loaded calibration
  uint16_t Read_CAL=0;
  
  Serial.begin(9600);
  Serial.println("Hej!");
  Serial.println("Setting INA233 calibration for current and power measurements.");
  
  IC1.begin();
  CAL=IC1.setCalibration(R_shunt,I_max,&Current_LSB,&Power_LSB,&m_c,&R_c,&m_p,&R_p,&Set_ERROR);

  Serial.println(" ");
  Serial.print("Shunt Resistor IC1:   "); Serial.print(R_shunt);Serial.println(" Ohms");
  Serial.print("Max. expected current IC1:   "); Serial.print(I_max);Serial.println(" A");
  Serial.print("Current LSB IC1:   "); Serial.print(Current_LSB);Serial.println(" uA");
  Serial.print("Power LSB IC1:   "); Serial.print(Power_LSB);Serial.println(" mW");
  Serial.print("CAL IC1:   "); Serial.println(CAL);
  Serial.print("m_c IC1:   "); Serial.println(m_c);
  Serial.print("R_c IC1:   "); Serial.println(R_c);
  Serial.print("m_p IC1:   "); Serial.println(m_p);
  Serial.print("R_p IC1:   "); Serial.println(R_p);
  if (Set_ERROR==1)
  {
    Serial.println("CALIBRATION OUT of RANGE, choose a different R_shunt or I_max");
  }
  else
  {
   IC1.wireReadWord(MFR_CALIBRATION, &Read_CAL);
   Serial.print("CAL read from IC1:   ");Serial.println(Read_CAL);;
   Serial.println("CALIBRATION OK"); 
  }
}

void loop() {
//place your code here
  
}
