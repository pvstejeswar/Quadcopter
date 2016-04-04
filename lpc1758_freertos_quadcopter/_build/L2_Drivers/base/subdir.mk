################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L2_Drivers/base/char_dev.cpp \
../L2_Drivers/base/i2c_base.cpp \
../L2_Drivers/base/uart_dev.cpp 

OBJS += \
./L2_Drivers/base/char_dev.o \
./L2_Drivers/base/i2c_base.o \
./L2_Drivers/base/uart_dev.o 

CPP_DEPS += \
./L2_Drivers/base/char_dev.d \
./L2_Drivers/base/i2c_base.d \
./L2_Drivers/base/uart_dev.d 


# Each subdirectory must supply rules for building sources it contributes
L2_Drivers/base/%.o: ../L2_Drivers/base/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\newlib" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L0_LowLevel" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS\include" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS\portable" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L2_Drivers" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L2_Drivers\base" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L3_Utils" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L3_Utils\tlm" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO\fat" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO\wireless" -I"C:\QuadcopterMasterProject\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L5_Application" -std=gnu++11 -fabi-version=0 -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

