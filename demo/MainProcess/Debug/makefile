################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include luvcview/subdir.mk
-include Finchos/subdir.mk
-include v4l2bmp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MainProcess

# Tool invocations
MainProcess: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L/home/sis/lib -o "MainProcess" $(OBJS) $(USER_OBJS) $(LIBS) -ldl -lpthread -lncursesw 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MainProcess
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
