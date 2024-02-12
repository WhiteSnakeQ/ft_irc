#include "../../class_hpp/user.hpp"

user::user( int fd, std::string hostName, std::string IP ) : _fd(fd), _hostName(hostName), _IP(IP), _nickName(""), _userName("Unregistered user"), _isRegistered(false), _passProve(false)
{
	return ;
}

user::~user()
{
	if (_fd != -1)
		close(_fd);
	return ;
}

void	user::updateStatus( void )
{
	if (!_isRegistered)
		_isRegistered = true;
}

void	user::updatePass( void )
{
	if (!_passProve)
		_passProve = true;
	msgToUsser("Passwordd confirm\r\n");
}

void	user::printBuffer( void )
{
	for (size_t i = 0; i < _msgFromUser.size(); i++)
		std::cout << _msgFromUser[i] << " " << i << std::endl;
}

void	user::setNickName( std::string nickName ) 
{ 
	_nickName = nickName;
	msgToUsser(":Your nickname has been changed to " + nickName + "\r\n");
}

void	user::setUserName( std::string userName ) 
{ 
	_userName = userName;
	msgToUsser(":Your username has been changed to " + userName + "\r\n");
}

std::string	user::getFull( void )
{
	std::string	toRet;

	toRet = _nickName + "!" + _userName + "@" + _hostName;
	return (toRet);
}

bool	user::isReg( void ) { return (_isRegistered); }
bool	user::pasProvide( void ) { return (_passProve); }
int	user::getfd( void ) { return (_fd); }
std::string user::getIP( void ) { return (_IP); }
std::string user::getHostName( void ) { return (_hostName); }
std::string user::getNickName( void ) {	return (_nickName); }
std::string user::getUserName( void ) { return (_userName); }
std::vector<std::string>	user::getMsgF( void ) { return (_msgFromUser); }
std::vector<std::string>	&user::getMsgT( void ) { return (_msgToUser); }
