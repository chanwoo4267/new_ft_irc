NAME = ircserv

CPPFLAGS = # -Wall -Wextra -Werror
CPP = c++

SRCS =	srcs/main.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp \
		srcs/ClientManager.cpp \
		srcs/Channel.cpp \
		srcs/ChannelManager.cpp \
		srcs/Command.cpp \
		srcs/Utility.cpp

SRCOBJ = ${SRCS:.cpp=.o}

Include = includes/Server.hpp \
		  includes/Client.hpp \
		  includes/ClientManager.hpp \
		  includes/Channel.hpp \
		  includes/ChannelManager.hpp \
		  includes/Command.hpp \
		  includes/Utility.hpp

%.o:%.cpp ${Include}
	${CPP} ${CPPFLAGS} -c $< -o $@

$(NAME): ${SRCOBJ} 
	@$(CPP) ${CPPFLAGS} $(SRCOBJ) -o $(NAME)
	@echo "|+| make the program [${GREEN}DONE${RESET}]"
	
all: ${NAME}

clean:
	@rm -f ${SRCOBJ}

fclean: clean
	@rm -f ${NAME}
	@echo "|-| remove the programs  [${RED}DONE${RESET}]"

re: fclean all

# colors used
GREEN = \033[0;92m
RED = \033[0;91m
RESET = \033[0m