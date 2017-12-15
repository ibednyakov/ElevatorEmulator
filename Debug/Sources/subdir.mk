################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/CommandProcessor.cpp \
../Sources/Elevator.cpp \
../Sources/EmuCommands.cpp \
../Sources/EmuConfig.cpp \
../Sources/elemu.cpp 

OBJS += \
./Sources/CommandProcessor.o \
./Sources/Elevator.o \
./Sources/EmuCommands.o \
./Sources/EmuConfig.o \
./Sources/elemu.o 

CPP_DEPS += \
./Sources/CommandProcessor.d \
./Sources/Elevator.d \
./Sources/EmuCommands.d \
./Sources/EmuConfig.d \
./Sources/elemu.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


