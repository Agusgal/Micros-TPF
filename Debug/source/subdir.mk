################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/App_test.c 

OBJS += \
./source/App.o \
./source/App_test.o 

C_DEPS += \
./source/App.d \
./source/App_test.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\equalizer" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\CMSIS" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\startup" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\id3" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\lists" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\helix" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs\fsl_sd_disk" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\device" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\features\include" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\redlib\include" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\drivers" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\HAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\MCAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\SDK" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\FSM" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\mp3_file_handler" -I"/TPFinal/source/mp3_handler" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\board" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


