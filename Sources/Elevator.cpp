/*
 * Elevator.cpp
 *
 *  Created on: 11 дек. 2017 г.
 *      Author: ivan
 */

#include "Elevator.h"

namespace ElevatorEmulator
{
	std::shared_ptr<Elevator> Elevator::impl_ = nullptr;

	std::shared_ptr<Elevator> Elevator::create_elevator( unsigned floors_count, unsigned elevator_velocity )
	{
		auto instance = std::make_shared<Elevator>( floors_count, elevator_velocity );
		impl_ = instance;
		return instance;
	}

	Elevator::Elevator( unsigned floors_count, unsigned elevator_velocity )
		: floors_count_( floors_count )
		, elevator_velocity_ms_( elevator_velocity )
		, current_floor( 0 )
		, in_progress_( false )
	{}

	void Elevator::select_floor( unsigned floor )
	{
		std::lock_guard<std::mutex> lock( mutex_ );
		if (impl_ && impl_->validate_action_applicable( UserCommand_SelectFloor ))
		{
			in_progress_ = true;


		}
	}

	bool Elevator::validate_action_applicable( UserCommand command ) const
	{
		switch (command)
		{
		case UserCommand_CallElevator: break; // applicable always
		case UserCommand_SelectFloor:
			// TODO: conditions have to be clarified!
			if (in_progress_)
				return false;
			break;
		default:
			throw ActionDeprecatedException();
		}
		return true;
	}
}

