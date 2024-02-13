#pragma once

#include "user.hpp"
#include "bot.hpp"

class chanell
{
private:
	bool				_l_mode;
	bool				_i_mode;
	bool				_t_mode;
	bool				_k_mode;
	bool				_o_mode;

	int					_limit;
	std::string			_pass;

	std::string			_name;
	std::string			_topic;
	std::vector<user *>	_users;
	std::vector<user *>	_operators;

	static bot			_bot;
public:
	chanell( user *user, std::string name );
	~chanell();

	void			sendMessage( std::string msg );
	void			sendMessage( std::string msg, user *user );
	int				removeUser( std::string nick, std::string cmd, std::string msg );

	void			addUser( user *user, std::string status );
	void			addOper( user *user );

	bool			isInside( std::string nick );
	bool			isOper( std::string nick );
	bool			isAvailb( void );

	std::string		getName( void );
	std::string		getPass( void );
	std::string		getTopic( void );
	void			setTopic( std::string msg );

	bool			getModeStatus( char mode );
	void			changeMode( std::string mode, user *user, std::string addInfo );
};

struct findByStrCh
{
	std::string	str;

	findByStrCh(std::string str) : str(str) {}

	bool operator()(user *usr) const { return (usr->getNickName() == str); }
	bool operator()(chanell *ch) const { return (ch->getName() == str); }
};