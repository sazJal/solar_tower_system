################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include DeviceOperation/operation/subdir.mk
-include DeviceOperation/interrupts/subdir.mk
-include DeviceOperation/GlobalVariable/subdir.mk
-include DeviceObject/rs232/subdir.mk
-include DeviceObject/pdu/subdir.mk
-include DeviceObject/pcs/subdir.mk
-include DeviceObject/miscellaneous/subdir.mk
-include DeviceObject/mathfunction/subdir.mk
-include DeviceObject/infotainment/subdir.mk
-include DeviceObject/geenix/subdir.mk
-include DeviceObject/esp32/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
SOLAR_TOWER.hex \

SECONDARY_LIST += \
SOLAR_TOWER.lst \

SECONDARY_SIZE += \
SOLAR_TOWER.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "SOLAR_TOWER.map"

TOOLCHAIN_ROOT=D:/Software/DAVE_4-4-2_64bit_2018-02-23/DAVE_4-4-2_64bit_2018-02-23/DAVE-IDE-4.4.2-64Bit/DAVE-IDE-4.4.2-64Bit/eclipse/ARM-GCC-49
PROJECT_LOC=C:/Users/Lenovo/workspace/dave_workspace/SOLAR_TOWER

# All Target
all: SOLAR_TOWER.elf secondary-outputs

# Tool invocations
SOLAR_TOWER.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,$(MAP_FILE) -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "SOLAR_TOWER.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
SOLAR_TOWER.hex: SOLAR_TOWER.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "SOLAR_TOWER.elf" "SOLAR_TOWER.hex"
	@echo 'Finished building: $@'
	@echo.
SOLAR_TOWER.lst: SOLAR_TOWER.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "SOLAR_TOWER.elf" > "SOLAR_TOWER.lst"
	@echo 'Finished building: $@'
	@echo.
SOLAR_TOWER.siz: SOLAR_TOWER.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "SOLAR_TOWER.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) SOLAR_TOWER.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
