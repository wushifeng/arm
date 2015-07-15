################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Finchos/Finchos_V3.c \
../Finchos/Process_blh3001.c \
../Finchos/Process_fsc1001B.c \
../Finchos/Process_fsc2001.c \
../Finchos/fp_port.c \
../Finchos/finchos.c 

OBJS += \
./Finchos/Finchos_V3.o \
./Finchos/Process_blh3001.o \
./Finchos/Process_fsc1001B.o \
./Finchos/Process_fsc2001.o \
./Finchos/fp_port.o \
./Finchos/finchos.o 

C_DEPS += \
./Finchos/Finchos_V3.d \
./Finchos/Process_blh3001.d \
./Finchos/Process_fsc1001B.d \
./Finchos/Process_fsc2001.d \
./Finchos/fp_port.d \
./Finchos/finchos.d 


# Each subdirectory must supply rules for building sources it contributes
Finchos/%.o: ../Finchos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


