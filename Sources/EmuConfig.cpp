/*
 * EmuConfig.cpp
 *
 *  Created on: 12 дек. 2017 г.
 *      Author: ivan
 */

#include "EmuConfig.h"

#include <iostream>

namespace ElevatorEmulator
{

std::map<std::string, EmuConfig::parameter_description_type> EmuConfig::parameter_descs_ = {
		{"floors_count", {"count of floors in the building: ", MinFloorsCount, MaxFloorsCount, ""}},
		{"floor_height", {"each floor height in [m]:", MinFloorHeight, MaxFloorHeight, ""}},
		{"doors_action_time", {"time spent to open/close elevator doors [s]:", MinDoorsActionTime, MaxDoorsActionTime, ""}},
		{"elevator_valocity", {"elevator velocity [m/s]", MinElevatorVelocity, MaxElevatorVelocity, ""}}
};

EmuConfig& EmuConfig::parse_config( int argc, char* argv[] )
{
	static EmuConfig instance( argc, argv );
	try {
		instance.validate_config();
		std::cout << "Ready to start..." << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what();
		instance.print_help();
	}
	return instance;
}

EmuConfig::EmuConfig()
	: floors_count_( 0 )
	, floor_height_( 0 )
	, elevator_velocity_ms_( 0 )
	, time_of_doors_action_( 0 )
{}

EmuConfig::EmuConfig( int argc, char* argv[] )
	: floors_count_( 0 )
	, floor_height_( 0 )
	, elevator_velocity_ms_( 0 )
	, time_of_doors_action_( 0 )
{
	parse_config_parameters( argc, argv );
}

void EmuConfig::parse_config_parameters( int argc, char* argv[] )
{
	if (argc <= ParametersCount)
		throw InvalidParametersCount( argc-1, ParametersCount );

	for (int i = 0; i < argc; ++i)
	{
		const auto part = std::string( argv[i] );

		const auto delimeter_pos = part.find( "=" );
		if (delimeter_pos == std::string::npos)
		{
			std::cout << "Skipping " << argv[i] << std::endl;
			continue;
		}

		const auto parameter_name = part.substr( 0, delimeter_pos );

		if (!parameter_name.empty() && parameter_descs_.find( parameter_name ) != parameter_descs_.end())
		{
			const std::string parameter_value = part.substr( delimeter_pos + 1, part.length() );
			if (parameter_value.empty() || parameter_value.length() > MaxParameterString)
				throw WrongConfigurationException( "Too high parameter length!" );

			auto parameter_desc_position = parameter_descs_.find( parameter_name );
			if (parameter_desc_position == parameter_descs_.end())
				throw WrongConfigurationException( "Inapplicable key!" );

			std::get<3>( parameter_desc_position->second ) = parameter_value;
			std::cout << "Updated" << parameter_name << " set " << std::get<3>( parameter_desc_position->second ) << std::endl;
		}
	}

	// TODO: can be improved later...
	for (auto parameter_itr : parameter_descs_)
	{
		if (parameter_itr.first == "floors_count")
			floors_count_ = std::atoi( std::get<3>( parameter_itr.second ).c_str() );
		else if (parameter_itr.first == "floor_height")
			floor_height_ = std::atoi( std::get<3>( parameter_itr.second ).c_str() );
		else if (parameter_itr.first == "doors_action_time")
			time_of_doors_action_ = std::atoi( std::get<3>( parameter_itr.second ).c_str() );
		else if (parameter_itr.first == "elevator_valocity")
			elevator_velocity_ms_ = std::atoi( std::get<3>( parameter_itr.second ).c_str() );
	}
}

void EmuConfig::validate_config() const
{
	// Note: verification made for all parameters to simplify user understanding of the problem
	bool result = true;
	if (floors_count_ < MinFloorsCount || floors_count_ > MaxFloorsCount)
	{
		std::cerr << "Wrong floors count provided: " << floors_count_ << std::endl;
		result &= false;
	}

	if (floor_height_ < MinFloorHeight || floor_height_ > MaxFloorHeight)
	{
		std::cerr << "Wrong floor height provided: " << floor_height_ << std::endl;
		result &= false;
	}

	if (elevator_velocity_ms_ < MinElevatorVelocity || elevator_velocity_ms_ > MaxElevatorVelocity)
	{
		std::cerr << "Wrong elevator valocity provided: " << elevator_velocity_ms_ << std::endl;
		result &= false;
	}

	if (time_of_doors_action_ < MinDoorsActionTime || time_of_doors_action_ > MaxDoorsActionTime)
	{
		std::cerr << "Wrong doors action time provided: " << time_of_doors_action_ << std::endl;
		result &= false;
	}

	if (!result)
		throw WrongConfigurationException( "Invalid parameters were specified!");
}

void EmuConfig::print_help()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "  ElEmu <parameters>" << std::endl;
	std::cout << "  Where <parameters> are the following:" << std::endl;

	for (auto parameter_desc : parameter_descs_)
		std::cout << "    " << parameter_desc.first
		<< "= (" << std::get<0>( parameter_desc.second )
		<< ") of range [" << std::get<1>( parameter_desc.second )
		<< "; " << std::get<2>( parameter_desc.second ) << "]" << std::endl;
}

void EmuConfig::print_usage()
{
	std::cout << "Ready to start elevator emulation!" << std::endl;
	print_acceptable_commands();
}

void EmuConfig::print_acceptable_commands()
{
	std::cout << "Acceptable commands are:" << std::endl;
}

}


