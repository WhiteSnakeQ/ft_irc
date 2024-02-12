#include "../headers/ft_irc.h"
#include "../class_hpp/ft_irc.hpp"

int	main(int argc, char **argv)
{
	ft_irc	*main;
	short	error_index;

	main = new ft_irc(argc, argv);
	error_index = main->getErrorCode();
	if (error_index != 0)
	{
		error::error_index_print(error_index);
		delete (main);
		return(1);
	}
	main->start_server();
	main->start_polling();
	delete (main);
	return (0);
}
