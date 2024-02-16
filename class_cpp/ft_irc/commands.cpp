#include "../../class_hpp/ft_irc.hpp"

/*Main command handler, register/unregister*/
void	ft_irc::commandHandler( user *user )
{
	std::vector<std::string>	msg;

	error::print_message(CYAN_COLOR, "Recive message from user " + user->getUserName() + "\\" + user->getNickName());
	msg = user->getMsgF();
	if (!user->isReg())
		registerCmd(user, msg);
	else
		operatorCmd(user, msg);
}

/*Register without client*/
void	ft_irc::registerCmd( user *user, std::vector<std::string> msg )
{
	if (msg.front() == "CAP")
	{
		registerUser(user, msg);
		return ;
	}
	else if (msg.front() == "NICK" && msg.size() == 2)
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
	else
		user->msgToUsser("You are not register!\nUse PASS/NICK/USER\r\n");
	if (user->pasProvide() && user->getNickName().length() > 0 && user->getUserName() != "Unregistered user")
		welcomeMessage(user);
}

/*Choose operation*/
void	ft_irc::operatorCmd( user *user, std::vector<std::string> msg )
{
	if (msg.front() == "JOIN" && msg.size() > 1)
		join(user, msg);
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
}

/*Change topic, send all msg*/
void	ft_irc::topic( user *user, std::vector<std::string> msg )
{
	if (msg[1][0] != '#')
		msg[1].insert(0, "#");

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Chanel not exist!\r\n");
	else if (!(*ch)->isOper(user->getNickName()) || (*ch)->getModeStatus('t'))
		user->msgToUsser("NOTICE " + msg[1] + " :" + user->getNickName() + " You dont have root!\r\n");
	else if (msg.size() < 3)
		user->msgToUsser("NOTICE " + msg[1] + " :" + user->getNickName() + " Invalid number of arguments!\r\n");
	else
	{
		(*ch)->setTopic(parser::makeStrFromVector(msg, 2));
		(*ch)->sendMessage(":" + user->getNickName() + " TOPIC " + (*ch)->getName() + " " + (*ch)->getTopic() + "\r\n");
	}
}

/*Send message to user/channel*/
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
		else if ((*ch)->isInside(user->getNickName()))
			(*ch)->sendMessage(":" + user->getFull() + " PRIVMSG " + (*ch)->getName() + " " + message + "\r\n", user);
		else
			user->msgToUsser("You are not in channel!\r\n");
	}
	else
	{
		std::vector<class user *>::iterator usr = std::find_if(_users.begin(), _users.end(), findByStrCh(msg[1]));

		if (usr == _users.end())
			user->msgToUsser("User is not exist!\r\n");
		else
			(*usr)->msgToUsser(":" + user->getNickName() + " PRIVMSG " + (*usr)->getNickName() + " " + message + "\r\n");
	}
}

/*Invite user to channel*/
void	ft_irc::invite( user *user, std::vector<std::string> msg )
{
	if (msg.size() < 3)
	{
		user->msgToUsser("Invalid arguments\r\n");
		user->msgToUsser("Usage: INVITE <user nickname> <channel name>\r\n");
		return ;
	}
	if (msg[2][0] != '#')
		msg[2].insert(0, "#");

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[2]));
	std::vector<class user *>::iterator inv = std::find_if(_users.begin(), _users.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Channel was not creating\r\n");
	else if (!(*ch)->isOper(user->getNickName()) || (*ch)->getModeStatus('o'))
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " You dont have root!\r\n");
	else if (inv == _users.end())
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Unexisting user!\r\n");
	else if ((*ch)->isInside(msg[1]))
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " User alredy inside!\r\n");
	else if (msg[1] == user->getNickName())
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Cant add your self!\r\n");
	else if ((*ch)->isAvailb())
		(*ch)->addUser(*inv, "INVITE");
	else
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Channel is full\r\n");
}

