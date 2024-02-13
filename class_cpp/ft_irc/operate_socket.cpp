#include "../../class_hpp/ft_irc.hpp"

/*Catch conections IP4/IP6*/
int	ft_irc::addNewFd( void )
{
	int						newFd;
	struct sockaddr_storage clientInfo;
	socklen_t				sizeStruct;
	char					clientIP[INET6_ADDRSTRLEN];
	char					clientHost[NI_MAXHOST];

	sizeStruct = sizeof(clientInfo);

	if ((newFd = accept(_socketfd, (struct sockaddr *)&clientInfo, &sizeStruct)) == -1)
		emergency_exit(ACCEPTERR_CODE);

	if (clientInfo.ss_family == AF_INET)
		inet_ntop(AF_INET, &(((struct sockaddr_in *)&clientInfo)->sin_addr), clientIP, INET_ADDRSTRLEN);
	else if (clientInfo.ss_family == AF_INET6)
		inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)&clientInfo)->sin6_addr), clientIP, INET6_ADDRSTRLEN);
	else
	{
		error::print_message(RED_COLOR, "Unreadable address family");
		return -1;
	}

	if (getnameinfo((struct sockaddr *)&clientInfo, sizeStruct, clientHost, NI_MAXHOST, NULL, 0, NI_NUMERICSERV))
	{
		error::print_emessage(RED_COLOR, "Unable get user name. User name will be change to \"Unknown\"");
		std::strcpy(clientHost, "Unknown");
	}

	std::cout << MAGENTA_COLOR << "New connection from " << clientIP << " (Hostname: " << clientHost << ")" << STD_COLOR << std::endl;
	
	pollfd tmp2 = {newFd, POLLIN, 0};
	_fds.push_back(tmp2);

	_users.push_back(new user(newFd, clientIP, clientHost));
	_users.back()->msgToUsser(INIT_MSG);
	return (newFd);
}
