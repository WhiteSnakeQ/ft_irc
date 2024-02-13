#include "../../class_hpp/ft_irc.hpp"

/*Set exit code(only 1 time)*/
void	ft_irc::setExitCode( short code )
{
	if (_errorCode == 0)
		_errorCode = code;
}

/*Check for available nick*/
bool	ft_irc::checkFreeNick( std::string nick )
{
	std::vector<user *>::iterator i = std::find_if(_users.begin(), _users.end(), findByStrCh(nick));

	return (i == _users.end() ? true : false);
}
