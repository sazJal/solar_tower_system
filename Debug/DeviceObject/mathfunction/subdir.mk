################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DeviceObject/mathfunction/mathfunc_operation.c 

OBJS += \
./DeviceObject/mathfunction/mathfunc_operation.o 

C_DEPS += \
./DeviceObject/mathfunction/mathfunc_operation.d 


# Each subdirectory must supply rules for building sources it contributes
DeviceObject/mathfunction/%.o: ../DeviceObject/mathfunction/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC1404_F064x0200 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/DeviceOperation/operation" -I"$(PROJECT_LOC)/DeviceObject/infotainment" -I"$(PROJECT_LOC)/DeviceOperation/GlobalVariable" -I"$(PROJECT_LOC)/DeviceOperation/interrupts" -I"$(PROJECT_LOC)/DeviceObject/esp32" -I"$(PROJECT_LOC)/DeviceObject/geenix" -I"$(PROJECT_LOC)/DeviceObject/mathfunction" -I"$(PROJECT_LOC)/DeviceObject/miscellaneous" -I"$(PROJECT_LOC)/DeviceObject/pcs" -I"$(PROJECT_LOC)/DeviceObject/pdu" -I"$(PROJECT_LOC)/DeviceObject/rs232" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC1400_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

