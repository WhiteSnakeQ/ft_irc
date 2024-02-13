#pragma once

#include "iostream"
#include "string"
#include "algorithm"
#include "vector"

class parser
{
public:
	static bool	parse_ip( std::string &ip );
	static bool	parse_port( std::string &port );
	static bool	is_number( std::string &numb );

	static void	toupper( std::string );

	static std::string				convertIntToStr( int num );
	static std::string				makeStrFromVector( std::vector<std::string> msg, int skip );

	static std::vector<std::string>	makeVectorString( std::string );
};
