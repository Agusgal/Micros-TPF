################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/SDK/clock_config.c \
../drivers/SDK/fsl_clock.c \
../drivers/SDK/fsl_common.c \
../drivers/SDK/fsl_dac.c \
../drivers/SDK/fsl_dmamux.c \
../drivers/SDK/fsl_dspi.c \
../drivers/SDK/fsl_edma.c \
../drivers/SDK/fsl_i2c.c \
../drivers/SDK/fsl_pdb.c \
../drivers/SDK/fsl_pmc.c \
../drivers/SDK/fsl_rcm.c \
../drivers/SDK/fsl_rtc.c \
../drivers/SDK/fsl_smc.c \
../drivers/SDK/fsl_sysmpu.c \
../drivers/SDK/power_mode_switch.c 

OBJS += \
./drivers/SDK/clock_config.o \
./drivers/SDK/fsl_clock.o \
./drivers/SDK/fsl_common.o \
./drivers/SDK/fsl_dac.o \
./drivers/SDK/fsl_dmamux.o \
./drivers/SDK/fsl_dspi.o \
./drivers/SDK/fsl_edma.o \
./drivers/SDK/fsl_i2c.o \
./drivers/SDK/fsl_pdb.o \
./drivers/SDK/fsl_pmc.o \
./drivers/SDK/fsl_rcm.o \
./drivers/SDK/fsl_rtc.o \
./drivers/SDK/fsl_smc.o \
./drivers/SDK/fsl_sysmpu.o \
./drivers/SDK/power_mode_switch.o 

C_DEPS += \
./drivers/SDK/clock_config.d \
./drivers/SDK/fsl_clock.d \
./drivers/SDK/fsl_common.d \
./drivers/SDK/fsl_dac.d \
./drivers/SDK/fsl_dmamux.d \
./drivers/SDK/fsl_dspi.d \
./drivers/SDK/fsl_edma.d \
./drivers/SDK/fsl_i2c.d \
./drivers/SDK/fsl_pdb.d \
./drivers/SDK/fsl_pmc.d \
./drivers/SDK/fsl_rcm.d \
./drivers/SDK/fsl_rtc.d \
./drivers/SDK/fsl_smc.d \
./drivers/SDK/fsl_sysmpu.d \
./drivers/SDK/power_mode_switch.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/SDK/%.o: ../drivers/SDK/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\equalizer" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\CMSIS" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\startup" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\HAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\MCAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\SDK" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\id3" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\lists" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\helix" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs\fsl_sd_disk" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\board" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\fft" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\device" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\features\include" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\redlib\include" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


