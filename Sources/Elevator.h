/*
 * Elevator.h
 *
 *  Created on: 11 дек. 2017 г.
 *      Author: ivan
 */

#ifndef SOURCES_ELEVATOR_H_
#define SOURCES_ELEVATOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <queue>
#include <memory>

#include <mutex>
#include <thread>

//#include "EmuCommands.h"

namespace ElevatorEmulator
{

	enum UserCommand
	{
		UserCommand_CallElevator,
		UserCommand_SelectFloor,
		UserCommand_Exit,
	};

	class InvalidFloorException : public std::exception
	{
	public:
		virtual const char* what() const noexcept { return "Invalid floor number provided"; }
		virtual ~InvalidFloorException() throw() {}
	};


	class ActionDeprecatedException : public std::exception
	{
	public:
		virtual const char* what() const noexcept { return reason_.c_str(); }
		virtual ~ActionDeprecatedException() throw() {}
		explicit ActionDeprecatedException( const std::string& hint ) : reason_( hint ) {}

	private:
		std::string            reason_;
	};


	class ICommand;

	class Elevator
	{
	public:
		enum Action
		{
			Action_CloseDoors,
			Action_OpenDoors,
			Action_Move,
		};

		static std::shared_ptr<Elevator> create_elevator( unsigned floors_count, unsigned elevator_velocity, unsigned floor_height, unsigned time_of_doors_action );

		Elevator( unsigned floors_count, unsigned elevator_velocity, unsigned floor_height, unsigned time_of_doors_action );

		~Elevator();

		///
		/// Floor key selected inside elevator handler
		/// \param[in] floor: (key)
		/// throw: InvalidFloorException
		///
		void select_floor( unsigned floor, const std::vector<Elevator::Action>& list_of_actions );

		///
		/// Calling the elevator to a specified floor
		/// param[in] floor: (current user floor)
		///
		void get_to( unsigned floor, const std::vector<Elevator::Action>& list_of_actions );

	private:
		Elevator() = delete;

		bool move_up();
		bool move_down();
		bool open_doors() const;
		bool close_doors() const;
		bool validate_action_applicable( UserCommand command ) const;
		void execute_common_part_async(unsigned floor,
			const std::vector<Elevator::Action>& list_of_actions);

		const unsigned                   floors_count_;
		const unsigned                   elevator_velocity_ms_;
		const unsigned                   floor_height_;
		const unsigned                   time_of_doors_action_;

		unsigned                         current_floor_;
		bool                             in_progress_;

		mutable std::recursive_mutex     mutex_;
		std::thread                      executor_;

		static std::shared_ptr<Elevator> impl_;
	};

}


#endif /* SOURCES_ELEVATOR_H_ */
