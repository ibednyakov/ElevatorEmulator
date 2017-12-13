/*
 * EmuConfig.h
 *
 *  Created on: 11 дек. 2017 г.
 *      Author: ivan
 */


#ifndef __EMU_CONFIG_H__
#define __EMU_CONFIG_H__

#include <exception>
#include <string>
#include <sstream>
#include <tuple>
#include <map>

namespace ElevatorEmulator
{

	enum Limits
	{
		MaxFloorsCount = 20,     ///< Pre-defined Maximum Floors Count
		MinFloorsCount = 5,      ///< Pre-defined Minimum floors count

		MaxElevatorVelocity = 5, ///< Assuming elevator cannot move faster than 5 m/s
		MaxDoorsActionTime = 10, ///< Assuming doors opening\closing takes less than 10 seconds
		MaxFloorHeight = 5,      ///< Assuming 5m - Maximum floor height

		MinElevatorVelocity = 1, ///< Assuming elevator cannot move faster than 1 m/s
		MinDoorsActionTime = 1,  ///< Assuming doors opening\closing takes less than 1 second
		MinFloorHeight = 2,      ///< Assuming 2m - Minimum floor height
	};


	class InvalidParametersCount: public std::exception
	{
	public:
		InvalidParametersCount( unsigned original_count, unsigned required_count )
		{
			std::stringstream ss;
			ss << "Invalid number of mandatory parameters provided! [" << required_count << " / " << original_count << "]";
			hint_ = ss.str();
		}
		virtual const char* what() const noexcept { return hint_.c_str(); }
		virtual ~InvalidParametersCount() throw() {}

	private:
		std::string hint_;
	};


	class WrongConfigurationException : public std::exception
	{
	public:
		WrongConfigurationException() : hint_( "Invalid configuration!" ) {}
		explicit WrongConfigurationException( const std::string& hint ) : hint_( hint ) {}

		virtual const char* what() const noexcept { return hint_.c_str(); }
		virtual ~WrongConfigurationException() throw() {}

	private:
		std::string hint_;
	};


	class EmuConfig
	{
		enum
		{
			ParametersCount = 4,
			MaxParameterString = 3,
		};

	public:
		typedef std::tuple<std::string, unsigned, unsigned, std::string> parameter_description_type;

		static EmuConfig& parse_config( int argc, char* argv[] );

		unsigned get_floors_count() const { return floors_count_; }
		unsigned get_floor_height() const { return floor_height_; }
		unsigned get_elevator_velocity_ms() const { return elevator_velocity_ms_; }
		unsigned get_time_of_doors_action() const { return time_of_doors_action_; }

	private:
		unsigned                          floors_count_;
		unsigned                          floor_height_;
		unsigned                          elevator_velocity_ms_;
		unsigned                          time_of_doors_action_;

		EmuConfig();
		EmuConfig( int argc, char* argv[] );

		void parse_config_parameters( int argc, char* argv[] );
		void validate_config() const;

		static void print_help();
		static void print_usage();
		static void print_acceptable_commands();

		static std::map<std::string, parameter_description_type> parameter_descs_;

	};
}

#endif // __EMU_CONFIG_H__
