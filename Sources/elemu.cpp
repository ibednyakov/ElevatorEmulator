/*
 * elemu.cpp
 *
 *  Created on: 9 дек. 2017 г.
 *      Author: ivan
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#include "CommandProcessor.h"
#include "Elevator.h"
#include "EmuConfig.h"

using namespace ElevatorEmulator;
void print_usage();

int main(int argc, char* argv[])
{
	try {
		// 1. Read input parameters;
		auto config = EmuConfig::parse_config( argc, argv );
		auto emulator = Elevator::create_elevator( config.get_floors_count(), config.get_elevator_velocity_ms(), config.get_floor_height(), config.get_time_of_doors_action() );
		// 2. Validate parameters.
		// 3. Create Elevator emulator thread;
		// 4. Start commands listener.
		// 5. Print usage instraction and CL prompt
		auto command_processor = EmuCommandProcessor::create_command_processor( emulator );

		while (true)
		{
			std::string input_string;
			std::getline( std::cin, input_string );

			try {
				auto command = command_processor.parse_command( input_string );
				if (command->get_command_type() == UserCommand_Exit)
					break;
				command_processor.execute_command( std::move( command ) );

			} catch (UnsupportedCommandException& e) {
				print_usage();
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


void print_usage()
{
	std::cout << "  Emulator usage:" << std::endl;
	std::cout << "  call <floor>: calls elevator to the specified floor" << std::endl;
	std::cout << "  select <floor>: floor selection imitation - moves elevator to the specified floor" << std::endl;
	std::cout << "  exit: exits emulator" << std::endl;
}


