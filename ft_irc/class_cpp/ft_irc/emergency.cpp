#include "../../class_hpp/ft_irc.hpp"

#include "cerrno"
#include "cstdio"

void	ft_irc::emergency_exit( short codeErr )
{
	if (codeErr != 0)
		_errorCode = codeErr;
	deleteUsers();
	error::error_index_print(_errorCode);
	error::print_emessage(MAGENTA_COLOR, "Server emergency closed!");
	delete (this);
	exit(1);
}

short	ft_irc::getErrorCode( void )
{
	return (_errorCode);
}

void	ft_irc::signalHandler( int sygnal )
{
	switch (sygnal)
	{
	case SIGTERM:
		error::print_emessage(BLUE_COLOR, SIGTERM_MESS);
		break ;
	case SIGINT:
		error::print_emessage(BLUE_COLOR, SIGINT_MESS);
		break ;
	default:
		return;
	}
	cleanExit(NULL);
	exit(sygnal);
}
