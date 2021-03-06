#ifndef INC_PGM_H_
#define INC_PGM_H_

#include "main.h"

/****** EEPROM MAP ******/
#define EE_DATA     0xFB80
#define EE_CRC      0xFB80+sizeof(eeprom_t)

extern uint8_t FlashKey1, FlashKey2;

#define flkey_prepare	FlashKey1 = 0xA5; FlashKey2=0xF1;
#define flkey_clear		FlashKey1 = 0x00; FlashKey2=0x00;


void defaultSettingsEE(void);
void storeSettingsEE(void);
void loadSettingsEE(void);

#endif /* INC_PGM_H_ */
