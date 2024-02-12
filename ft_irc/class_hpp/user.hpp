#pragma once

#include "parser.hpp"
#include "iostream"
#include "netdb.h"
#include "arpa/inet.h"
#include "vector"
#include "unistd.h"
#include "error.hpp"

# define BUFFER_SIZE 2048

class user
{
private:
	int				_fd;
	std::string		_hostName;
	std::string		_IP;

	std::string		_nickName;
	std::string		_userName;

	std::vector<std::string>			_msgFromUser;
	std::vector<std::string>			_msgToUser;

	bool			_isRegistered;
	bool			_passProve;
public:
	user( int fd, std::string nickName, std::string IP );
	~user();

	void			msgToUsser( std::string msg );

	int				getfd( void );
	std::string		getHostName( void );
	std::string		getIP( void );

	std::string		getNickName( void );
	std::string		getUserName( void );
	std::string		getFull ( void );

	void			setNickName( std::string nickName );
	void			setUserName( std::string nickName );

	std::vector<std::string>	getMsgF( void );
	std::vector<std::string>	&getMsgT( void );

	int				msgRecv( void );
	void			printBuffer( void );

	bool			isReg( void );
	bool			pasProvide( void );
	void			updateStatus( void );
	void			updatePass( void );
};
