#include "../../../class_hpp/bot.hpp"
#include "../../../class_hpp/user.hpp"

/*Return random number from msg[6] to msg[7]*/
std::string	bot::randNumb( std::vector<std::string> msg )
{
	if (msg.size() != 7 || !parser::is_number(msg[5]) || !parser::is_number(msg[6]))
		return ("Invalid argument -- Usage: BOT RAND <digit> <digit>");
	
	int	d1 = std::atoi(msg[5].c_str());
	int	d2 = std::atoi(msg[6].c_str());

	std::cout << YELLOW_COLOR << d1 << d2 << STD_COLOR << std::endl;
	if (d1 > d2)
		return ("Invalid argument -- Second digit must bigger than first");
	int	randNB = (rand() + d1) % (d2 - d1 + 1);
	std::cout << YELLOW_COLOR << randNB << STD_COLOR << std::endl;
	return ("Your digit is " + parser::convertIntToStr(randNB));
}

/*list of users*/
std::string	bot::getUsersInfo( std::vector<user *> users )
{
	std::string	toRet;
	bool		first = true;

	toRet = "List of users:";
	for (std::vector<user *>::iterator i = users.begin(); i != users.end(); i++)
	{
		if (!first)
			toRet += " | ";
		toRet += (*i)->getNickName() + " ";
		toRet += (*i)->getUserName() + " ";
		first = false;
	}
	return (toRet);
}

/*Simple bot*/
std::string	bot::cmdHandler( std::vector<user *> users, std::vector<std::string> msg, std::string chName )
{
	std::string	toRet;

	parser::toupper(msg[4]);
	toRet = ":BOT." + chName + " " + msg[1] + " " + msg[2] + " ";
	if (msg.size() <= 4)
		return (toRet +"Invalid arguments -- Use: BOT HELP!\r\n");
	else if (msg[4] == "HELP")
		return (toRet + "BOT available commands: INFO, TIME, RAND\r\n");
	else if (msg[4] == "RAND")
		return (toRet + randNumb(msg) + "\r\n");
	else if (msg[4] == "TIME")
	{
		time_t	now = time(0);
		return (toRet + ctime(&now) + "\r\n");
	}
	else if (msg[4] == "INFO")
		return (toRet + getUsersInfo(users) + "\r\n");
	else
		return (toRet +"Invalid arguments -- Use: BOT HELP!\r\n");
}
