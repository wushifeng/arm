################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../luvcview/avilib.c \
../luvcview/color.c \
../luvcview/converter.c \
../luvcview/gui.c \
../luvcview/luvcview.c \
../luvcview/main.c \
../luvcview/utils.c \
../luvcview/v4l2uvc.c 

OBJS += \
./luvcview/avilib.o \
./luvcview/color.o \
./luvcview/converter.o \
./luvcview/gui.o \
./luvcview/luvcview.o \
./luvcview/main.o \
./luvcview/utils.o \
./luvcview/v4l2uvc.o 

C_DEPS += \
./luvcview/avilib.d \
./luvcview/color.d \
./luvcview/converter.d \
./luvcview/gui.d \
./luvcview/luvcview.d \
./luvcview/main.d \
./luvcview/utils.d \
./luvcview/v4l2uvc.d 


# Each subdirectory must supply rules for building sources it contributes
luvcview/%.o: ../luvcview/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/sis/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


