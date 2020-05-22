################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/USART\ /Src/stm32f4xx_hal_usart.c 

OBJS += \
./Drivers/USART\ /Src/stm32f4xx_hal_usart.o 

C_DEPS += \
./Drivers/USART\ /Src/stm32f4xx_hal_usart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/USART\ /Src/stm32f4xx_hal_usart.o: ../Drivers/USART\ /Src/stm32f4xx_hal_usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/USART /Src/stm32f4xx_hal_usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

