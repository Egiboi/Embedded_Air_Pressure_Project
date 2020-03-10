################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/i2c_class/subdir.mk
-include src/ModbusMaster/subdir.mk
-include src/GUI_class/subdir.mk
-include src/DigitalIoPinMaster_class/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Ilmapaine_Projekti.axf

# Tool invocations
Ilmapaine_Projekti.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\Debug" -L"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\Debug" -Xlinker -Map="Ilmapaine_Projekti.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Ilmapaine_Projekti_Debug.ld" -o "Ilmapaine_Projekti.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) Ilmapaine_Projekti.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Ilmapaine_Projekti.axf"; # arm-none-eabi-objcopy -v -O binary "Ilmapaine_Projekti.axf" "Ilmapaine_Projekti.bin" ; # checksum -p LPC1549 -d "Ilmapaine_Projekti.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
