#include "../../class_hpp/channel.hpp"

chanell::chanell( user *user, std::string name ) : _l_mode(false), _i_mode(false), _t_mode(false), _k_mode(false), _o_mode(false), _limit(0), _name(name), _topic("")
{
	if (_name[0] != '#')
		_name.insert(0, "#");
	_operators.clear();
	_users.clear();
	_users.push_back(user);
	_operators.push_back(user);
}

chanell::~chanell()
{

}

void	chanell::changeMode( std::string modes, user *user )
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
		_l_mode = change;
	else if (modes.find_first_of('o') != modes.npos)
		_l_mode = change;
	else if (modes.find_first_of('t') != modes.npos)
		_l_mode = change;
	else if (modes.find_first_of('k') != modes.npos)
		_l_mode = change;
	else
	{
		user->msgToUsser("This mod unexist!\r\n");
		return ;
	}
	changes += ":" + user->getNickName() + " PRIVMSG " + _name + " ";
	if (_i_mode)
		change += "i";
	if (_l_mode)
		change += "l";
	if (_o_mode)
		change += "o";
	if (_t_mode)
		change += "t";
	if (_k_mode)
		change += "k";
	changes += "\r\n";
	sendMessage(changes);
}

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

void	chanell::sendMessage( std::string msg )
{
	for (std::vector<user *>::iterator i = _users.begin(); i != _users.end() ; i++)
		(*i)->msgToUsser(msg);
}

void	chanell::sendMessage( std::string msg, user *user )
{
	for (std::vector<class user *>::iterator i = _users.begin(); i != _users.end() ; i++)
	{
		if ((*i) != user)
			(*i)->msgToUsser(msg);
	}
}

int	chanell::removeUser( std::string nick, std::string cmd, std::string msg )
{
	std::vector<user *>::iterator user = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	if (user != _users.end() && cmd == "PART")
	{
		sendMessage(":" + (*user)->getNickName() + " PART " + _name + " " + msg + "\r\n");
		_users.erase(user);
	}
	else if (user != _users.end() && cmd == "KICK")
	{
		sendMessage(":" + (*user)->getNickName() + " KICK " + _name + " " + msg + "\r\n");
		_users.erase(user);
	}
	else
		return (0);
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

void	chanell::addUser( user *user )
{
	sendMessage(":" + user->getNickName() + " JOIN " + _name + "\r\n");
	_users.push_back(user);
	user->msgToUsser(":IRC 332 " + user->getNickName() + " " + _name + " " + _topic + "\r\n");
}

void	chanell::addOper( user *user )
{
	_operators.push_back(user);
}

bool	chanell::isInside( std::string nick )
{
	std::vector<class user *>::iterator check = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	if (check != _users.end())
		return (true);
	return (false);
}

bool	chanell::isOper( std::string nick )
{
	std::vector<class user *>::iterator check = std::find_if(_operators.begin(), _operators.end(), findByStrCh(nick));

	if (check != _operators.end())
		return (true);
	return (false);
}

bool	chanell::isAvailb( void )
{
	return ((_limit < (int)_users.size() || _limit == 0) ? true : false);
}

void		chanell::setTopic( std::string msg ) { _topic = msg; }
std::string	chanell::getName( void ) { return (_name); }
