################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_MCU/MCAL/gpio.c \
../HMI_MCU/MCAL/timer.c \
../HMI_MCU/MCAL/uart.c 

OBJS += \
./HMI_MCU/MCAL/gpio.o \
./HMI_MCU/MCAL/timer.o \
./HMI_MCU/MCAL/uart.o 

C_DEPS += \
./HMI_MCU/MCAL/gpio.d \
./HMI_MCU/MCAL/timer.d \
./HMI_MCU/MCAL/uart.d 


# Each subdirectory must supply rules for building sources it contributes
HMI_MCU/MCAL/%.o: ../HMI_MCU/MCAL/%.c HMI_MCU/MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


