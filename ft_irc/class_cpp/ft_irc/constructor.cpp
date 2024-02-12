#include "../../class_hpp/ft_irc.hpp"

ft_irc::ft_irc( int argc, char **argv ) : _servName("ft_irc"), _errorCode(0), _port(0), _pass("0000"), _socketfd(0)
{
	std::string	to_check;

	cleanExit(this);
	if (_servName.length() == 0)
		setExitCode(INVSERVNAME_CODE);
	if ( argc == 3 )
	{
		to_check = argv[1];
		if (!parser::parse_port(to_check))
			setExitCode(INVPORT_CODE);
		else
		{
			_port = std::atoi(argv[1]);
				to_check = std::string(argv[2]);
			if (to_check.length() < 4 || to_check.length() > 10 || !parser::is_number(to_check))
				setExitCode(INVPASS_CODE);
			else
				_pass = argv[2];
		}
	}
	else
		setExitCode(INVARG_CODE);
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
}

void	ft_irc::cleanExit( ft_irc *irc )
{
	static ft_irc	*clean;

	if (irc)
		clean = irc;
	else
		delete (clean);
}

ft_irc::~ft_irc()
{
	deleteUsers();
	deleteChannels();
	if (_socketfd != 0)
	{
		error::print_message(YELLOW_COLOR, "Server closed");
		close(_socketfd);
	}
	return ;
}
