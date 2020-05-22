################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/mcp9808.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/usart_app.c 

OBJS += \
./Src/main.o \
./Src/mcp9808.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/usart_app.o 

C_DEPS += \
./Src/main.d \
./Src/mcp9808.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/usart_app.d 


# Each subdirectory must supply rules for building sources it contributes
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/mcp9808.o: ../Src/mcp9808.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/mcp9808.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_msp.o: ../Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_it.o: ../Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f4xx.o: ../Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usart_app.o: ../Src/usart_app.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/DMA/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/I2C/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Drivers/USART /Inc" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Config" -I"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/Third party/FreeRTOS/org/Source/include" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usart_app.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
