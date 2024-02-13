#pragma once

#include "../headers/colors.h"
#include "iostream"

# define INVARG_CODE 1
# define INVARG "Invalid arguments!\nUsing: ./ft_irc <PORT> <PASSWORD>"

# define INVPORT_CODE 2
# define INVPORT "Invalid port!"

# define INVPASS_CODE 3
# define INVPASS "Invalid Password!"

# define SOCKERR_CODE 4
# define SOCKERR "Socket create fail!"

# define SETSOCKERR_CODE 5
# define SETSOCKERR "Socket option set fail!"

# define BINDERR_CODE 6
# define BINDERR "Bind fail!"

# define UNBLOKSOCERR_CODE 7
# define UNBLOKSOCERR "Unbloking socket failed!"

# define POLERR_CODE 8
# define POLSERR "Polling faild!"

# define ACCEPTERR_CODE 9
# define ACCEPTERR "Accept failed to return user socket!"

# define SENDERR_CODE 10
# define SENDERR "Send message failed!"

# define RECVERR_CODE 11
# define RECVERR "Recive message failed!"

# define INVSERVNAME_CODE 12
# define INVSERVNAME "Server name is invalid!"

class error
{
public:
	static int	print_error( std::string &error );
	static int	error_index_print( int error );
	static void	print_emessage( std::string color1, std::string message);

	static void	print_message( std::string color1, std::string message);
};
