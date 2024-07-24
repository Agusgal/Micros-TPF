################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/vumeter/subdir.mk
-include source/mp3_handler/subdir.mk
-include source/mp3_file_handler/subdir.mk
-include source/equalizer/subdir.mk
-include source/drivers/SDK/subdir.mk
-include source/drivers/MCAL/subdir.mk
-include source/drivers/HAL/subdir.mk
-include source/FSM/States/subdir.mk
-include source/FSM/subdir.mk
-include source/EventQueue/subdir.mk
-include source/subdir.mk
-include device/subdir.mk
-include component/lists/subdir.mk
-include component/id3/subdir.mk
-include component/helix/real/subdir.mk
-include component/helix/subdir.mk
-include component/fatfs/fsl_sd_disk/subdir.mk
-include component/fatfs/subdir.mk
-include component/SD/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TPFinal.axf

# Tool invocations
TPFinal.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\libs" -Xlinker -Map="TPFinal.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T TPFinal_Debug.ld -o "TPFinal.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TPFinal.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TPFinal.axf"; # arm-none-eabi-objcopy -v -O binary "TPFinal.axf" "TPFinal.bin" ; # checksum -p MK64FN1M0xxx12 -d "TPFinal.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
