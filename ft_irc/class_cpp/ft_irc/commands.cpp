#include "../../class_hpp/ft_irc.hpp"

void	ft_irc::commandHandler( user *user )
{
	std::vector<std::string>	msg;

	error::print_message(CYAN_COLOR, "Recive message from user " + user->getUserName());
	msg = user->getMsgF();
	if (!user->isReg())
		registerCmd(user, msg);
	else
		operatorCmd(user, msg);
}

void	ft_irc::registerCmd( user *user, std::vector<std::string> msg )
{
	if (msg.front() == "NICK" && msg.size() == 2)
	{
		if (msg[1][0] == '#')
			user->msgToUsser("Wrong  format of nick\r\n");
		else if (checkFreeNick(msg[1]))
			user->setNickName(msg[1]);
		else
			user->msgToUsser("Nick name is used!\r\n");
	}
	else if (msg.front() == "USER" && msg.size() == 2)
		user->setUserName(msg[1]);
	else if (msg.front() == "PASS" && msg.size() == 2)
	{
		if (user->pasProvide())
			user->msgToUsser("Password already confirm!\r\n");
		else if (msg[1] == _pass)
			user->updatePass();
		else
			user->msgToUsser("Wrong password!\r\n");
	}
	if (user->pasProvide() && user->getNickName().length() > 0 && user->getUserName() != "Unregistered user")
		welcomeMessage(user);
	if (msg.front() == "CAP")
		registerUser(user, msg);
	if (msg.size() == 1)
		user->msgToUsser("Invalid number of arguments!\r\n");
}

void	ft_irc::operatorCmd( user *user, std::vector<std::string> msg )
{
	if (msg.front() == "JOIN" && msg.size() > 1)
		join(user, msg[1]);
	else if (msg.front() == "PART" && msg.size() > 1)
		part(user, msg[1]);
	else if (msg.front() == "NICK" && msg.size() > 1)
		nick(user, msg[1]);
	else if (msg.front() == "USER" && msg.size() > 1)
		userCmd( user, msg[1] );
	else if (msg.front() == "PASS" && msg.size() > 1)
		user->msgToUsser("Password already confirm!\r\n");
	else if (msg.front() == "PRIVMSG" || msg.front() == "MSG")
		prvmsg(user, msg);
	else if (msg.front() == "KICK" && msg.size() > 1)
		kick(user, msg);
	else if (msg.front() == "INVITE" && msg.size() > 1)
		invite(user, msg);
	else if (msg.front() == "TOPIC" && msg.size() > 1)
		topic(user, msg);
	else if (msg.front() == "MODE" && msg.size() > 1)
		mode(user, msg);
	else if (msg.front() == "PING")
		user->msgToUsser("PONG\r\n");
	else if (msg.size() == 1)
		user->msgToUsser("Invalid arguments\r\n");
	else
		user->msgToUsser("This command is not supported!\r\n");
}

void	ft_irc::topic( user *user, std::vector<std::string> msg )
{
	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Chanel not exist!\r\n");
	else if (!(*ch)->isOper(user->getNickName()) || !(*ch)->getModeStatus('t'))
		user->msgToUsser("You dont have root!\r\n");
	else if (msg.size() < 3)
		user->msgToUsser("Invalid number of arguments!\r\n");
	else
		(*ch)->setTopic(parser::makeStrFromVector(msg, 1) + "\r\n");
}

void	ft_irc::prvmsg( user *user, std::vector<std::string> msg )
{
	std::string	message;

	if (msg.size() < 3)
	{
		user->msgToUsser("Invalid argument!\r\n");
		user->msgToUsser("Usage: PRIVMSG/MSG <nickname/channel> <message>\r\n");
		return ;
	}
	message = parser::makeStrFromVector(msg, 2);
	if (message[0] != ':')
		message.insert(0, ":");
	if (msg[1][0] == '#')
	{
		std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

		if (ch == _channels.end())
			user->msgToUsser("Channel is not exist!\r\n");
		else
			(*ch)->sendMessage(":" + user->getFull() + " PRIVMSG " + (*ch)->getName() + " " + message + "\r\n", user);
	}
	else
	{
		std::vector<class user *>::iterator usr = std::find_if(_users.begin(), _users.end(), findByStrCh(msg[1]));

		if (usr == _users.end())
			user->msgToUsser("User is not exist!\r\n");
		else
			(*usr)->msgToUsser(":" + user->getFull() + " PRIVMSG " + (*usr)->getNickName() + " " + message + "\r\n");
	}
}

