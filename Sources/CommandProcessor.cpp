/*
 * CommandProcessor.cpp
 *
 *  Created on: 14 дек. 2017 г.
 *      Author: ivan
 */

#include "CommandProcessor.h"

namespace ElevatorEmulator
{

	EmuCommandProcessor& EmuCommandProcessor::create_command_processor( std::shared_ptr<Elevator> elevator )
	{
		static EmuCommandProcessor processor( elevator );
		return processor;
	}

	std::unique_ptr<ICommand> EmuCommandProcessor::parse_command( const std::string& user_request )
	{
		const int command_id = 0;
		// TODO: parse command and try map it to supported commands...

		switch (command_id)
		{
		case UserCommand_CallElevator:
			return std::make_unique<CallElevatorCommand>();
			break;

		case UserCommand_SelectFloor:
			return std::make_unique<SelectFloorCommand>();
			break;

		default:
			throw UnsupportedCommandException( " " );
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
				elevator_->move_to( (static_cast<std::unique_ptr<CallElevatorCommand>>( command ))->get_user_floor() );
				break;
			case UserCommand_SelectFloor:
				break;
			}
		} catch (Exception e) {

		}
	}

}


