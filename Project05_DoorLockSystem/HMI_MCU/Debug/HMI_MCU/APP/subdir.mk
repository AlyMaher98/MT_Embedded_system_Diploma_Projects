################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_MCU/APP/HMI_MCU.c 

OBJS += \
./HMI_MCU/APP/HMI_MCU.o 

C_DEPS += \
./HMI_MCU/APP/HMI_MCU.d 


# Each subdirectory must supply rules for building sources it contributes
HMI_MCU/APP/%.o: ../HMI_MCU/APP/%.c HMI_MCU/APP/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