void	ft_irc::invite( user *user, std::vector<std::string> msg )
{
	if (msg.size() < 3)
	{
		user->msgToUsser("Invalid arguments\r\n");
		user->msgToUsser("Usage: INVITE <channel name> <user nickname>\r\n");
		return ;
	}
	if (msg[1][0] != '#')
	{
		user->msgToUsser("Channels name start from '#'\r\n");
		return ;
	}

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Channel was not creating\r\n");
	else if (!(*ch)->isOper(user->getNickName()) || (*ch)->getModeStatus('o'))
		user->msgToUsser("You dont have root!\r\n");
	else if ((*ch)->isAvailb())
		(*ch)->addUser(user);
	else
		user->msgToUsser("Channel is full\r\n");
}

void	ft_irc::nick( user *user, std::string msg )
{
	if (msg[0] == '#')
		user->msgToUsser("Wrong  format of nick\r\n");
	else if (checkFreeNick(msg))
		user->setNickName(msg);
	else
		user->msgToUsser("Nick name is used!\r\n");
}

void	ft_irc::userCmd( user *user, std::string msg )
{
	if (user->isReg())
		user->msgToUsser("You cant change username to " + msg + ", you already login!\r\n");
}

void	ft_irc::join( user *user, std::string msg )
{
	if(msg[0] != '#')
		msg.insert(0, "#");

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg));

	if (ch != _channels.end())
	{
		if ((*ch)->getModeStatus('i'))
			user->msgToUsser("Channel is unvite-only!\r\n");
		else if (!(*ch)->isAvailb())
			user->msgToUsser("Channel is full!\r\n");
		else if (!(*ch)->isInside(user->getNickName()))
			(*ch)->addUser(user);
		else
			user->msgToUsser("You already Join!\r\n");
	}
	else
	{
		_channels.push_back(new chanell(user, msg));
		user->msgToUsser("You raised to operator of channel " + msg + "\r\n");
	}
}

void	ft_irc::part( user *user, std::string msg )
{
	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg));

	if (msg[0] != '#')
	{
		user->msgToUsser("Channels name start from '#'\r\n");
		return ;
	}
	if (ch == _channels.end())
	{
		user->msgToUsser("Unexisting channel\r\n");
		return ;
	}
	else
	{
		short	err;
	
		err = (*ch)->removeUser(user->getNickName(), "PART", ":Byby");
		if (err == -2)
			emergency_exit(SENDERR_CODE);
		user->msgToUsser("You leave channel " + (*ch)->getName() + "\r\n");
		if (err == -1)
		{
			delete (*ch);
			_channels.erase(ch);
			return ;
		}
	}
}

void	ft_irc::kick( user *user, std::vector<std::string> msg )
{
	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Channel not exist\r\n");
	else if ((*ch)->getModeStatus('o'))
		user->msgToUsser("Operator in this channel cant  do this!\r\n");
	else if (!(*ch)->isOper(user->getNickName()))
		user->msgToUsser("You are not operator!\r\n");
	else if (msg[2] == user->getNickName())
		user->msgToUsser("Cant kick your self!");
	else if ((*ch)->isInside(msg[2]))
		(*ch)->removeUser(msg[2], msg[0], msg[3]);
	else
		user->msgToUsser("User not in channel!\r\n");
}

void	ft_irc::mode( user *user, std::vector<std::string> msg )
{
	if (msg.size() != 3 || (msg[2][0] != '+' && msg[2][1] != '-'))
	{
		user->msgToUsser("Invalid format of Command!\n\r");
		user->msgToUsser("Usage: MODE <channel name> <+/-><modes>\n\r");
		return ;
	}
	if (msg[1][0] != '#')
		return ;
	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));
	if (ch == _channels.end())
		user->msgToUsser("Channel is not exist!\n\r");
	else if (!(*ch)->isOper(user->getNickName()))
		user->msgToUsser("You dot have root!\n\r");
	else if ((*ch)->getModeStatus('o'))
		user->msgToUsser("Operator in this channel cant do this!\r\n");
	else if (msg[2].length() != 2)
		user->msgToUsser("Wrong mode!\r\n");
	else
		(*ch)->changeMode(msg[2], user);
}
