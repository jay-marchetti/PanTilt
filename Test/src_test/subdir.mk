################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_test/Factorial_test.cpp \
../src_test/PanTilt.cpp 

OBJS += \
./src_test/Factorial_test.o \
./src_test/PanTilt.o 

CPP_DEPS += \
./src_test/Factorial_test.d \
./src_test/PanTilt.d 


# Each subdirectory must supply rules for building sources it contributes
src_test/%.o: ../src_test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/jaym/workspace/PanTilt/Includes_test -I/home/jaym/workspace/PanTilt/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


