/**************************************************************************/
/*!
    @file     infinityPV_INA233.h
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
  v1.0  - First release Mar 2018
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
    #define INA233_ADDRESS_40                      (0x40)    // 1000000 (A1+A0=GND)
    #define INA233_ADDRESS_41                      (0x41)    // 1000001 (A1=GND, A0=VDD)
    #define INA233_ADDRESS_42                      (0x42)    // 1000010 (A1=GND, A0=SDA)
    #define INA233_ADDRESS_43                      (0x43)    // 1000011 (A1=GND, A0=SCL)
    #define INA233_ADDRESS_44                      (0x44)    // 1000100 (A1=VDD, A0=GND)
    #define INA233_ADDRESS_45                      (0x45)    // 1000101 (A1+A0=VDD)
    #define INA233_ADDRESS_46                      (0x46)    // 1000110 (A1=VDD, A0=SDA)
    #define INA233_ADDRESS_47                      (0x47)    // 1000111 (A1=VDD, A0=SCL)
    #define INA233_ADDRESS_48                      (0x48)    // 1001000 (A1=SDA, A0=GND)
    #define INA233_ADDRESS_49                      (0x49)    // 1001001 (A1=SDA, A0=VDD)
    #define INA233_ADDRESS_4A                      (0x4A)    // 1001010 (A1+A0=SDA)
    #define INA233_ADDRESS_4B                      (0x4B)    // 1001011 (A1=SDA, A0=SCL)
    #define INA233_ADDRESS_4C                      (0x4C)    // 1001100 (A1=SCL, A0=GND)
    #define INA233_ADDRESS_4D                      (0x4D)    // 1001101 (A1=SCL, A0=VDD)
    #define INA233_ADDRESS_4E                      (0x4E)    // 1001110 (A1=SCL, A0=SDA)
    #define INA233_ADDRESS_4F                      (0x4F)    // 1001111 (A1+A0=SCL)
/*=========================================================================*/

/*=====================================================================================================================
    PMBus Commands
    	    NAME				CODE 	FUNCTION	R/W	Number of Data Bytes 	Default value
     -----------------------------------------------------------------------------------------------------------------*/

    #define CLEAR_FAULTS			(0x03)	//Clears the status registers and rearms the black box registers for udpating SB 0 N/A
    #define RESTORE_DEFAULT_ALL			(0x12)	//Restores internal registers to the default values 			SB 0 N/A
    #define CAPABILITY				(0x19)	//Retrieves the device capability					R 1 xB0
    #define IOUT_OC_WARN_LIMIT			(0x4A)	//Retrieves or stores the output overcurrent warn limit threshold  	R/W 2 x7FF8
    #define VIN_OV_WARN_LIMIT			(0x57)  //Retrieves or stores the input overvoltage warn limit threshold   	R/W 2 x7FF8
    #define VIN_UV_WARN_LIMIT			(0x58)  //Retrieves or stores the input undervoltage warn limit threshold  	R/W 2 x0000
    #define PIN_OP_WARN_LIMIT			(0x6B)  //Retrieves or stores the output overpower warn limit threshold    	R/W 2 x7FF8
    #define STATUS_BYTE				(0x78)  //Retrieves information about the device operating status          	R 1 x00
    #define STATUS_WORD				(0x79)  //Retrieves information about the device operating status          	R 2 x1000
    #define STATUS_IOUT				(0x7B)  //Retrieves information about the output current status          	R/W,CLR 1 x00
    #define STATUS_INPUT			(0x7C)  //Retrieves information about the input status          	        R/W,CLR 1 x00
    #define STATUS_CML				(0x7E)  //Retrieves information about the communications status          	R/W,CLR 1 x00
    #define STATUS_MFR_SPECIFIC			(0x80)  //Retrieves information about the manufacturer specific device status   R/W,CLR 1 x20
    #define READ_EIN				(0x86)	//Retrieves the energy reading measurement				Block_R 6 6 bytes at x00
    #define READ_VIN				(0x88)	//Retrieves the measurement for the VBUS voltage			R 2 0x0000
    #define READ_IIN				(0x89)	//Retrieves the input current measurement, supports both + and - currents R 2 0x0000
    #define READ_VOUT				(0x8B)	//Mirrors READ_VIN							R 2 0x0000
    #define READ_IOUT				(0x8C)	//Mirrors READ_IIN for compatibility 					R 2 0x0000
    #define READ_POUT				(0x96)	//Mirrors READ_PIN for compatibility with possible VBUS connections	R 2 0x0000
    #define READ_PIN				(0x97)	//Retrieves the input power measurement					R 2 0x0000
    #define MFR_ID				(0x99)	//Retrieves the manufacturer ID in ASCII Characters (TI)		Block_R 2 0x54, 0x49
    #define MFR_MODEL				(0x9A)	//Retrieves the device number in ASCII Characters (INA233)		Block_R 6 0x49, 0x4E, 0x41, 0x32, 0x33, 0x33
    #define MFR_REVISION			(0x9B)	//Retrieves the device revision letter and number in ASCII (e.g.A0)	R 2 0x41,0x30
    #define MFR_ADC_CONFIG			(0xD0)	//Configures the ADC averaging modes, conversion times, and opr. modes  R/W 2 0x4127
    #define MFR_READ_VSHUNT			(0xD1)	//Retrieves the shunt voltage measurement				R 2 0x0000
    #define MFR_ALERT_MASK			(0xD2)	//Allows masking of device warnings					R/W 1 0xF0
    #define MFR_CALIBRATION			(0xD4)	//Allows the value of the current-sense resistor calibration value to be input.
						                            //Must be programed at power-up. Default value is set to 1. 		R/W 2 0x0001
    #define MFR_DEVICE_CONFIG			(0xD5)	//Allows the ALERT pin polarity to be changed				R/W 1 0x02
    #define CLEAR_EIN				(0xD6)	//Clears the energy accumulator						S_B 0 N/A
    #define TI_MFR_ID				(0xE0)	//Returns a unique word for the manufacturer ID				R 2 0x5449 (TI in ASCII)
    #define TI_MFR_MODEL			(0xE1)	//Returns a unique word for the manufacturer model			R 2 33 in ASCII
    #define TI_MFR_REVISION			(0xE2)	//Returns a unique word for the manufacturer revision			R 2 A0 in ASCII

