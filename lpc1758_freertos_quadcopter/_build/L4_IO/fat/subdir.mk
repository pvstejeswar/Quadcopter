################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L4_IO/fat/fatfs_time.c \
../L4_IO/fat/ff.c 

OBJS += \
./L4_IO/fat/fatfs_time.o \
./L4_IO/fat/ff.o 

C_DEPS += \
./L4_IO/fat/fatfs_time.d \
./L4_IO/fat/ff.d 


# Each subdirectory must supply rules for building sources it contributes
L4_IO/fat/%.o: ../L4_IO/fat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\newlib" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L0_LowLevel" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS\include" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L1_FreeRTOS\portable" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L2_Drivers" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L2_Drivers\base" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L3_Utils" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L3_Utils\tlm" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO\fat" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L4_IO\wireless" -I"C:\Users\Dheeraj\git\Quadcopter-surveillance\lpc1758_freertos_quadcopter\L5_Application" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


