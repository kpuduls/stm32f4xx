################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpioa_led.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart_dma.c 

OBJS += \
./Src/gpioa_led.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart_dma.o 

C_DEPS += \
./Src/gpioa_led.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart_dma.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32 -DSTM32F4 -c -I../Inc -I"C:/Users/pudul/OneDrive/Documents/STM32F4 - bare metal docs/STM32F4-BareMetal_Workspace/F4_chip_headers/CMSIS/Include" -I"C:/Users/pudul/OneDrive/Documents/STM32F4 - bare metal docs/STM32F4-BareMetal_Workspace/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gpioa_led.cyclo ./Src/gpioa_led.d ./Src/gpioa_led.o ./Src/gpioa_led.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart_dma.cyclo ./Src/uart_dma.d ./Src/uart_dma.o ./Src/uart_dma.su

.PHONY: clean-Src

