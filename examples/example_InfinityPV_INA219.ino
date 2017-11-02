//////////////////////////////////////////////////////////////////////////
//                                                                          
// This is an example for using the Multiplatform LiBAT5V2A board   
// from infinityPV with Arduino.                                    
// The board contains 3 INA219 Analog to I2C converters             
// to measure current and voltage at 3 points:                      
// IC2. Charging battery 
// IC3. Battery to 5V2A Converter Input (IC3)
// IC5. 5V2A Converter Output to your load (Arduino shields) 
//
// The example allows you to configure and calibrate each INA219
// according to the maximum current you want to measure (i_max),
// the maximum voltage (v_max) and the shunt resistor (r_shunt)
// you place in each point. 
// These values must be introduced in  2 functions during the setup: 
// - setCalibration(r_shunt, i_max) 
// - setSettings(r_shunt, i_max, v_max)
// The return of these functions are the Calibration and Configuration
// register for each INA219. But you do not have to worry about them,
// only if you are a pro and want to go more in details. Check the INA
// datasheet for info about them. 
//
// Once calibrated you just have to use the functions:
// getShuntVoltage_mV(), getBusVoltage_V() and getCurrent_mA() to get
// the measured values. 
//
// (*) The example uses the InfinityPV_INA219 library, which is 
// a modification of the Adafruit_INA219 library, which licensed 
// under BSD license. 
//
//////////////////////////////////////////////////////////////////////////////


#include <Wire.h>
#include <InfinityPV_INA219.h>

//I2C Adresses for the 3 INA219 on board 
INA219 IC2(0x41);  // IC2 address 0x41 1000001 
INA219 IC3(0x44);  // IC3 address 0x44 1000100 
INA219 IC5(0x45);  // IC5 address 0x45 1000101 

void setup(void) 
{
  uint16_t IC2_configRegister=0;
  uint32_t IC2_calValue=0;
  uint16_t IC3_configRegister=0;
  uint32_t IC3_calValue=0;
  uint16_t IC5_configRegister=0;
  uint32_t IC5_calValue=0;
  Serial.begin(115200);
  Serial.println("Hej!");
  
  Serial.println("Measuring voltage and current with INA219 ...");
  IC2.begin();
  IC2_calValue=IC2.setCalibration(0.05,1);
  IC2_configRegister=IC2.setSettings(0.05,1,5);
  IC3.begin();
  IC3_calValue=IC3.setCalibration(0.05,1);
  IC3_configRegister=IC3.setSettings(0.05,1,5);
  IC5.begin();
  IC5_calValue=IC5.setCalibration(0.01,2);
  IC5_configRegister=IC5.setSettings(0.01,2,5);
}

void loop(void) 
{
  float IC2_shuntvoltage = 0;
  float IC2_busvoltage = 0;
  float IC2_current_mA = 0;
  float IC2_loadvoltage = 0;
  float IC3_shuntvoltage = 0;
  float IC3_busvoltage = 0;
  float IC3_current_mA = 0;
  float IC3_loadvoltage = 0;
  float IC5_shuntvoltage = 0;
  float IC5_busvoltage = 0;
  float IC5_current_mA = 0;
  float IC5_loadvoltage = 0;
  
  IC2_shuntvoltage = IC2.getShuntVoltage_mV();
  IC2_busvoltage = IC2.getBusVoltage_V();
  IC2_current_mA = IC2.getCurrent_mA();
  IC2_loadvoltage = IC2_busvoltage + (IC2_shuntvoltage / 1000);

  IC3_shuntvoltage = IC3.getShuntVoltage_mV();
  IC3_busvoltage = IC3.getBusVoltage_V();
  IC3_current_mA = IC3.getCurrent_mA();
  IC3_loadvoltage = IC3_busvoltage + (IC3_shuntvoltage / 1000);

  IC5_shuntvoltage = IC5.getShuntVoltage_mV();
  IC5_busvoltage = IC5.getBusVoltage_V();
  IC5_current_mA = IC5.getCurrent_mA();
  IC5_loadvoltage = IC5_busvoltage + (IC5_shuntvoltage / 1000);
  
  Serial.print("Bus Voltage IC2:   "); Serial.print(IC2_busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage IC2: "); Serial.print(IC2_shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage IC2:  "); Serial.print(IC2_loadvoltage); Serial.println(" V");
  Serial.print("Current IC2:       "); Serial.print(IC2_current_mA); Serial.println(" mA"); 
  Serial.println("");

  Serial.print("Bus Voltage IC3:   "); Serial.print(IC3_busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage IC3: "); Serial.print(IC3_shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage IC3:  "); Serial.print(IC3_loadvoltage); Serial.println(" V");
  Serial.print("Current IC3:       "); Serial.print(IC3_current_mA); Serial.println(" mA");
  Serial.println("");

  Serial.print("Bus Voltage IC5:   "); Serial.print(IC5_busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage IC5: "); Serial.print(IC5_shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage IC5:  "); Serial.print(IC5_loadvoltage); Serial.println(" V");
  Serial.print("Current IC5:       "); Serial.print(IC5_current_mA); Serial.println(" mA"); 
  Serial.println("");

  delay(2000);
}
