################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DigitalIoPinMaster_class/DigitalIoPin.cpp 

OBJS += \
./src/DigitalIoPinMaster_class/DigitalIoPin.o 

CPP_DEPS += \
./src/DigitalIoPinMaster_class/DigitalIoPin.d 


# Each subdirectory must supply rules for building sources it contributes
src/DigitalIoPinMaster_class/%.o: ../src/DigitalIoPinMaster_class/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


