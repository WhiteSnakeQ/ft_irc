#include "../../class_hpp/ft_irc.hpp"

/*Prepair server for polling, create socket*/
void	ft_irc::start_server( void )
{
	pollfd	servPoll;

	if (_errorCode != 0)
		emergency_exit(0);
	createSocket();
	bindSocket();
	listenSocket();
	unblocSocket();
	servPoll.fd = _socketfd;
	servPoll.revents = 0;
	servPoll.events = POLLIN;
	_fds.push_back(servPoll);
	error::print_message(YELLOW_COLOR, "Server open!");
}

/*Polling, loop*/
void	ft_irc::start_polling( void )
{
	error::print_message(CYAN_COLOR, "Start polling!");
	while (42)
	{
		if ((poll(_fds.data(), _fds.size(), -1)) == -1)
			emergency_exit(POLERR_CODE);
		error::print_message(GREEN_COLOR, "New request accepted!");
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _socketfd)
				{
					addNewFd();
					continue ;
				}

				user	*user = *std::find_if(_users.begin(), _users.end(), FindByFD(_fds[i].fd));
				int		state;

				user->getMsgF().clear();
				if ((state = user->msgRecv()) == -1)
					emergency_exit(RECVERR_CODE);
				else if (state == 0)
					kickUser(_fds[i--].fd);
				else if (user->getMsgF().size() == 0)
					continue ;
				else if (user->getMsgF()[0] == "QUIT")
				{
					user->msgToUsser("QUIT\r\n");
					kickUser(_fds[i--].fd);
				}
				else
					commandHandler(user);
			}
		}
	}
}
