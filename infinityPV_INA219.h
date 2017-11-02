/**************************************************************************/
/*!
    @file     infinityPV_INA219.h
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

/*=========================================================================
    I2C ADDRESSES/BITS
    -----------------------------------------------------------------------*/
    #define INA219_ADDRESS_40                      (0x40)    // 1000000 (A1+A0=GND)
    #define INA219_ADDRESS_41                      (0x41)    // 1000001 (A1=GND, A0=VDD)
    #define INA219_ADDRESS_42                      (0x42)    // 1000010 (A1=GND, A0=SDA)
    #define INA219_ADDRESS_43                      (0x43)    // 1000011 (A1=GND, A0=SCL)
    #define INA219_ADDRESS_44                      (0x44)    // 1000100 (A1=VDD, A0=GND)
    #define INA219_ADDRESS_45                      (0x45)    // 1000101 (A1+A0=VDD)
    #define INA219_ADDRESS_46                      (0x46)    // 1000110 (A1=VDD, A0=SDA)
    #define INA219_ADDRESS_47                      (0x47)    // 1000111 (A1=VDD, A0=SCL)
    #define INA219_ADDRESS_48                      (0x48)    // 1001000 (A1=SDA, A0=GND)
    #define INA219_ADDRESS_49                      (0x49)    // 1001001 (A1=SDA, A0=VDD)
    #define INA219_ADDRESS_4A                      (0x4A)    // 1001010 (A1+A0=SDA)
    #define INA219_ADDRESS_4B                      (0x4B)    // 1001011 (A1=SDA, A0=SCL)
    #define INA219_ADDRESS_4C                      (0x4C)    // 1001100 (A1=SCL, A0=GND)
    #define INA219_ADDRESS_4D                      (0x4D)    // 1001101 (A1=SCL, A0=VDD)
    #define INA219_ADDRESS_4E                      (0x4E)    // 1001110 (A1=SCL, A0=SDA)
    #define INA219_ADDRESS_4F                      (0x4F)    // 1001111 (A1+A0=SCL)
    #define INA219_READ                            (0x01)
/*=========================================================================*/

/*=========================================================================
    // MASKS to CODE the CAL_OUTPUT (8 bits status variable)
    // -----------------------------------------------------------------------*/
    // //LSB bit 0 defines the VBUS_MAX
    // #define VBUS_MAX16                            (0x00)  // 00000000 Bit0=0 -> VBUS_MAX16
    // #define VBUS_MAX32                            (0x01)  // 00000001 Bit0=1 -> VBUS_MAX32
    // // bits 2-1 define the PGA for the V_Shunt_max
    // #define PGA_1                                 (0x00)  // 00000000 40mV scale (20mV with VBUS=16)
    // #define PGA_2                                 (0x02)  // 00000010 80mV scale (40mV with VBUS=16)
    // #define PGA_4                                 (0x04)  // 00000100 160mV scale (80mV with VBUS=16)
    // #define PGA_8                                 (0x06)  // 00000110 320mV scale (160mV with VBUS=16)
    // //bit 3 defines a error because the selected r_shunt and i_max lead to higher scale than possible with INA219
    // #define PGA_OUT                               (0x08)  // 00001000 out of scale
    // //MSB bit 7 defines if the max voltage to measure is higher than the maximum allowed in INA219 (26V)
    // #define V_MAX_HIGH26                          (0x80)  // 10000000



