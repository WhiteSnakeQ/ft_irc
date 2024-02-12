#include "../../class_hpp/ft_irc.hpp"

void	ft_irc::setExitCode( short code )
{
	if (_errorCode == 0)
		_errorCode = code;
}

bool	ft_irc::checkFreeNick( std::string nick )
{
	std::vector<user *>::iterator i = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	return (i == _users.end() ? true : false);
}
