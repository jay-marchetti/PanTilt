################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/molloy/GPIO.cpp \
../src/molloy/PWM.cpp \
../src/molloy/util.cpp 

OBJS += \
./src/molloy/GPIO.o \
./src/molloy/PWM.o \
./src/molloy/util.o 

CPP_DEPS += \
./src/molloy/GPIO.d \
./src/molloy/PWM.d \
./src/molloy/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/molloy/%.o: ../src/molloy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


