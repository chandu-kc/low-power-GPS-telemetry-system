################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/NETWORK_SUPPORT.c \
../Core/Src/Sleep_state.c \
../Core/Src/Waiting_to_Wake_up.c \
../Core/Src/gprs_machine.c \
../Core/Src/main.c \
../Core/Src/mqtt_machine_file.c \
../Core/Src/random.c \
../Core/Src/stm32l1xx_hal_msp.c \
../Core/Src/stm32l1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l1xx.c 

OBJS += \
./Core/Src/NETWORK_SUPPORT.o \
./Core/Src/Sleep_state.o \
./Core/Src/Waiting_to_Wake_up.o \
./Core/Src/gprs_machine.o \
./Core/Src/main.o \
./Core/Src/mqtt_machine_file.o \
./Core/Src/random.o \
./Core/Src/stm32l1xx_hal_msp.o \
./Core/Src/stm32l1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l1xx.o 

C_DEPS += \
./Core/Src/NETWORK_SUPPORT.d \
./Core/Src/Sleep_state.d \
./Core/Src/Waiting_to_Wake_up.d \
./Core/Src/gprs_machine.d \
./Core/Src/main.d \
./Core/Src/mqtt_machine_file.d \
./Core/Src/random.d \
./Core/Src/stm32l1xx_hal_msp.d \
./Core/Src/stm32l1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/NETWORK_SUPPORT.d ./Core/Src/NETWORK_SUPPORT.o ./Core/Src/Sleep_state.d ./Core/Src/Sleep_state.o ./Core/Src/Waiting_to_Wake_up.d ./Core/Src/Waiting_to_Wake_up.o ./Core/Src/gprs_machine.d ./Core/Src/gprs_machine.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/mqtt_machine_file.d ./Core/Src/mqtt_machine_file.o ./Core/Src/random.d ./Core/Src/random.o ./Core/Src/stm32l1xx_hal_msp.d ./Core/Src/stm32l1xx_hal_msp.o ./Core/Src/stm32l1xx_it.d ./Core/Src/stm32l1xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32l1xx.d ./Core/Src/system_stm32l1xx.o

.PHONY: clean-Core-2f-Src

