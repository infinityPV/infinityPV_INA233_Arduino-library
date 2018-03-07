/////////////////////////////////////////////////////////////////////
//
//  infinityPV_INA233_Read_Energy.ino
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
//  Here we show how to measure the averaged power and consumed energy 
//  from the accumulated power measurement using the functions 
//  implemented in the library infinityPV_INA233.h.
//
//  The accumulated power measurement is what does the INA233 a 
//  real energy monitor, since it is measuring even when you are not
//  reading, so when you read you get the real consumption during 
//  the period, not just the instantaneous reading values.  
//
//  As always in the setup area we use the setCalibration() function
//  for each ones of the INA233 on board. 
//  This is needed in all the application which need to 
//  measure current or/and power. Check the example 
//  infinityPV_INA233_Setting_Calibration.ino for info. 
//  
//  INA233 contains an accumulator which add the measured power every ADC sampling
//  There are different methods for reading properly this accumulator, but we use 
//  here the one we thing is simplest and most accurate. 
//  We configure INA233 to continuos ADC sampling (by default) and to clear the
//  accumulator after each reading.
//  Take a look to section 7.6.2.14 in the INA233 datasheet is you want more info.  
// 
//  We have implemented:
//  - getAv_Power_mW(), which returns the value (float) of averaged consumed power 
//  from the accumulator since the last energy reading. 
//
//  In this example we read the average consumed power in the loop every 1 second.
//  So if we sum the av_power (mW) the energy units will be (mW*s), translating to Wh
//  needs /1000 and /3600. Taking into account this if you want to change the period
//  in your loop. 
//     
//
// (*) The example uses the infinityPV_INA233 library, which licensed 
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
  float R_shunt_IC1=0.05;   //call RS2 on the board 
  float I_max_IC1=2;
  float R_shunt_IC2=0.05;   //call RS1 on the board 
  float I_max_IC2=2;

  //variables to catch the outputs from set_Calibration()
  uint16_t CAL=0;
  int16_t m_c=0;
  int16_t m_p=0;
  int8_t R_c=0;
  int8_t R_p=0;
  uint8_t Set_ERROR=0;
  uint8_t r_data8=0;
  float Current_LSB=0;
  float Power_LSB=0;
  //variable to check the loaded calibration
  uint16_t Read_CAL=0;
  
  Serial.begin(9600);
  Serial.println("Hej!");
  Serial.println("Measuring instantaneous values from IC1 and IC2:");
  IC1.begin();
  IC2.begin();

  // Call calibration function for each INA233 (Check infinityPV_INA233_Setting_Calibration.ino 
  // if you want to check the details about calibration.
  CAL=IC1.setCalibration(R_shunt_IC1,I_max_IC1,&Current_LSB,&Power_LSB,&m_c,&R_c,&m_p,&R_p,&Set_ERROR);
  CAL=IC2.setCalibration(R_shunt_IC2,I_max_IC2,&Current_LSB,&Power_LSB,&m_c,&R_c,&m_p,&R_p,&Set_ERROR);

  //Configuring the devices for clearing the accumulator after every reading
  //Check INA233 Datasheet section 7.6.2.14 for more info.
  IC2.wireWriteByte (MFR_DEVICE_CONFIG, 0x06);
  IC1.wireWriteByte (MFR_DEVICE_CONFIG, 0x06);
 
}

void loop() {
  float av_power_IC1=0;
  float av_power_IC2=0;
  static float Energy_IC1_Wh=0;
  static float Energy_IC2_Wh=0;
  
  av_power_IC1=IC1.getAv_Power_mW();
  Energy_IC1_Wh=Energy_IC1_Wh+av_power_IC1/(3600L*1000L);
  av_power_IC2=IC2.getAv_Power_mW();
  Energy_IC2_Wh=Energy_IC2_Wh+av_power_IC2/(3600L*1000L);
  Serial.print(" AvPower IC1:   "); Serial.print(av_power_IC1);Serial.println(" mW");
  Serial.print(" Accumulated Energy IC1:   "); Serial.print(Energy_IC1_Wh);Serial.println(" Wh");
  Serial.print(" AvPower IC2:   "); Serial.print(av_power_IC2);Serial.println(" mW");
  Serial.print(" Accumulated Energy IC2:   "); Serial.print(Energy_IC2_Wh);Serial.println(" Wh");
  

  delay(1000);
}
