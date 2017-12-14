/*
 * CommandProcessor.h
 *
 *  Created on: 14 дек. 2017 г.
 *      Author: ivan
 */

#ifndef SOURCES_COMMANDPROCESSOR_H_
#define SOURCES_COMMANDPROCESSOR_H_

#include "EmuCommands.h"

#include <memory>

namespace ElevatorEmulator
{


	class EmuCommandProcessor
	{
	public:
		static EmuCommandProcessor& create_command_processor( std::shared_ptr<Elevator> elevator );
		std::unique_ptr<ICommand> parse_command( const std::string& user_request );
		void execute_command( std::unique_ptr<ICommand> command, bool allow_delayed = false );

	private:
		void add_command( const ICommand& command );

		explicit EmuCommandProcessor( std::shared_ptr<Elevator> elevator ) : elevator_( elevator ) {}
		EmuCommandProcessor() = delete;

		std::queue<std::shared_ptr<ICommand>>                requests_;
		std::shared_ptr<Elevator>                            elevator_;
	};


}

#endif /* SOURCES_COMMANDPROCESSOR_H_ */
