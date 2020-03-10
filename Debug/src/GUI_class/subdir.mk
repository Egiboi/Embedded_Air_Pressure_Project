################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GUI_class/DecimalEdit.cpp \
../src/GUI_class/IntegerEdit.cpp \
../src/GUI_class/LiquidCrystal.cpp \
../src/GUI_class/MenuItem.cpp \
../src/GUI_class/SimpleMenu.cpp 

C_SRCS += \
../src/GUI_class/ITM_write.c 

OBJS += \
./src/GUI_class/DecimalEdit.o \
./src/GUI_class/ITM_write.o \
./src/GUI_class/IntegerEdit.o \
./src/GUI_class/LiquidCrystal.o \
./src/GUI_class/MenuItem.o \
./src/GUI_class/SimpleMenu.o 

CPP_DEPS += \
./src/GUI_class/DecimalEdit.d \
./src/GUI_class/IntegerEdit.d \
./src/GUI_class/LiquidCrystal.d \
./src/GUI_class/MenuItem.d \
./src/GUI_class/SimpleMenu.d 

C_DEPS += \
./src/GUI_class/ITM_write.d 


# Each subdirectory must supply rules for building sources it contributes
src/GUI_class/%.o: ../src/GUI_class/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/GUI_class/%.o: ../src/GUI_class/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


