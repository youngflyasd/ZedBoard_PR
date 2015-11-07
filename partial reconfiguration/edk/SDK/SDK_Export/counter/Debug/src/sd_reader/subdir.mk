################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sd_reader/ff.c \
../src/sd_reader/mmc.c 

OBJS += \
./src/sd_reader/ff.o \
./src/sd_reader/mmc.o 

C_DEPS += \
./src/sd_reader/ff.d \
./src/sd_reader/mmc.d 


# Each subdirectory must supply rules for building sources it contributes
src/sd_reader/%.o: ../src/sd_reader/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../counter_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


