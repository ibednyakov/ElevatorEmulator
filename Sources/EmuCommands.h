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
#include <exception>


namespace ElevatorEmulator
{

	class UnsupportedCommandException : public std::exception
	{
	public:
		virtual const char* what() const noexcept { return hint_.c_str(); }

		explicit UnsupportedCommandException( const std::string& hint ) : hint_( hint ) {}

		virtual ~UnsupportedCommandException() throw() {}

	private:
		std::string hint_;
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

		///
		/// \brief Gets the type of the command
		/// \return UserCommand type
		///
		virtual UserCommand get_command_type() const =0;

		///
		/// \brief Parses commands' parameters from a passed string
		/// \param[in] parameters: the string containing commands parameters
		///
		virtual void parse_command_parameters( const std::string& parameters ) =0;

		///
		/// \brief Returns command parameter. For harder implementations can be modified to tuple
		/// \return unsigned parameter value
		///
		virtual unsigned get_parameter() const { return 0; }
	};


	class CallElevatorCommand : public ICommand
	{
	public:
		~CallElevatorCommand() {}

		virtual void get_actions_list( std::vector<Elevator::Action>& actions );
		virtual UserCommand get_command_type() const { return UserCommand_CallElevator; }
		virtual void parse_command_parameters( const std::string& parameters );

		unsigned get_parameter() const { return user_floor_; }

	private:
		unsigned                               user_floor_;
		static std::vector<Elevator::Action>   actions_;
	};


	class SelectFloorCommand : public ICommand
	{
	public:
		~SelectFloorCommand() {}

		virtual void get_actions_list( std::vector<Elevator::Action>& actions );
		virtual UserCommand get_command_type() const { return UserCommand_SelectFloor; }
		virtual void parse_command_parameters( const std::string& parameters );

		unsigned get_parameter() const { return target_floor_; }

	private:
		unsigned                               target_floor_;
		static std::vector<Elevator::Action>   actions_;
	};


	class ExitEmulatorCommand : public ICommand
	{
	public:
		~ExitEmulatorCommand() {}

		virtual void get_actions_list( std::vector<Elevator::Action>& actions ) {}
		virtual UserCommand get_command_type() const { return UserCommand_Exit; }
		virtual void parse_command_parameters( const std::string& parameters ) {}
	};

}


#endif /* SOURCES_EMUCOMMANDS_H_ */
