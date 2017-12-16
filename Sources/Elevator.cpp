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
		if (executor_.joinable())
			executor_.join();
	}

	void Elevator::execute_common_part_async(unsigned floor,
			const std::vector<Elevator::Action>& list_of_actions)
	{
		in_progress_ = true;
		auto floors_delta = floor - current_floor_;
		std::cout << "  Floors delta is " << floors_delta << std::endl;
		executor_ = std::thread([list_of_actions, this, &floors_delta]() {
			if (floors_delta)
			{
				for (auto action: list_of_actions)
				{
					switch (action)
					{
					case Action_CloseDoors:
						close_doors();
						break;
					case Action_OpenDoors:
						open_doors();
						break;
					case Action_Move:
						while (floors_delta) {
							if (floors_delta < 0) {
								move_down();
								++floors_delta;
							} else
							if (floors_delta > 0) {
								move_up();
								--floors_delta;
							}
						}
						break;
					}
				}
			}
			std::cout << "Ready!.." << std::endl;
			std::lock_guard<std::mutex> lock(mutex_);
			in_progress_ = false;
		});
	}

	void Elevator::select_floor( unsigned floor, const std::vector<Elevator::Action>& list_of_actions )
	{
		if (floor > floors_count_)
			throw InvalidFloorException();

		std::lock_guard<std::mutex> lock( mutex_ );
		if (validate_action_applicable( UserCommand_SelectFloor ) )
		{
			execute_common_part_async(floor, list_of_actions);
		}
		else
		{
			throw ActionDeprecatedException( "Validation not passed!" );
		}
	}


	void Elevator::get_to( unsigned floor, const std::vector<Elevator::Action>& list_of_actions )
	{
		if (floor > floors_count_)
			throw InvalidFloorException();

		std::lock_guard<std::mutex> lock( mutex_ );
		if (impl_ && impl_->validate_action_applicable( UserCommand_CallElevator ))
		{
			execute_common_part_async(floor, list_of_actions);
		}
		else
		{
			throw ActionDeprecatedException( "Validation not passed!" );
		}
	}


	bool Elevator::move_up()
	{

		unsigned delay_us = 0;
		{
			std::lock_guard<std::mutex> lock( mutex_ );
			delay_us = 1000 * floor_height_ / elevator_velocity_ms_ + 100 * (floor_height_ % elevator_velocity_ms_);
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( delay_us ) );
		std::time_t t = std::time( nullptr );
		{
			std::lock_guard<std::mutex> lock( mutex_ );
			std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Passed " << current_floor_ << " floor " << std::endl;
			++impl_->current_floor_;
		}
		return true;
	}

	bool Elevator::move_down()
	{
		unsigned delay_us = 0;
		{
			std::lock_guard<std::mutex> lock( mutex_ );
			delay_us = 1000 * floor_height_ / elevator_velocity_ms_ + 100 * (floor_height_ % elevator_velocity_ms_);
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( delay_us ) );
		std::time_t t = std::time( nullptr );
		{
			std::lock_guard<std::mutex> lock( mutex_ );
			std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Passed " << current_floor_ << " floor " << std::endl;
			--impl_->current_floor_;
		}
		return true;
	}

	bool Elevator::open_doors() const
	{
		unsigned time_of_doors_action = 0;
		{
			std::lock_guard<std::mutex> lock( mutex_ );
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
			std::lock_guard<std::mutex> lock( mutex_ );
			time_of_doors_action = time_of_doors_action_;
		}
		std::this_thread::sleep_for( std::chrono::seconds( time_of_doors_action ) );
		std::time_t t = std::time( nullptr );
		std::cout << std::put_time( std::localtime( &t ), "%T" ) << "  Doors are closed!" << std::endl;
		return true;
	}


	bool Elevator::validate_action_applicable( UserCommand command ) const
	{
		switch (command)
		{
		case UserCommand_CallElevator: break; // applicable always
		case UserCommand_SelectFloor:
			// TODO: conditions have to be clarified!
			if (impl_->in_progress_)
				return false;
			break;
		default:
			throw ActionDeprecatedException( "Command not supported by Elevator Emulator yet!" );
		}
		return true;
	}
}

