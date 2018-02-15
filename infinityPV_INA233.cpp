/**************************************************************************/
/*!
  @file     infinityPV_INA233.cpp
  @author   rava (infinityPV ApS)
	@license  BSD (see license.txt)

	This is a library for all the infinityPV boards using the ADC/I2C converter
  INA233:
  - 1. LiBAT5V2A Energy Management Shield
  ----> https://infinitypv.com/products/electronics/libat5v2a

	InfinityPV makes printed organic solar cells. We invests part of our
  time and resources providing open source examples (hardware and code) to
  make easier the use of our solar cells in your projects.
	Printed organic solar cells are flexible, rollable and super light unlike
  other solar cell technologies, please visit our website to find more !

  (*)This libray is protected by BSD license, Copyright (c) 2018, infinityPV ApS
  All rights reserved
	@section  HISTORY

    v1.0  - First release Feb 2018
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "infinityPV_INA233.h"

/**************************************************************************/
/*!
    @brief  Sends a single command byte over I2C
*/
/**************************************************************************/
void INA233::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(ina233_i2caddr);
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
    @brief  Writes a byte over I2C, no data are sent, only the
    PMBus comand (reg).
*/
/**************************************************************************/
void INA233::wireSendByte(uint8_t reg)
{
  Wire.beginTransmission(ina233_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // PMBus command
  #else
    Wire.send(reg);                        // PMBus command
  #endif
  Wire.endTransmission();
}
/**************************************************************************/
/*!
    @brief  Writes a word (value) to the specified register
    by the PMBus comand (reg) over I2C
*/
/**************************************************************************/
void INA233::wireWriteWord (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(ina233_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // PMBus command
    Wire.write(value & 0xFF);              // Lower 8-bits
    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
  #else
    Wire.send(reg);                        // PMBus command
    Wire.send(value & 0xFF);               // Lower 8-bits
    Wire.send(value >> 8);                 // Upper 8-bits
  #endif
  Wire.endTransmission();
}
/**************************************************************************/
/*!
    @brief  Reads a Block of 6 bytes from the INA233 over I2C
    When reading a block in PMBUS the first byte from the slave is the
    block size (6 in this case), so the request must be for block_size+1
*/
/**************************************************************************/
void INA233::wireReadBlock(uint8_t reg, uint8_t value[6])
{
  int i;
  uint8_t block_size;
  Wire.requestFrom(ina233_i2caddr,(uint8_t)7,reg,(uint8_t)1,(uint8_t)true);
  block_size=Wire.read();
  for (i=0;i<block_size;i++)
  {
    value[i]=Wire.read();
  }
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value from the INA233 over I2C
*/
/**************************************************************************/
void INA233::wireReadWord(uint8_t reg, uint16_t *value)
{
  Wire.requestFrom(ina233_i2caddr,(uint8_t)2,reg,(uint8_t)1,(uint8_t)true);
  *value = Wire.read();
  *value=((Wire.read() << 8) | *value);
}
/**************************************************************************/
/*!
    @brief  Reads a 8 bit value from the INA233 over I2C
*/
/**************************************************************************/
void INA233::wireReadByte(uint8_t reg, uint8_t *value)
{
  Wire.requestFrom(ina233_i2caddr,(uint8_t)1,reg,(uint8_t)1,(uint8_t)true);
  *value = Wire.read();
}


/**************************************************************************/
/*!
    @brief  Set INA233 Config register for measuring based on the user's inputs
    r_shunt, i_max, and v_max.
    The function tries to find the best calibration for INA233 for the given
    i_max, r_shunt and v_max.
    It evaluates the v_shunt_max=v_bus_max/100/(8/pga) for the different PGA (1, 2, 4, 8) and V_bus_max (16,32)
    options, with the expected v_shunt_max_exp=i_max*r_shunt
    INPUTS:
    - r_shunt, is the value of the shunt resistor in Ohms
    - i_max, is the maximum expected current to measure in A.
    - v_max, is the maximum expected voltage to measure in V.
    */
/**************************************************************************/
uint16_t INA233::setSettings(float r_shunt, float i_max, float v_max)
{

  //TODO
return(uint16_t)0xFF;
}

/**************************************************************************/
/*!
    @brief  Set INA233 Calibration register for measuring based on the user's
    inputs r_shunt and i_max.

    */
/**************************************************************************/
uint16_t INA233::setCalibration(float r_shunt,float i_max)
{
  //TODO
return(uint16_t)0xFF;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new INA233 class
*/
/**************************************************************************/
INA233::INA233(uint8_t addr) {
  ina233_i2caddr = addr;
  ina233_currentDivider_mA = 0;
  ina233_powerDivider_mW = 0;
}

/**************************************************************************/
/*!
    @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void INA233::begin() {
  Wire.begin();
}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA233::getBusVoltage_raw() {
  uint16_t value;
  wireReadRegister(INA233_REG_BUSVOLTAGE, &value);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)((value >> 3) * 4);
}

/**************************************************************************/
/*!
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA233::getShuntVoltage_raw() {
  uint16_t value;
  wireReadRegister(INA233_REG_SHUNTVOLTAGE, &value);
  return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw current value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t INA233::getCurrent_raw() {
  uint16_t value;

  // Sometimes a sharp load will reset the INA233, which will
  // reset the cal register, meaning CURRENT and POWER will
  // not be available ... avoid this by always setting a cal
  // value even if it's an unfortunate extra step
  wireWriteRegister(INA233_REG_CALIBRATION, ina233_calValue);

  // Now we can safely read the CURRENT register!
  wireReadRegister(INA233_REG_CURRENT, &value);

  return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in mV (so +-327mV)
*/
/**************************************************************************/
float INA233::getShuntVoltage_mV() {
  int16_t value;
  value = getShuntVoltage_raw();
  return value * 0.01;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
float INA233::getBusVoltage_V() {
  int16_t value = getBusVoltage_raw();
  return value * 0.001;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
*/
/**************************************************************************/
float INA233::getCurrent_mA() {
  float valueDec = getCurrent_raw();
  valueDec /= ina233_currentDivider_mA;
  return valueDec;
}
