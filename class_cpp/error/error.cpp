#include "../../class_hpp/error.hpp"

/*Print error, took number from error.hpp*/
int	error::error_index_print( int error )
{
	std::string	error_msg;

	switch (error)
	{
	case INVARG_CODE:
		error_msg = INVARG;
		break ;
	case INVPORT_CODE:
		error_msg = INVPORT;
		break ;
	case INVPASS_CODE:
		error_msg = INVPASS;
		break ;
	case SOCKERR_CODE:
		error_msg = SOCKERR;
		break ;
	case BINDERR_CODE:
		error_msg = BINDERR;
		break ;
	case UNBLOKSOCERR_CODE:
		error_msg = UNBLOKSOCERR;
		break ;
	case POLERR_CODE:
		error_msg = POLSERR;
		break ;
	case ACCEPTERR_CODE:
		error_msg = ACCEPTERR;
		break ;
	case SENDERR_CODE:
		error_msg = SENDERR;
		break ;
	case RECVERR_CODE:
		error_msg = RECVERR;
		break ;
	case INVSERVNAME_CODE:
		error_msg = INVSERVNAME;
		break ;
	default:
		error_msg = "NOERROR";
		break ;
	}
	return (print_error(error_msg));
}

/*Print RED_COLOR message in TERMINAL 2*/
int	error::print_error( std::string &message )
{
	print_emessage(RED_COLOR, std::string("ERROR: " + message));
	return (1);
}

/*Print in TERMINAL 2*/
void	error::print_emessage( std::string color1, std::string message)
{
	std::cerr << color1 << message << STD_COLOR << std::endl;
}

/*Print color message(COLORS in colors.h)*/
void	error::print_message( std::string color1, std::string message)
{
	std::cout << color1 << message << STD_COLOR << std::endl;
}
