#include "../../class_hpp/ft_irc.hpp"

/*Remove user from server*/
void	ft_irc::kickUser( int fd )
{
	std::vector<user *>::iterator	_user = std::find_if(_users.begin(), _users.end(), FindByFD(fd));
	std::vector<struct pollfd>::iterator	pollfdR = std::find_if(_fds.begin(), _fds.end(), FindByFD(fd));
	user	*toDel = *_user;

	if (_user != _users.end())
	{
		error::print_message(MAGENTA_COLOR, "User disconnected USERNAME: " + (*_user)->getUserName());
		for (std::vector<chanell *>::iterator i = _channels.begin(); i != _channels.end(); i++)
			(*i)->removeUser((*_user)->getNickName(), "PART", ":user disconnected!");	
		_users.erase(_user);
		delete(toDel);
	}
	if(pollfdR != _fds.end())
		_fds.erase(pollfdR);
}

/*Connection message(Client req)*/
void	ft_irc::welcomeMessage( user *user )
{
	std::string	deff;
	std::string welcomeMsg;

	error::print_message(MAGENTA_COLOR, "New user register!\n");
	deff = user->getNickName() + "!" + user->getUserName() + "@" + user->getHostName();
	welcomeMsg = ":IRC 001 " + deff + " :Welcome to the Internet Relay Network " + user->getNickName() + "\n";
	user->msgToUsser(welcomeMsg);
	welcomeMsg = ":IRC 002 " + deff + " :Your host is " + _servName + ", running version V1.0\n";
	user->msgToUsser(welcomeMsg);
	welcomeMsg = ":IRC 003 " + deff + " :This server was created in February 2024\n";
	user->msgToUsser(welcomeMsg);
	welcomeMsg = ":IRC 004 " + deff + " :<" + _servName + "> <1.0> <NONE> <i't'k'o'l>\n";
	user->msgToUsser(welcomeMsg);
	user->updateStatus();
}

/*Clean all users*/
void	ft_irc::deleteUsers()
{
	for (size_t i = _users.size(); i > 0; i--)
		kickUser(_users.front()->getfd());
}

/*Clean all  channel*/
void	ft_irc::deleteChannels()
{
	for (size_t i = _channels.size(); i > 0; i--)
	{
		delete (_channels.front());
		_channels.erase(_channels.begin());
	}
		
}

/*Client register*/
void	ft_irc::registerUser( user *user, std::vector<std::string> msg )
{
	for (std::vector<std::string>::iterator i = msg.begin(); i != msg.end(); i++)
	{
		if ((*i) == "PASS" && i + 1 != msg.end())
		{
			if (*(i + 1) == _pass)
			{
				user->updatePass();
				continue ;
			}
			user->msgToUsser("Wrong Password!\r\n");
			break ;
		}
		else if ((*i) == "NICK" && i + 1 != msg.end())
		{
			if (!checkFreeNick(*(i + 1)))
				user->msgToUsser("Duplicate nickname!\r\n");
			else if  ((*(i + 1))[0] == '#')
				user->msgToUsser("Wrong  format of nick\r\n");
			else
				user->setNickName(*(i + 1));
		}
		else if ((*i) == "USER" && i + 1 != msg.end())
			user->setUserName(*(i + 1));
	}
	if (!user->pasProvide() || user->getNickName().length() <= 0 || user->getUserName() == "Unregistered user")
			kickUser(user->getfd());
	else
		welcomeMessage(user);
}
