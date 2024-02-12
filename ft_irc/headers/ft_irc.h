#ifndef FT_IRC_H
# define FT_IRC_H

# include "iostream"
# include "netinet/in.h"
# include "sys/socket.h"
# include "cstring"
# include "fcntl.h"
# include "unistd.h"
# include "csignal"
# include "vector"
# include "poll.h"
# include "netdb.h"
# include "arpa/inet.h"

# define INIT_MSG "CAP * ACK multi-prefix\r\n"

# define MAX_CLIENT 4096
# define MaxServ 5

# define SIGTERM_MESS "Signal - 15(SIGTERM) recived!"
# define SIGINT_MESS "Signal - 15(SIGTERM) recived!"

#endif