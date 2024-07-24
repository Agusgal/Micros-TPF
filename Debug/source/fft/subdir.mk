################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/fft/vumeter.c 

C_DEPS += \
./source/fft/vumeter.d 

OBJS += \
./source/fft/vumeter.o 


# Each subdirectory must supply rules for building sources it contributes
source/fft/%.o: ../source/fft/%.c source/fft/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/equalizer" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/CMSIS" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/id3" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/lists" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/helix" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs/fsl_sd_disk" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/fft" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/device" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\features\include" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\redlib\include" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/drivers" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/drivers/HAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/drivers/MCAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/drivers/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/FSM" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/mp3_file_handler" -I"/TPFinal/source/mp3_handler" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/board" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-fft

clean-source-2f-fft:
	-$(RM) ./source/fft/vumeter.d ./source/fft/vumeter.o

.PHONY: clean-source-2f-fft

