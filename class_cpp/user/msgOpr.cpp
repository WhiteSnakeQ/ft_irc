#include "../../class_hpp/user.hpp"
#include "../../class_hpp/ft_irc.hpp"

#include "cstring"

/*Recive msg from user(Handle error)*/
int	user:: msgRecv( void )
{
	char	str[BUFFER_SIZE];
	int		recv_b;

	std::memset(str, 0, sizeof(str));
	if ((recv_b = recv(_fd, str, BUFFER_SIZE, 0)) == -1)
	{
		error::print_emessage(RED_COLOR, RECVERR);
		ft_irc::cleanExit(NULL);
	}
	_msgFromUser = parser::makeVectorString(str);
	if (_msgFromUser.size() > 0)
	{
		parser::toupper(_msgFromUser[0]);
		if (_msgFromUser[0][0] == '/')
			_msgFromUser[0].erase(0);
	}
	return (recv_b);
}

/*Send msg to user(Handle error)*/
void	user::msgToUsser( std::string msg )
{
	if (send(_fd, msg.c_str(), msg.length(), 0) == -1)
	{
		error::print_emessage(RED_COLOR, SENDERR);
		ft_irc::cleanExit(NULL);
	}
}
