################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/USART\ /Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/NRF24L01/Src/subdir.mk
-include Drivers/I2C/Src/subdir.mk
-include Drivers/DMA/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
TemperatureLogger.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
TemperatureLogger.list \

OBJCOPY_BIN += \
TemperatureLogger.bin \


# All Target
all: TemperatureLogger.elf secondary-outputs

# Tool invocations
TemperatureLogger.elf: $(OBJS) $(USER_OBJS) /Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "TemperatureLogger.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/huzaifaasif/STM32CubeIDE/workspace_1.0.2/TemperatureLogger/STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="TemperatureLogger.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

TemperatureLogger.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "TemperatureLogger.list"
	@echo 'Finished building: $@'
	@echo ' '

TemperatureLogger.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "TemperatureLogger.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
