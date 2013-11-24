################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InitShader.cpp \
../src/rubics\ cube.cpp 

OBJS += \
./src/InitShader.o \
./src/rubics\ cube.o 

CPP_DEPS += \
./src/InitShader.d \
./src/rubics\ cube.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rubics\ cube.o: ../src/rubics\ cube.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/rubics cube.d" -MT"src/rubics\ cube.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


