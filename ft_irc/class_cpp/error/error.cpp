#include "../../class_hpp/error.hpp"

int	error::error_index_print( int error )
{
	std::string	error_pr;

	switch (error)
	{
	case INVARG_CODE:
		error_pr = INVARG;
		break ;
	case INVPORT_CODE:
		error_pr = INVPORT;
		break ;
	case INVPASS_CODE:
		error_pr = INVPASS;
		break ;
	case SOCKERR_CODE:
		error_pr = SOCKERR;
		break ;
	case BINDERR_CODE:
		error_pr = BINDERR;
		break ;
	case UNBLOKSOCERR_CODE:
		error_pr = UNBLOKSOCERR;
		break ;
	case POLERR_CODE:
		error_pr = POLSERR;
		break ;
	case ACCEPTERR_CODE:
		error_pr = ACCEPTERR;
		break ;
	case SENDERR_CODE:
		error_pr = SENDERR;
		break ;
	case RECVERR_CODE:
		error_pr = RECVERR;
		break ;
	case INVSERVNAME_CODE:
		error_pr = INVSERVNAME;
		break ;
	default:
		error_pr = "NOERROR";
		break ;
	}
	return (print_error(error_pr));
}

int	error::print_error( std::string &message )
{
	print_emessage(RED_COLOR, std::string("ERROR: " + message));
	return (1);
}

void	error::print_emessage( std::string color1, std::string message)
{
	std::cerr << color1 << message << STD_COLOR << std::endl;
}

void	error::print_message( std::string color1, std::string message)
{
	std::cout << color1 << message << STD_COLOR << std::endl;
}
