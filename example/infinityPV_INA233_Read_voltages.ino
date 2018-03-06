//////////////////////////////////////////////////////////////////////////
// 
// infinityPV_INA233_Read_voltages.ino
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
// We show here how to read the instantaneous voltage measured by. 
// each INA233 at the board, i.e. IC2 measures the battery voltage
// and IC1 measures the output voltage.
// We have implemented a simple function to make it straight forward:
//   -getBusVoltage_V()
// The function returns a float containing the measured voltage in V.  
//
// We have also implemented a another function to read easily the 
// value of voltage read on the shunt resistor, in case this value is needed:
//    -getShuntVoltage_mV()
// The function returns a float containing the measured shunt voltage in mV. 
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

void setup() {
  Serial.begin(9600);
  Serial.println("Hej!");
  
  Serial.println("Measuring voltages at IC1 and IC2 INA233 at LiBAT5V2A Energy Management Shield");
  IC1.begin();
  IC2.begin();
}

void loop() {
  float voltage_IC1=0;
  float voltage_IC2=0;
  float shunt_voltage_IC1=0;
  float shunt_voltage_IC2=0;

  voltage_IC1=IC1.getBusVoltage_V();  //getting bus voltage (V) at INA233 IC1
  voltage_IC2=IC2.getBusVoltage_V();  //getting bus voltage (V) at INA233 IC2
  shunt_voltage_IC1=IC1.getShuntVoltage_mV();  //getting shunt voltage (mV) at INA233 IC1
  shunt_voltage_IC2=IC2.getShuntVoltage_mV();  //getting shunt voltage (mV) at INA233 IC2
                   
  
  Serial.print(" Battery voltage:   "); Serial.print(voltage_IC2);Serial.println(" V");
  Serial.print(" Output voltage:   "); Serial.print(voltage_IC1);Serial.println(" V");
  Serial.print(" Shunt voltage at IC1:   "); Serial.print(shunt_voltage_IC1);Serial.println(" mV");
  Serial.print(" Shunt voltage at IC2:   "); Serial.print(shunt_voltage_IC2);Serial.println(" mV");
  
  delay(2000);

}
