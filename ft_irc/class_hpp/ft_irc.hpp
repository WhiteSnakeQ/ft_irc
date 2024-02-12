#pragma once

#include "../headers/ft_irc.h"
#include "error.hpp"
#include "parser.hpp"
#include "channel.hpp"
#include "user.hpp"

class ft_irc
{
private:
	std::string		_servName;

	short			_errorCode;

	int				_port;
	std::string		_pass;

	int				_socketfd;

	std::vector<pollfd>	_fds;
	std::vector<class user *>	_users;
	std::vector<class chanell *>	_channels;

	void		createSocket( void );
	void		bindSocket( void );
	void		listenSocket( void );
	void		unblocSocket( void );

	int			addNewFd( void );

	void		setExitCode( short code );
	void		emergency_exit( short codeErr );

	void		commandHandler( user *user );

	void		registerCmd( user *user, std::vector<std::string> msg );
	void		operatorCmd( user *user, std::vector<std::string> msg );

	void		join( user *user, std::string msg );
	void		part( user *user, std::string msg );
	void		prvmsg( user *user, std::vector<std::string> msg );
	void		kick( user *user, std::vector<std::string> msg );
	void		nick( user *user, std::string msg );
	void		userCmd( user *user, std::string msg );
	void		invite( user *user, std::vector<std::string> msg );
	void		topic( user *user, std::vector<std::string> msg );
	void		mode( user *user, std::vector<std::string> msg );
	void		quit( user *user, std::string msg );

	void		registerUser( user *user, std::vector<std::string> msg );
	void		kickUser( int fd );
	void		welcomeMessage( user *user);

	bool		checkFreeNick( std::string nick );

	void		deleteUsers( void );
	void		deleteChannels( void );

	static void	signalHandler( int signal );
public:
	ft_irc( int argc, char **argv );
	~ft_irc();

	void		start_server( void );
	void		start_polling( void );

	short		getErrorCode( void );

	static void	cleanExit( ft_irc *irc );
};

struct FindByFD
{
	int fd;

    FindByFD(int fd) : fd(fd) { }

    bool operator()(user *user) const { return (user->getfd() == fd); }
    bool operator()(const struct pollfd& pfd) const { return (pfd.fd == fd); }
};
