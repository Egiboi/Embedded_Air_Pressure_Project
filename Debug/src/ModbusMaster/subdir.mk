################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModbusMaster/LpcUart.cpp \
../src/ModbusMaster/ModbusMaster.cpp \
../src/ModbusMaster/ModbusRegister.cpp \
../src/ModbusMaster/SerialPort.cpp 

C_SRCS += \
../src/ModbusMaster/retarget_itm.c 

OBJS += \
./src/ModbusMaster/LpcUart.o \
./src/ModbusMaster/ModbusMaster.o \
./src/ModbusMaster/ModbusRegister.o \
./src/ModbusMaster/SerialPort.o \
./src/ModbusMaster/retarget_itm.o 

CPP_DEPS += \
./src/ModbusMaster/LpcUart.d \
./src/ModbusMaster/ModbusMaster.d \
./src/ModbusMaster/ModbusRegister.d \
./src/ModbusMaster/SerialPort.d 

C_DEPS += \
./src/ModbusMaster/retarget_itm.d 


# Each subdirectory must supply rules for building sources it contributes
src/ModbusMaster/%.o: ../src/ModbusMaster/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ModbusMaster/%.o: ../src/ModbusMaster/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\danie\Documents\MCUXpressoIDE_11.1.0_3209\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


