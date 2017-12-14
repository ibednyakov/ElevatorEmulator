/*
 * EmuCommands.h
 *
 *  Created on: 9 дек. 2017 г.
 *      Author: ivan
 */

#ifndef SOURCES_EMUCOMMANDS_H_
#define SOURCES_EMUCOMMANDS_H_

#include "Elevator.h"

#include <memory>
#include <queue>
#include <vector>


namespace ElevatorEmulator
{

	class UnsupportedCommandException : public std::runtime_error
	{
	public:
		virtual const char* what() const noexcept { return hint_.c_str(); }

		explicit UnsupportedCommandException( const std::string& hint ) : hint_( hint ) {}

		virtual ~UnsupportedCommandException() throw() {}

	private:
		std::string hint_;
	};

	enum UserCommand
	{
		UserCommand_CallElevator,
		UserCommand_SelectFloor,
	};

	class ICommand
	{
	public:
		static std::unique_ptr<ICommand> create_command( UserCommand command_type );
		virtual ~ICommand() {}

		///
		/// \brief Gets the list of required Evelators' micro-actions
		/// \param[in/out] actions: vector of required actions
		///
		virtual void get_actions_list( std::vector<Elevator::Action>& actions ) =0;

		virtual UserCommand get_command_type() const =0;

		virtual void parse_command_parameters( const std::string& parameters ) =0;
	};


	class CallElevatorCommand : public ICommand
	{
	public:
		virtual void get_actions_list( std::vector<Elevator::Action>& actions );
		virtual UserCommand get_command_type() const { return UserCommand_CallElevator; }

		virtual void parse_command_parameters( const std::string& parameters );

		unsigned get_user_floor() const { return user_floor_; }

	private:
		unsigned                        user_floor_;
	};


	class SelectFloorCommand : public ICommand
	{
	public:
		virtual void get_actions_list( std::vector<Elevator::Action>& actions );
		virtual UserCommand get_command_type() const { return UserCommand_SelectFloor; }
		virtual void parse_command_parameters( const std::string& parameters );

		unsigned get_target_floor() const { return target_floor_; }

	private:
		unsigned                        target_floor_;
	};

}


#endif /* SOURCES_EMUCOMMANDS_H_ */
