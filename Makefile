SRC_MAIN	= 	main.cpp
DIR_MAIN	=	./scripts/
OBJ_MAIN	=	${addprefix ${DIR_MAIN}, ${SRC_MAIN:.cpp=.o}}

SRC_CLASS	=	ft_irc/ft_irc.cpp \
				ft_irc/constructor.cpp \
				ft_irc/emergency.cpp \
				ft_irc/get_set.cpp \
				ft_irc/create_serv.cpp \
				ft_irc/operate_socket.cpp \
				ft_irc/commands.cpp \
				ft_irc/operations.cpp \
				error/error.cpp \
				parser/parser.cpp \
				user/user.cpp \
				user/msgOpr.cpp \
				channel/channel.cpp \
				channel/bot/bot.cpp
DIR_CLASS	=	./class_cpp/
OBJ_CLASS	=	${addprefix ${DIR_CLASS}, ${SRC_CLASS:.cpp=.o}}


ALLOBJ		=	${OBJ_MAIN} ${OBJ_CLASS}

NAME		=	ft_irc

CPP			=	c++

FLAGS		=	-Wall -Wextra -Werror -std=c++98

all:		${NAME}

.cpp.o:
			@${CPP} ${FLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${ALLOBJ}
			@${CPP} ${FLAGS} ${ALLOBJ} -o ${NAME}
			@echo DONE

clean:
			@rm -f ${ALLOBJ}
			@echo clean DONE

fclean:		clean
			@rm -f ${NAME}
			@echo fclean DONE

re:			fclean all

.PHONY:		all clean fclean re