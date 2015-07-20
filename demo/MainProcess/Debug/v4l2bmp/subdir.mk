################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../v4l2bmp/v4l2.c 

OBJS += \
./v4l2bmp/v4l2.o 

C_DEPS += \
./v4l2bmp/v4l2.d 


# Each subdirectory must supply rules for building sources it contributes
v4l2bmp/%.o: ../v4l2bmp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/sis/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


