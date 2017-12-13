################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/Elevator.cpp \
../Sources/EmuConfig.cpp \
../Sources/elemu.cpp 

OBJS += \
./Sources/Elevator.o \
./Sources/EmuConfig.o \
./Sources/elemu.o 

CPP_DEPS += \
./Sources/Elevator.d \
./Sources/EmuConfig.d \
./Sources/elemu.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


