#include "../headers/ft_irc.h"
#include "../class_hpp/ft_irc.hpp"

int	main(int argc, char **argv)
{
	ft_irc	*main;

	main = new ft_irc(argc, argv);
	main->start_server();
	main->start_polling();
	delete (main);
	return (0);
}
