/*
 * Elevator.cpp
 *
 *  Created on: 11 дек. 2017 г.
 *      Author: ivan
 */

#include "Elevator.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>


namespace ElevatorEmulator
{

	std::shared_ptr<Elevator> Elevator::impl_ = nullptr;

	std::shared_ptr<Elevator> Elevator::create_elevator( unsigned floors_count, unsigned elevator_velocity, unsigned floor_height, unsigned time_of_doors_action )
	{
		static std::shared_ptr<Elevator> instance = std::make_shared<Elevator>( floors_count, elevator_velocity, floor_height, time_of_doors_action );
		impl_ = instance;
		return instance;
	}

	const char* Elevator::command_pretty_name( UserCommand command )
	{
		switch (command)
		{
		case UserCommand_CallElevator: return "Call-elevator command";
		case UserCommand_Exit: return "Exit emulator";
		case UserCommand_SelectFloor: return "Select floor to move to command";
		default: return "Unrecognized command";
		}
	}

	const char* Elevator::action_pretty_name( Action action )
	{
		switch (action)
		{
		case Action_CloseDoors: return "Close Doors action";
		case Action_Move: return "Move action";
		case Action_OpenDoors: return "Select floor to move to command";
		default: return "Unrecognized action";
		}
	}

	Elevator::Elevator( unsigned floors_count, unsigned elevator_velocity, unsigned floor_height, unsigned time_of_doors_action )
		: floors_count_( floors_count )
		, elevator_velocity_ms_( elevator_velocity )
		, floor_height_( floor_height )
		, time_of_doors_action_( time_of_doors_action )
		, current_floor_( 0 )
		, in_progress_( false )
	{}

	Elevator::~Elevator()
	{
		std::cout << "  Emulator dtor..." << std::endl;
		if (executor_.joinable())
			executor_.join();
	}

	void Elevator::execute_common_part_async(unsigned floor,
			const std::vector<Elevator::Action>& list_of_actions)
	{
		in_progress_ = true;
		int floors_delta = floor - current_floor_;
		std::cout << "  Floors delta is " << floors_delta << std::endl;
		executor_ = std::thread([list_of_actions, this, floors_delta]() {
			if (floors_delta)
			{
				auto floors_counter = floors_delta;
				for (auto action: list_of_actions)
				{
					std::cout << "  processing action " << action_pretty_name( action ) << std::endl;
					switch (action)
					{
					case Action_CloseDoors:
						close_doors();
						break;
					case Action_OpenDoors:
						open_doors();
						break;
					case Action_Move:
						while (floors_counter) {
							if (floors_counter < 0) {
								move_down();
								++floors_counter;
							} else
							if (floors_counter > 0) {
								move_up();
								--floors_counter;
							}
						}
						break;
					}
				}
			}
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			std::cout << "Ready!.. Current floor is " << current_floor_ << std::endl;
			in_progress_ = false;
		});
	}

	void Elevator::select_floor( unsigned floor, const std::vector<Elevator::Action>& list_of_actions )
	{
		if (floor > floors_count_)
			throw InvalidFloorException();

		if (validate_action_applicable( UserCommand_SelectFloor ) )
		{
			std::cout << "Waiting for emulator thread execution finished!.." << std::endl;
			if (executor_.joinable())
				executor_.join();

			execute_common_part_async( floor, list_of_actions);
		}
		else
		{
			std::cerr << "  Not passed validation!" << std::endl;
			throw ActionDeprecatedException( "Validation not passed!" );
		}
	}


	void Elevator::get_to( unsigned floor, const std::vector<Elevator::Action>& list_of_actions )
	{
		if (floor > floors_count_)
			throw InvalidFloorException();

		if (validate_action_applicable( UserCommand_CallElevator ))
		{
			std::cout << "Waiting for emulator thread execution finished!.." << std::endl;
			if (executor_.joinable())
				executor_.join();

			execute_common_part_async( floor, list_of_actions );
		}
		else
		{
			std::cerr << "  Not passed validation!" << std::endl;
			throw ActionDeprecatedException( "Validation not passed!" );
		}
	}


	bool Elevator::move_up()
	{

		unsigned delay_us = 0;
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			delay_us = 1000 * floor_height_ / elevator_velocity_ms_ + 100 * (floor_height_ % elevator_velocity_ms_);
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( delay_us ) );
		std::time_t t = std::time( nullptr );
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Passed " << current_floor_ << " floor " << std::endl;
			++current_floor_;
		}
		return true;
	}

	bool Elevator::move_down()
	{
		unsigned delay_us = 0;
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			delay_us = 1000 * floor_height_ / elevator_velocity_ms_ + 100 * (floor_height_ % elevator_velocity_ms_);
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( delay_us ) );
		std::time_t t = std::time( nullptr );
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Passed " << current_floor_ << " floor " << std::endl;
			--current_floor_;
		}
		return true;
	}

	bool Elevator::open_doors() const
	{
		unsigned time_of_doors_action = 0;
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			time_of_doors_action = time_of_doors_action_;
		}
		std::this_thread::sleep_for( std::chrono::seconds( time_of_doors_action ) );
		std::time_t t = std::time( nullptr );
		std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Doors are opened!" << std::endl;
		return true;
	}

	bool Elevator::close_doors() const
	{
		unsigned time_of_doors_action = 0;
		{
			std::lock_guard<std::recursive_mutex> lock( mutex_ );
			time_of_doors_action = time_of_doors_action_;
		}
		std::this_thread::sleep_for( std::chrono::seconds( time_of_doors_action ) );
		std::time_t t = std::time( nullptr );
		std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Doors are closed!" << std::endl;
		return true;
	}


	bool Elevator::validate_action_applicable( UserCommand command ) const
	{
		std::lock_guard<std::recursive_mutex> lock( mutex_ );
		std::cout << "  validating for " << command_pretty_name( command ) << " InProgress status is " << in_progress_ << std::endl;

		switch (command)
		{
		case UserCommand_CallElevator:
			break; // applicable always
		case UserCommand_SelectFloor:
			// TODO: conditions have to be clarified!
			if (in_progress_)
				return false;
			break;
		default:
			throw ActionDeprecatedException( "Command not supported by Elevator Emulator yet!" );
		}
		return true;
	}
}

