################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DB_Operation.c \
../Debug_Operation.c \
../Door_Operation.c \
../Door_Process.c \
../FP_Operation.c \
../FP_Process.c \
../LCD_Operation.c \
../Ncurses_Operation.c \
../Sqlite_Operation.c \
../Wait_Operation.c \
../main.c \
../sqlite3.c \
./Cam_Operation.c

OBJS += \
./DB_Operation.o \
./Debug_Operation.o \
./Door_Operation.o \
./Door_Process.o \
./FP_Operation.o \
./FP_Process.o \
./LCD_Operation.o \
./Ncurses_Operation.o \
./Sqlite_Operation.o \
./Wait_Operation.o \
./main.o \
./sqlite3.o \
./Cam_Operation.o

C_DEPS += \
./DB_Operation.d \
./Debug_Operation.d \
./Door_Operation.d \
./Door_Process.d \
./FP_Operation.d \
./FP_Process.d \
./LCD_Operation.d \
./Ncurses_Operation.d \
./Sqlite_Operation.d \
./Wait_Operation.d \
./main.d \
./sqlite3.d \
./Cam_Operation.d


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/sis/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


