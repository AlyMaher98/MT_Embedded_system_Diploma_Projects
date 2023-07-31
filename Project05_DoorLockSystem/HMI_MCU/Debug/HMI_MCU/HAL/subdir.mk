################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_MCU/HAL/keypad.c \
../HMI_MCU/HAL/lcd.c 

OBJS += \
./HMI_MCU/HAL/keypad.o \
./HMI_MCU/HAL/lcd.o 

C_DEPS += \
./HMI_MCU/HAL/keypad.d \
./HMI_MCU/HAL/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
HMI_MCU/HAL/%.o: ../HMI_MCU/HAL/%.c HMI_MCU/HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


