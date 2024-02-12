#pragma once

#include "user.hpp"

class chanell
{
private:
	bool				_l_mode;
	bool				_i_mode;
	bool				_t_mode;
	bool				_k_mode;
	bool				_o_mode;

	int					_limit;

	std::string			_name;
	std::string			_topic;
	std::vector<user *>	_users;
	std::vector<user *>	_operators;

public:
	chanell( user *user, std::string name );
	~chanell();

	void			sendMessage( std::string msg );
	void			sendMessage( std::string msg, user *user );
	int				removeUser( std::string nick, std::string cmd, std::string msg );

	void			addUser( user *user );
	void			addOper( user *user );

	bool			isInside( std::string nick );
	bool			isOper( std::string nick );
	bool			isAvailb( void );

	std::string		getName( void );
	void			setTopic( std::string msg );

	bool			getModeStatus( char mode );
	void			changeMode( std::string mode, user *user );
};

struct findByStrCh
{
	std::string	str;

	findByStrCh(std::string str) : str(str) {}

	bool operator()(user *usr) const { return (usr->getNickName() == str); }
	bool operator()(chanell *ch) const { return (ch->getName() == str); }
};