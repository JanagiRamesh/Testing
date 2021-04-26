################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Custom_Libs/Src/CAN_Application.c \
../Custom_Libs/Src/CAN_Driver.c \
../Custom_Libs/Src/Debug.c 

OBJS += \
./Custom_Libs/Src/CAN_Application.o \
./Custom_Libs/Src/CAN_Driver.o \
./Custom_Libs/Src/Debug.o 

C_DEPS += \
./Custom_Libs/Src/CAN_Application.d \
./Custom_Libs/Src/CAN_Driver.d \
./Custom_Libs/Src/Debug.d 


# Each subdirectory must supply rules for building sources it contributes
Custom_Libs/Src/%.o: ../Custom_Libs/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F413xx -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Core/Inc" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Custom_Libs" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Custom_Libs" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Admin/Desktop/Working Project/CAN_KIT_Test/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