/*=========================================================================*//*=========================================================================

/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CONFIG                      (0x00)
    /*---------------------------------------------------------------------*/
    #define INA219_CONFIG_RESET                    (0x8000)  // Reset Bit

    #define INA219_CONFIG_BVOLTAGERANGE_MASK       (0x2000)  // Bus Voltage Range Mask
    #define INA219_CONFIG_BVOLTAGERANGE_16V        (0x0000)  // 0-16V Range
    #define INA219_CONFIG_BVOLTAGERANGE_32V        (0x2000)  // 0-32V Range

    #define INA219_CONFIG_GAIN_MASK                (0x1800)  // Gain Mask
    #define INA219_CONFIG_GAIN_1_40MV              (0x0000)  // Gain 1, 40mV Range
    #define INA219_CONFIG_GAIN_2_80MV              (0x0800)  // Gain 2, 80mV Range
    #define INA219_CONFIG_GAIN_4_160MV             (0x1000)  // Gain 4, 160mV Range
    #define INA219_CONFIG_GAIN_8_320MV             (0x1800)  // Gain 8, 320mV Range

    #define INA219_CONFIG_BADCRES_MASK             (0x0780)  // Bus ADC Resolution Mask
    #define INA219_CONFIG_BADCRES_9BIT             (0x0080)  // 9-bit bus res = 0..511
    #define INA219_CONFIG_BADCRES_10BIT            (0x0100)  // 10-bit bus res = 0..1023
    #define INA219_CONFIG_BADCRES_11BIT            (0x0200)  // 11-bit bus res = 0..2047
    #define INA219_CONFIG_BADCRES_12BIT            (0x0400)  // 12-bit bus res = 0..4097

    #define INA219_CONFIG_SADCRES_MASK             (0x0078)  // Shunt ADC Resolution and Averaging Mask
    #define INA219_CONFIG_SADCRES_9BIT_1S_84US     (0x0000)  // 1 x 9-bit shunt sample
    #define INA219_CONFIG_SADCRES_10BIT_1S_148US   (0x0008)  // 1 x 10-bit shunt sample
    #define INA219_CONFIG_SADCRES_11BIT_1S_276US   (0x0010)  // 1 x 11-bit shunt sample
    #define INA219_CONFIG_SADCRES_12BIT_1S_532US   (0x0018)  // 1 x 12-bit shunt sample
    #define INA219_CONFIG_SADCRES_12BIT_2S_1060US  (0x0048)	 // 2 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_4S_2130US  (0x0050)  // 4 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_8S_4260US  (0x0058)  // 8 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_16S_8510US (0x0060)  // 16 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_32S_17MS   (0x0068)  // 32 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_64S_34MS   (0x0070)  // 64 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_128S_69MS  (0x0078)  // 128 x 12-bit shunt samples averaged together

    #define INA219_CONFIG_MODE_MASK                (0x0007)  // Operating Mode Mask
    #define INA219_CONFIG_MODE_POWERDOWN           (0x0000)
    #define INA219_CONFIG_MODE_SVOLT_TRIGGERED     (0x0001)
    #define INA219_CONFIG_MODE_BVOLT_TRIGGERED     (0x0002)
    #define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED (0x0003)
    #define INA219_CONFIG_MODE_ADCOFF              (0x0004)
    #define INA219_CONFIG_MODE_SVOLT_CONTINUOUS    (0x0005)
    #define INA219_CONFIG_MODE_BVOLT_CONTINUOUS    (0x0006)
    #define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)
/*=========================================================================*/

/*=========================================================================
    SHUNT VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_SHUNTVOLTAGE                (0x01)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_BUSVOLTAGE                  (0x02)
/*=========================================================================*/

/*=========================================================================
    POWER REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_POWER                       (0x03)
/*=========================================================================*/

/*=========================================================================
    CURRENT REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CURRENT                     (0x04)
/*=========================================================================*/

/*=========================================================================
    CALIBRATION REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CALIBRATION                 (0x05)
/*=========================================================================*/

class INA219{
 public:
  INA219(uint8_t addr = INA219_ADDRESS_45);
  void begin(void);
  float getBusVoltage_V(void);
  float getShuntVoltage_mV(void);
  float getCurrent_mA(void);
  int16_t getBusVoltage_raw(void);
  int16_t getShuntVoltage_raw(void);
  int16_t getCurrent_raw(void);
  uint16_t setSettings(float r_shunt, float i_max, float v_max);
  uint16_t setCalibration(float r_shunt, float i_max);

 private:
  uint8_t ina219_i2caddr;
  uint32_t ina219_calValue;
  // The following multipliers are used to convert raw current and power
  // values to mA and mW, taking into account the current config settings
  uint32_t ina219_currentDivider_mA;
  uint32_t ina219_powerDivider_mW;
  void wireWriteRegister(uint8_t reg, uint16_t value);
  void wireReadRegister(uint8_t reg, uint16_t *value);

};