/*=========================================================================*//*=========================================================================


/*=========================================================================
    SHUNT VOLTAGE TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define m_vs                (4)
    #define R_vs                (5)
    #define b_vs                (0)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define m_vb                (8)
    #define R_vb                (2)
    #define b_vb                (0)
/*=========================================================================*/

/*=========================================================================
    CURRENT & POWER CONSTANT TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define b_c                (0)
    #define b_p                (0)
/*=========================================================================*/


class INA233{
 public:
  INA233(uint8_t addr = INA233_ADDRESS_45);
  void begin(void);
  float getBusVoltage_V(void);
  float getShuntVoltage_mV(void);
  float getCurrent_mA(void);
  float getPower_mW(void);
  float getAv_Power_mW(void);
  int16_t getBusVoltage_raw(void);
  int16_t getShuntVoltage_raw(void);
  int16_t getCurrent_raw(void);
  int16_t getPower_raw(void);
  void getEnergy_raw(uint16_t* accumulator, uint8_t* roll_over, uint32_t* sample_count);
  uint16_t setCalibration(float r_shunt,float i_max,float *Current_LSB,float *Power_LSB, int16_t *m_c,int8_t *R_c, int16_t *m_p, int8_t *R_p,  uint8_t *ERROR);
  void wireReadWord(uint8_t reg, uint16_t *value);
  void wireReadByte(uint8_t reg, uint8_t *value);
  void wireReadBlock(uint8_t reg, uint8_t value[6]);
  void wireWriteWord(uint8_t reg, uint16_t value);
  void wireWriteByte (uint8_t reg, uint8_t value);
  void wireSendCmd(uint8_t reg);

 private:
  uint8_t ina233_i2caddr;
  uint32_t ina233_calValue;
  // The following coefficients are used to convert raw current and power
  // values to mA and mW, taking into account the current config settings
  int16_t m_c;
  int8_t R_c;
  int16_t m_p;
  int8_t R_p;
};
