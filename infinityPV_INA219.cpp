/**************************************************************************/
/*!
  @file     infinityPV_INA219.cpp
  @author   rava (infinityPV ApS)
	@license  BSD (see license.txt)

	This is a library for all the infinityPV boards using the ADC/I2C converter
  INA219:
  - 1. Multiplatform LiBAT5V2A Boost
  ----> https://infinitypv.com/products/electronics

	InfinityPV makes printed organic solar cells. We invests part of our
  time and resources providing open source examples (hardware and code) to
  make easier the use of our solar cells in your projects.
	Printed organic solar cells are flexible, rollable and super light unlike
  other solar cell technologies, please visit our website to find more !

  (*)This libray is a modified version of Adafruit_INA219 library which is
  protected by BSD license, Copyright (c) 2012, Adafruit Industries
  All rights reserved
	@section  HISTORY

    v1.0  - First release Sep 2017
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "infinityPV_INA219.h"

/**************************************************************************/
/*!
    @brief  Sends a single command byte over I2C
*/
/**************************************************************************/
void INA219::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(ina219_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
    Wire.write(value & 0xFF);              // Lower 8-bits
  #else
    Wire.send(reg);                        // Register
    Wire.send(value >> 8);                 // Upper 8-bits
    Wire.send(value & 0xFF);               // Lower 8-bits
  #endif
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
void INA219::wireReadRegister(uint8_t reg, uint16_t *value)
{

  Wire.beginTransmission(ina219_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();

  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(ina219_i2caddr, (uint8_t)2);
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

/**************************************************************************/
/*!
    @brief  Set INA219 Config register for measuring based on the user's inputs
    r_shunt, i_max, and v_max.
    The function tries to find the best calibration for INA219 for the given
    i_max, r_shunt and v_max.
    It evaluates the v_shunt_max=v_bus_max/100/(8/pga) for the different PGA (1, 2, 4, 8) and V_bus_max (16,32)
    options, with the expected v_shunt_max_exp=i_max*r_shunt
    INPUTS:
    - r_shunt, is the value of the shunt resistor in Ohms
    - i_max, is the maximum expected current to measure in A.
    - v_max, is the maximum expected voltage to measure in V.
    */
/**************************************************************************/
uint16_t INA219::setSettings(float r_shunt, float i_max, float v_max)
//float INA219::setCalibration(float r_shunt, float i_max, float v_max)
{
  float v_shunt_max_exp;          //variable for calculated maximum expected voltage on the Rshunt for this calibration
  //Set config register, values for BADC, SADC and SAND registers are set to
  // BADC : 12-bit bus res = 0..4097
  // SADC : 1 x 12-bit shunt sample
  // SAND : measuring continuosly shunt and bus voltage.
  uint16_t config =   INA219_CONFIG_BADCRES_12BIT |
                     INA219_CONFIG_SADCRES_12BIT_1S_532US |
                     INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  //Values for PGA and VBUSMAX are set according to the user's inputs
  v_shunt_max_exp=r_shunt*i_max;
 //if (v_max > 26)
if (v_max > 16) //Evaluation of v_shunt_max for different pga with v_bus_max=32.
  {
    if (v_shunt_max_exp <= 0.04)  //Evaluate with v_shun_max=0.04 (32V and for pga=1)
      {
      config=config | INA219_CONFIG_GAIN_1_40MV;
      config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
      }
    else
      {
      if (v_shunt_max_exp <= 0.08)  //Evaluate with v_shun_max=0.08 (32V and for pga=2)
        {
        config=config | INA219_CONFIG_GAIN_2_80MV;
        config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
        }
      else
        {
        if (v_shunt_max_exp <= 0.16)  //Evaluate with v_shun_max=0.16 (32V and for pga=4)
          {
          config=config | INA219_CONFIG_GAIN_4_160MV;
          config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
          }
        else
          {
          if (v_shunt_max_exp <= 0.32)  //Evaluate with v_shun_max=0.32 (32V and for pga=8)
            {
            config=config | INA219_CONFIG_GAIN_8_320MV;
            config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
            }
          else
            {
            //config=config | PGA_OUT;
            config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
            }
          }
        }
      }
  }
else //Evaluation of v_shunt_max for different pga with v_bus_max=16.
{
  if (v_shunt_max_exp <= 0.02)  //Evaluate with v_shun_max=0.02 (16V and for pga=1)
    {
    config=config | INA219_CONFIG_GAIN_1_40MV;
    config=config | INA219_CONFIG_BVOLTAGERANGE_16V;
    }
  else
    {
    if (v_shunt_max_exp <= 0.04)  //Evaluate with v_shun_max=0.04 (16V and for pga=2)
      {
      config=config | INA219_CONFIG_GAIN_2_80MV;
      config=config | INA219_CONFIG_BVOLTAGERANGE_16V;
      }
    else
      {
      if (v_shunt_max_exp <= 0.08)  //Evaluate with v_shun_max=0.08 (16V and for pga=4)
        {
        config=config | INA219_CONFIG_GAIN_4_160MV;
        config=config | INA219_CONFIG_BVOLTAGERANGE_16V;
        }
      else
        {
        if (v_shunt_max_exp <= 0.16)  //Evaluate with v_shun_max=0.16 (16V and for pga=8)
          {
          config=config | INA219_CONFIG_GAIN_8_320MV;
          config=config | INA219_CONFIG_BVOLTAGERANGE_16V;
          }
        else //In case out of scale with VBUS=16V then evaluate for VBUS=32V
          {
            if (v_shunt_max_exp <= 0.04)  //Evaluate with v_shun_max=0.04 (32V and for pga=1)
              {
              config=config | INA219_CONFIG_GAIN_1_40MV;
              config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
              }
            else
              {
              if (v_shunt_max_exp <= 0.08)  //Evaluate with v_shun_max=0.08 (32V and for pga=2)
                {
                config=config | INA219_CONFIG_GAIN_2_80MV;
                config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
                }
              else
                {
                if (v_shunt_max_exp <= 0.16)  //Evaluate with v_shun_max=0.16 (32V and for pga=4)
                  {
                  config=config | INA219_CONFIG_GAIN_4_160MV;
                  config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
                  }
                else
                  {
                  if (v_shunt_max_exp <= 0.32)  //Evaluate with v_shun_max=0.32 (32V and for pga=8)
                    {
                    config=config | INA219_CONFIG_GAIN_8_320MV;
                    config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
                    }
                  else
                    {
                    //config=config | PGA_OUT;
                    config=config | INA219_CONFIG_BVOLTAGERANGE_32V;
                    }
                  }
                }
              }
          }
        }
      }
    }
}
wireWriteRegister(INA219_REG_CONFIG, config);
return(uint16_t)config;
}

/**************************************************************************/
/*!
    @brief  Set INA219 Calibration register for measuring based on the user's
    inputs r_shunt and i_max.

    */
/**************************************************************************/
uint16_t INA219::setCalibration(float r_shunt,float i_max)
{
  float Current_LSB;
  //float Power_LSB;
  float Cal;

  // Minimum current measured (uA per LSB)
  Current_LSB = i_max*1000000/32768;

  // Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  Current_LSB = ceil(Current_LSB);

  // Compute the calibration register
   Cal = trunc(40960 / (Current_LSB * r_shunt));

  //Cast to uint16_t
  ina219_calValue = uint32_t(Cal);

  // Calculate the power LSB (uW)
  //PowerLSB = 20 * CurrentLSB

  // Compute and check the maximum current value before overflow
  // Max_Current = Current_LSB * 32767
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If

  // Compute and check the maximum shunt voltage value before overflow
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX


  // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = uint32_t (1000/Current_LSB);
    //ina219_powerDivider_mW = uint32_t (1000/Power_LSB);

  // Set Calibration register to 'ina219_calValue' calculated above
  wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);
return(uint16_t)ina219_calValue;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new INA219 class
*/
/**************************************************************************/
INA219::INA219(uint8_t addr) {
  ina219_i2caddr = addr;
  ina219_currentDivider_mA = 0;
  ina219_powerDivider_mW = 0;
}

/**************************************************************************/
/*!
    @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void INA219::begin() {
  Wire.begin();
  // Set chip to known config values to start
  //ina219SetCalibration_32V_2A();
}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA219::getBusVoltage_raw() {
  uint16_t value;
  wireReadRegister(INA219_REG_BUSVOLTAGE, &value);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)((value >> 3) * 4);
}

/**************************************************************************/
/*!
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA219::getShuntVoltage_raw() {
  uint16_t value;
  wireReadRegister(INA219_REG_SHUNTVOLTAGE, &value);
  return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw current value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA219::getCurrent_raw() {
  uint16_t value;

  // Sometimes a sharp load will reset the INA219, which will
  // reset the cal register, meaning CURRENT and POWER will
  // not be available ... avoid this by always setting a cal
  // value even if it's an unfortunate extra step
  wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

  // Now we can safely read the CURRENT register!
  wireReadRegister(INA219_REG_CURRENT, &value);

  return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in mV (so +-327mV)
*/
/**************************************************************************/
float INA219::getShuntVoltage_mV() {
  int16_t value;
  value = getShuntVoltage_raw();
  return value * 0.01;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
float INA219::getBusVoltage_V() {
  int16_t value = getBusVoltage_raw();
  return value * 0.001;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
*/
/**************************************************************************/
float INA219::getCurrent_mA() {
  float valueDec = getCurrent_raw();
  valueDec /= ina219_currentDivider_mA;
  return valueDec;
}
