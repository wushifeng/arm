################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../avilib.c \
../color.c \
../converter.c \
../gui.c \
../luvcview.c \
../utils.c \
../v4l2uvc.c 

OBJS += \
./avilib.o \
./color.o \
./converter.o \
./gui.o \
./luvcview.o \
./utils.o \
./v4l2uvc.o 

C_DEPS += \
./avilib.d \
./color.d \
./converter.d \
./gui.d \
./luvcview.d \
./utils.d \
./v4l2uvc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DLINUX -DVERSION=0.2.1 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


