#include "../../class_hpp/parser.hpp"

bool	parser::is_number( std::string &numb )
{
	if (numb.find_first_not_of("0123456789") == numb.npos)
		return (true);
	return (false); 
}

bool	parser::parse_ip( std::string &ip )
{
	size_t		pos;
	size_t		size;
	std::string	tmp;
	std::string to_check;

	if (std::count(ip.begin(), ip.end(), '.') != 3)
		return (false);
	size = ip.length();
	if (7 > size || size > 15)
		return (false);
	tmp = ip.substr(0);
	for (int i = 0; i < 4; i++)
	{
		pos = tmp.find_first_of(".");
		to_check = tmp.substr(0, pos);
		tmp = tmp.substr(pos + 1);
		if (!is_number(to_check) || std::atoi(to_check.c_str()) >= 256)
			return (false);
	}
	return (true);
}

bool	parser::parse_port( std::string &port )
{
	int			port_int;

	if (port.length() <= 0 || port.length() >= 6 || !is_number(port))
		return (false);
	port_int = std::atoi(port.c_str());
	if (1024 > port_int || port_int > 65535)
		return (false);
	return (true);
}

std::vector<std::string>	parser::makeVectorString( std::string string )
{
	size_t						pos;
	std::string					tmp;
	std::string 				toVector;
	std::vector<std::string>	toRet;

	pos = 0;
	tmp = string;
	while (tmp.find("\r\n") != std::string::npos)
	{	
		toVector = tmp;
		pos = tmp.find_first_of(" \n\f\v\r\t\b\a");
		toVector = tmp.substr(0, pos);
		tmp = tmp.substr(pos + 1);
		if (toVector != "\r" && toVector.length() != 0)
			toRet.push_back(toVector);
	}
	while (tmp.find("\n") != std::string::npos)
	{	
		toVector = tmp;
		pos = tmp.find_first_of(" \n\f\v\r\t\b\a");
		toVector = tmp.substr(0, pos);
		tmp = tmp.substr(pos + 1);
		if (toVector != "\r" && toVector.length() != 0)
			toRet.push_back(toVector);
	}
	return (toRet);
}

std::string	parser::makeStrFromVector( std::vector<std::string> msg, int skip )
{
	std::string messsage = "";
	int	j = 0;

	for (std::vector<std::string>::iterator i = msg.begin(); i != msg.end(); i++, j++)
	{
		if (j < skip)
			continue ;
		messsage += *i;
		messsage += " ";
	}
	return (messsage);
}
