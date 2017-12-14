/*
 * elemu.cpp
 *
 *  Created on: 9 дек. 2017 г.
 *      Author: ivan
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Elevator.h"
#include "EmuConfig.h"
#include "CommandProcessor.h"

int main(int argc, char* argv[])
{
	try {
		// 1. Read input parameters;
		auto config = ElevatorEmulator::EmuConfig::parse_config( argc, argv );
		auto emulator = ElevatorEmulator::Elevator::create_elevator( config.get_floors_count(), config.get_elevator_velocity_ms() );
		// 2. Validate parameters.
		// 3. Create Elevator emulator thread;
		// 4. Start commands listener.
		// 5. Print usage instraction and CL prompt
		auto command_processor = ElevatorEmulator::EmuCommandProcessor::create_command_processor( emulator );

		while (truel)
		{
			std::string input_string;
			std::getline( std::cin, input_string );
			try {
				auto command = command_processor.parse_command( input_string );
				command_processor.execute_command( command );
			} catch (ElevatorEmulator::UnsupportedCommandException& e) {
				// TODO: show usage and continue execution...
			}
		}
	} catch (std::exception& e) {
		std::cerr << "Error occurred: " << e.what() << std::endl;
		std::cerr << "Terminating..." << std::endl;
		return -1;
	} catch (...) {
		std::cerr << "Fatal error occurred! Terminating the app..." << std::endl;
		return -2;
	}
	std::cout << "Execution successfully completed. GoodBye!.." << std::endl;
	return 0;
}


