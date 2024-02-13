#include "../../class_hpp/channel.hpp"

/*Create channel, set all deff variable and send msg*/
chanell::chanell( user *user, std::string name ) : _l_mode(false), _i_mode(false), _t_mode(false), _k_mode(false), _o_mode(false), _limit(0), _pass(""), _name(name), _topic("")
{
	if (_name[0] != '#')
		_name.insert(0, "#");
	_operators.clear();
	_users.clear();
	_users.push_back(user);
	_operators.push_back(user);
	user->msgToUsser(":IRC 332 " + user->getNickName() + " " + _name + " " + _topic + "\r\n");
	sendMessage(":" + user->getNickName() + " JOIN " + _name + "\r\n");
}

chanell::~chanell()
{

}

/*Change mode: Usage (<+/->mode addInfo)*/
void	chanell::changeMode( std::string modes, user *user, std::string addInfo )
{
	bool		change;
	std::string	changes = "";

	if (modes[0] == '+')
		change = true;
	else
		change = false;
	if (modes.find_first_of('i') != modes.npos)
		_i_mode = change;
	else if (modes.find_first_of('l') != modes.npos)
	{
		if (change)
		{
			if (parser::is_number(addInfo) && addInfo.length() > 0)
			{
				if (std::atoi(addInfo.c_str()) < (int)_users.size())
				{
					user->msgToUsser("Not valid number, too much users at channel!\r\n");
					return ;
				}
				else
					_limit = std::atoi(addInfo.c_str());
			}
			else
			{
				user->msgToUsser("Invalid arguments!\r\n");
				user->msgToUsser("Usage: MODE +l <number>\r\n");
				return ;
			}
		}
		else
			_limit = 0;
		_l_mode = change;
	}
	else if (modes.find_first_of('o') != modes.npos)
		_o_mode = change;
	else if (modes.find_first_of('t') != modes.npos)
		_t_mode = change;
	else if (modes.find_first_of('k') != modes.npos)
	{
		if (addInfo.length() > 0 && change)
			_pass = addInfo;
		else if (change)
		{
			user->msgToUsser("Invalid arguments!\r\n");
			user->msgToUsser("Usage: MODE <+>k <password>\r\n");
			return ;
		}
		else
			_pass = "";
		_k_mode = change;
	}
	else
	{
		user->msgToUsser("This mod unexist!\r\n");
		return ;
	}
	changes += ":" + user->getNickName() + " PRIVMSG " + _name + " " + "modes changed to <";
	if (_i_mode)
		changes += "i";
	if (_l_mode)
		changes += "l";
	if (_o_mode)
		changes += "o";
	if (_t_mode)
		changes += "t";
	if (_k_mode)
		changes += "k";
	changes += ">\r\n";
	sendMessage(changes);
}

/*Return mode status*/
bool	chanell::getModeStatus( char mode )
{
	if (mode == 'i')
		return (_i_mode);
	if (mode == 'l')
		return (_l_mode);
	if (mode == 'o')
		return (_o_mode);
	if (mode == 'k')
		return (_k_mode);
	if (mode == 't')
		return (_t_mode);
	else
		return (false);
}

/*Send msg to all channel*/
void	chanell::sendMessage( std::string msg )
{
	for (std::vector<user *>::iterator i = _users.begin(); i != _users.end() ; i++)
		(*i)->msgToUsser(msg);
}

/*Send msg from one user to all channel or send to bot*/
void	chanell::sendMessage( std::string msg, user *user )
{
	std::vector<std::string> checkBot = parser::makeVectorString(msg);

	if (checkBot[3] == ":BOT" || checkBot[3] == "BOT")
	{
		user->msgToUsser(bot::cmdHandler(_users, parser::makeVectorString(msg), _name));
		return ;
	}
	for (std::vector<class user *>::iterator i = _users.begin(); i != _users.end() ; i++)
	{
		if ((*i) != user)
			(*i)->msgToUsser(msg);
	}
}

/*Remove user from channel and send all msg*/
int	chanell::removeUser( std::string nick, std::string cmd, std::string msg )
{
	std::vector<user *>::iterator user = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	if (user != _users.end() && cmd == "PART")
		sendMessage(":" + (*user)->getNickName() + " PART " + _name + " " + msg + "\r\n");
	else if (user != _users.end() && cmd == "KICK")
		sendMessage(":" + (*user)->getNickName() + " KICK " + _name + " " + msg + "\r\n");
	else
		return (0);
	_users.erase(user);
	user = std::find_if(_operators.begin(), _operators.end(), findByStrCh(nick));
	if (user != _operators.end())
		_operators.erase(user);
	if (_operators.size() <= 0)
	{
		if (_users.size() <= 0)
			return (-1);
		_operators.push_back(_users.front());
		_users.front()->msgToUsser("You raised to operator of channel " + _name);
	}
	return (0);
}

/*Add user to channel, and send all msg*/
void	chanell::addUser( user *user, std::string status )
{
	_users.push_back(user);
	user->msgToUsser(":IRC 332 " + user->getNickName() + " " + _name + " " + _topic + "\r\n");
	sendMessage(":" + user->getNickName() + " " + status + " " + _name + "\r\n");
}

/*Add oper root*/
void	chanell::addOper( user *user )
{
	_operators.push_back(user);
}

/*Check for user in channel*/
bool	chanell::isInside( std::string nick )
{
	std::vector<class user *>::iterator check = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	return (check != _users.end() ? true : false);
}

bool	chanell::isOper( std::string nick ) //check for oper root
{
	std::vector<class user *>::iterator check = std::find_if(_operators.begin(), _operators.end(), findByStrCh(nick));

	return (check != _operators.end() ? true : false);
}

bool	chanell::isAvailb( void )	//check free place for new user (l flag)
{
	return ((_limit < (int)_users.size() || _limit == 0) ? true : false);
}

void		chanell::setTopic( std::string msg ) { _topic = msg; }
std::string	chanell::getName( void ) { return (_name); }
std::string chanell::getPass( void ) { return (_pass); }
std::string chanell::getTopic( void ) { return (_topic); }
