/*
 * CommandProcessor.cpp
 *
 *  Created on: 14 дек. 2017 г.
 *      Author: ivan
 */

#include "CommandProcessor.h"

#include <stdlib.h>
#include <iostream>

namespace ElevatorEmulator
{

	EmuCommandProcessor& EmuCommandProcessor::create_command_processor( std::shared_ptr<Elevator> elevator )
	{
		static EmuCommandProcessor processor( elevator );
		return processor;
	}

	std::unique_ptr<ICommand> EmuCommandProcessor::parse_command( const std::string& user_request )
	{
		int command_id = 0;

		if (user_request.empty())
			throw UnsupportedCommandException( "Unsupported command specified!.." );

		// TODO: parse command and try map it to supported commands...
		const auto delimeter_pos = user_request.find( " " );

		if (delimeter_pos != std::string::npos)
		{
			// TODO: the following design has to be improved later
			const auto command_part = user_request.substr( 0, delimeter_pos );
			if (command_part == "call")
			{
				command_id = UserCommand_CallElevator;
			}
			else if (command_part == "select")
			{
				command_id = UserCommand_SelectFloor;
			}
			else
				throw UnsupportedCommandException( "Unsupported command specified!.." );
		}
		else if (user_request == "exit")
		{
			command_id = UserCommand_SelectFloor;
			auto command = std::make_unique<ExitEmulatorCommand>();
			return command;
		}

		switch (command_id)
		{
		case UserCommand_CallElevator:
			{
				auto command = std::make_unique<CallElevatorCommand>();
				command->parse_command_parameters( user_request.substr( delimeter_pos ) );
				return command;
			}
		case UserCommand_SelectFloor:
			{
				auto command = std::make_unique<SelectFloorCommand>();
				command->parse_command_parameters( user_request.substr( delimeter_pos ) );
				return command;
			}
		default:
			throw UnsupportedCommandException( "Unsupported command specified!.." );
		}
	}

	void EmuCommandProcessor::execute_command( std::unique_ptr<ICommand> command, bool allow_delayed )
	{
		std::vector<Elevator::Action> list_of_actions;
		command->get_actions_list( list_of_actions );

		try {
			switch (command->get_command_type())
			{
			case UserCommand_CallElevator:
				elevator_->get_to( command->get_parameter(), list_of_actions );
				break;
			case UserCommand_SelectFloor:
				elevator_->select_floor( command->get_parameter(), list_of_actions );
				break;
			case UserCommand_Exit:
				break;
			}
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

}


