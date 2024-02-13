#pragma once

#include "iostream"
#include "vector"
#include "parser.hpp"

class user;

class bot
{
public:
	static std::string	cmdHandler( std::vector<user *> users, std::vector<std::string> msg, std::string chName );
	static std::string	getUsersInfo( std::vector<user *> users );
	static std::string	randNumb( std::vector<std::string> msg );	
};