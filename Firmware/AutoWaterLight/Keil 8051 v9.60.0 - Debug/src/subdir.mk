################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
A51_UPPER_SRCS += \
C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/SILABS_STARTUP.A51 

C_SRCS += \
../src/InitDevice.c \
../src/Interrupts.c \
../src/SSD1306.c \
../src/button.c \
../src/gen.c \
../src/i2c.c \
../src/main.c \
../src/pgm.c \
../src/pwm.c \
../src/screensaver.c 

OBJS += \
./src/InitDevice.OBJ \
./src/Interrupts.OBJ \
./src/SILABS_STARTUP.OBJ \
./src/SSD1306.OBJ \
./src/button.OBJ \
./src/gen.OBJ \
./src/i2c.OBJ \
./src/main.OBJ \
./src/pgm.OBJ \
./src/pwm.OBJ \
./src/screensaver.OBJ 


# Each subdirectory must supply rules for building sources it contributes
src/%.OBJ: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/InitDevice.OBJ: C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/InitDevice.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/Interrupts.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/gen.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/button.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/SSD1306.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/SILABS_STARTUP.OBJ: C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/SILABS_STARTUP.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/SSD1306.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/SSD1306.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/i2c.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/button.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/button.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/gen.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/gen.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/SSD1306.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/i2c.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/i2c.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/main.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/InitDevice.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/gen.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/i2c.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/SSD1306.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/button.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/pgm.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/bitmaps.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/screensaver.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/pwm.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/pgm.OBJ: C:/SiliconLabs/SimplicityStudio/v5/developer/toolchains/keil_8051/9.60/INC/ABSACC.H C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/pgm.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/gen.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/pwm.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/pwm.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h

src/screensaver.OBJ: C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/main.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/screensaver.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/SSD1306.h C:/src/Tennp/Git/AutoWatering/Firmware/AutoWaterLight/src/i2c.h C:/SiliconLabs/SimplicityStudio/v5/developer/toolchains/keil_8051/9.60/INC/STDLIB.H C:/SiliconLabs/SimplicityStudio/v5/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/EFM8BB1/inc/SI_EFM8BB1_Defs.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/8051/v4.2.0/Device/shared/si8051Base/stdbool.h


