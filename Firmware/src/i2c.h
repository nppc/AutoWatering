/***************************************************************************************************
                                    ExploreEmbedded Copyright Notice
****************************************************************************************************
 * File:   stdutils.h
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains function prototypes for I2c routines.

This code has been developed and tested on ExploreEmbedded boards.
We strongly believe that the library works on any of development boards for respective controllers.
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider buying the ExploreEmbedded boards.

The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php

EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION
RELATED TO UPDATES.


Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies
and that both those copyright notices and this permission notice appear in supporting documentation.
***************************************************************************************************/




/***************************************************************************************************
                             Revision History
****************************************************************************************************
15.0: Initial version
***************************************************************************************************/

#ifndef _I2C_H
#define _I2C_H

#include "main.h"

/***************************************************************************************************
                               SCL and SDA pin configuration
***************************************************************************************************/
SI_SBIT(SCL_PIN, SFR_P1, 4); //P+
SI_SBIT(SDA_PIN, SFR_P1, 5); //P-

//#define i2c_delay 1	// speed of I2C
//#define SCL_Direction SCL_PIN
//#define SDA_Direction SDA_PIN
/**************************************************************************************************/






/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t v_i2cData_u8);
uint8_t I2C_Read(uint8_t v_ackOption_u8);
//bool i2cByteOut(uint8_t b);
bool i2cBegin(uint8_t addr, bool bRead);

/**************************************************************************************************/

#endif