/*Add user tochannel and send msg*/
void	ft_irc::join( user *user, std::vector<std::string> msg )
{
	if(msg[1][0] != '#')
		msg[1].insert(0, "#");

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch != _channels.end())
	{
		if ((*ch)->getModeStatus('k') && msg.size() < 3)
			user->msgToUsser("Password required!\r\n");
		else if ((*ch)->getModeStatus('k') && msg[2] != (*ch)->getPass())
			user->msgToUsser("Wrong password\r\n");
		else if ((*ch)->getModeStatus('i'))
			user->msgToUsser("Channel is invite-only!\r\n");
		else if (!(*ch)->isAvailb())
			user->msgToUsser("Channel is full!\r\n");
		else if (!(*ch)->isInside(user->getNickName()))
			(*ch)->addUser(user, "JOIN");
		else
			user->msgToUsser("You already Join!\r\n");
	}
	else
	{
		_channels.push_back(new chanell(user, msg[1]));
		user->msgToUsser("NOTICE " + _channels.back()->getName() + " :" + user->getNickName() + " You raised to operator of channel " + msg[1] + "\r\n");
	}
}

/*Remove user from channel and send msg*/
void	ft_irc::part( user *user, std::string msg )
{

	if (msg[0] != '#')
		msg.insert(0, "#");

	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg));

	if (ch == _channels.end())
		user->msgToUsser("Unexisting channel\r\n");
	else
	{
		short	err;

		err = (*ch)->removeUser(user->getNickName(), "PART", ":Byby");
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " You leave channel " + (*ch)->getName() + "\r\n");
		if (err == -2)
		{
			delete (*ch);
			_channels.erase(ch);
		}
	}
}

/*Kick user and send msg*/
void	ft_irc::kick( user *user, std::vector<std::string> msg )
{
	std::vector<chanell *>::iterator ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));

	if (ch == _channels.end())
		user->msgToUsser("Channel not exist\r\n");
	else if (msg.size() != 4)
	{
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Invalid command!\r\n");
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Usage: KICK <channel name> <user> <message>\r\n");
	}
	else if (!(*ch)->isOper(user->getNickName()))
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " You are not operator!\r\n");
	else if ((*ch)->getModeStatus('o'))
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Operator in this channel cant do this!\r\n");
	else if (msg[2] == user->getNickName())
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Cant kick your self!\r\n");
	else if ((*ch)->isInside(msg[2]))
		(*ch)->removeUser(msg[2], msg[0], parser::makeStrFromVector(msg, 3));
	else
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " User not in channel!\r\n");
}

/*Change modes and send msg*/
void	ft_irc::mode( user *user, std::vector<std::string> msg )
{
	std::vector<chanell *>::iterator ch;

	if (msg.size() > 1 && msg[1] == user->getFull())
		return ;
	if (msg.size() > 1)
			ch = std::find_if(_channels.begin(), _channels.end(), findByStrCh(msg[1]));
	if (msg.size() < 3 || (msg[2][0] != '+' && msg[2][0] != '-') || msg[1][0] != '#')
	{
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Invalid format of Command!\n\r");
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Usage: MODE <channel name> <+/-><modes>\n\r");
		return ;
	}

	if (ch == _channels.end())
		user->msgToUsser("Channel is not exist!\n\r");
	else if (!(*ch)->isOper(user->getNickName()))
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " You dot have root!\n\r");
	else if (msg[2].length() != 2)
		user->msgToUsser("NOTICE " + (*ch)->getName() + " :" + user->getNickName() + " Wrong mode!\r\n");
	else if (msg.size() == 4)
		(*ch)->changeMode(msg[2], user, msg[3]);
	else
		(*ch)->changeMode(msg[2], user, "");
}

/*Change nick and send msg*/
void	ft_irc::nick( user *user, std::string msg )
{
	if (msg[0] == '#')
		user->msgToUsser("Wrong  format of nick\r\n");
	else if (checkFreeNick(msg))
		user->setNickName(msg);
	else
		user->msgToUsser("Nick name is used!\r\n");
}

/*Just send error*/
void	ft_irc::userCmd( user *user, std::string msg )
{
	if (user->isReg())
		user->msgToUsser("You cant change username to " + msg + ", you already login!\r\n");
}
