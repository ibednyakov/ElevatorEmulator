/*
 * EmuCommands.cpp
 *
 *  Created on: 14 дек. 2017 г.
 *      Author: ivan
 */

#include "EmuCommands.h"

namespace ElevatorEmulator
{

std::vector<Elevator::Action> CallElevatorCommand::actions_ = { Elevator::Action_CloseDoors, Elevator::Action_Move, Elevator::Action_OpenDoors };

std::vector<Elevator::Action> SelectFloorCommand::actions_ = { Elevator::Action_CloseDoors, Elevator::Action_Move, Elevator::Action_OpenDoors };

void CallElevatorCommand::parse_command_parameters( const std::string& parameters )
{
	if (!parameters.empty())
		user_floor_ = std::atoi( parameters.c_str() );
}

void CallElevatorCommand::get_actions_list( std::vector<Elevator::Action>& actions )
{
	actions = actions_;
}

void SelectFloorCommand::parse_command_parameters( const std::string& parameters )
{
	if (!parameters.empty())
		target_floor_ = std::atoi( parameters.c_str() );
}

void SelectFloorCommand::get_actions_list( std::vector<Elevator::Action>& actions )
{
	actions = actions_;
}

}


