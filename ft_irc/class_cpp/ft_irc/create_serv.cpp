#include "../../class_hpp/ft_irc.hpp"

void	ft_irc::createSocket( void )
{
	int	enable;

	if ((_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		emergency_exit(SOCKERR_CODE);
	error::print_message(MAGENTA_COLOR, "Socked created!");
	enable = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
		emergency_exit(SETSOCKERR_CODE);
	error::print_message(MAGENTA_COLOR, "Socked options set!");
}

void	ft_irc::bindSocket( void )
{
	struct sockaddr_in addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	if (bind(_socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		emergency_exit(BINDERR_CODE);
	error::print_message(MAGENTA_COLOR, "Connect local address and socket!");
}

void	ft_irc::listenSocket( void )
{
	if (listen(_socketfd, MAX_CLIENT) == -1)
		emergency_exit(BINDERR_CODE);
	error::print_message(MAGENTA_COLOR, "Socket listen!");
}

void	ft_irc::unblocSocket( void )
{
	if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) < 0)
		emergency_exit(UNBLOKSOCERR_CODE);
	error::print_message(MAGENTA_COLOR, "Socket unblock!");
}
