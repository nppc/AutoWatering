#ifndef INC_PGM_H_
#define INC_PGM_H_

#include "main.h"

/****** EEPROM MAPP ******/
#define EE_CALIBDATA	0x1E00
#define EE_CRC			0x1E00+sizeof(eeprom_t)
//#define EE_CODELASTADDR	0x1FFF	// last address of the code flash

extern uint8_t FlashKey1, FlashKey2;

#define flkey_prepare	FlashKey1 = 0xA5; FlashKey2=0xF1;
#define flkey_clear		FlashKey1 = 0x00; FlashKey2=0x00;


void defaultSettingsEE(void);
void storeSettingsEE(void);
void loadSettingsEE(void);

#endif /* INC_PGM_H_ */
