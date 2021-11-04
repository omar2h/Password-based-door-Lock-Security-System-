################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECUAL/dc_motor.c \
../ECUAL/external_eeprom.c \
../ECUAL/lcd.c 

OBJS += \
./ECUAL/dc_motor.o \
./ECUAL/external_eeprom.o \
./ECUAL/lcd.o 

C_DEPS += \
./ECUAL/dc_motor.d \
./ECUAL/external_eeprom.d \
./ECUAL/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
ECUAL/%.o: ../ECUAL/%.c ECUAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


