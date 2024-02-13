#include "../../class_hpp/user.hpp"

/*Cereate user with deff value*/
user::user( int fd, std::string hostName, std::string IP ) : _fd(fd), _hostName(hostName), _IP(IP), _nickName(""), _userName("Unregistered user"), _isRegistered(false), _passProve(false)
{
	return ;
}

/*Close fd*/
user::~user()
{
	if (_fd > 2)
		close(_fd);
	return ;
}

/*Change status to Register*/
void	user::updateStatus( void )
{
	if (!_isRegistered)
		_isRegistered = true;
}

/*Change pass and send msg*/
void	user::updatePass( void )
{
	if (!_passProve)
		_passProve = true;
	msgToUsser("Passwordd confirm\r\n");
}

/*Change nick and send msg*/
void	user::setNickName( std::string nickName ) 
{ 
	_nickName = nickName;
	msgToUsser(":Your nickname has been changed to " + nickName + "\r\n");
}

/*Change username and send msg*/
void	user::setUserName( std::string userName ) 
{ 
	_userName = userName;
	msgToUsser(":Your username has been changed to " + userName + "\r\n");
}

/*Print message from user by word*/
void	user::printBuffer( void )
{
	for (size_t i = 0; i < _msgFromUser.size(); i++)
		std::cout << _msgFromUser[i] << " " << i << std::endl;
}

/*Return info in format (nick!username@host)*/
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
