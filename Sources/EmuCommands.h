/*
 * EmuCommands.h
 *
 *  Created on: 9 дек. 2017 г.
 *      Author: ivan
 */

#ifndef SOURCES_EMUCOMMANDS_H_
#define SOURCES_EMUCOMMANDS_H_

namespace ElevatorEmulator
{

enum UserCommand
{
	UserCommand_CallElevator,
	UserCommand_SelectFloor,
};

class Command
{
public:
private:
	UserCommand               user_command_;
};

class EmuCommandProcessor
{
public:
private:

};
}


#endif /* SOURCES_EMUCOMMANDS_H_ */
