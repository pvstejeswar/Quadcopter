################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L4_IO/fat/option/ccsbcs.c \
../L4_IO/fat/option/reentrant.c 

OBJS += \
./L4_IO/fat/option/ccsbcs.o \
./L4_IO/fat/option/reentrant.o 

C_DEPS += \
./L4_IO/fat/option/ccsbcs.d \
./L4_IO/fat/option/reentrant.d 


# Each subdirectory must supply rules for building sources it contributes
L4_IO/fat/option/%.o: ../L4_IO/fat/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\newlib" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L0_LowLevel" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L1_FreeRTOS" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L1_FreeRTOS\include" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L1_FreeRTOS\portable" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L2_Drivers" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L2_Drivers\base" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L3_Utils" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L3_Utils\tlm" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L4_IO" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L4_IO\fat" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L4_IO\wireless" -I"C:\QuadcopterMasterProject\MasterProject\lpc1758_freertos_quadcopter\L5_Application" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


