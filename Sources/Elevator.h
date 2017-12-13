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
#include <thread>
#include <mutex>

#include "EmuCommands.h"

namespace ElevatorEmulator
{

	class InvalidFloorException : public std::exception
	{
	public:
		virtual const char* what() const noexcept { return "Invalid floor number provided"; }
		virtual ~InvalidFloorException() throw() {}
	};


	class ActionDeprecatedException : public std::exception
	{
	public:
		virtual const char* what() const noexcept { return "Invalid action due to "; }
		virtual ~ActionDeprecatedException() throw() {}

	private:
		std::string            reason_;
	};



	class Elevator
	{
	public:
		static std::shared_ptr<Elevator> create_elevator( unsigned floors_count, unsigned elevator_velocity );

		Elevator( unsigned floors_count, unsigned elevator_velocity );

		///
		/// Floor key selected inside elevator handler
		/// \param[in] floor: (key)
		/// throw: InvalidFloorException
		///
		void select_floor( unsigned floor );

		///
		/// Calling the elevator to a specified floor
		/// param[in] floor: (current user floor)
		///
		void get_to( unsigned floor );


	private:
		Elevator() = delete;

		bool move_to( unsigned floor );
		bool open_doors() const;
		bool close_doors() const;
		bool validate_action_applicable( UserCommand command ) const;


		const unsigned                   floors_count_;
		const unsigned                   elevator_velocity_ms_;

		unsigned                         current_floor;
		bool                             in_progress_;

		std::queue<Command>              commands_queue_;
		std::mutex                       mutex_;

		static std::shared_ptr<Elevator> impl_;
	};
}


#endif /* SOURCES_ELEVATOR_H_ */
