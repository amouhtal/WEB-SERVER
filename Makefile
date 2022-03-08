NAME = containers
CC = clang++
FLAGS = -std=c++98 -Wall -Werror -Wextra
SRCS = main.cpp srcs/server.cpp srcs/client.cpp srcs/cgi.cpp srcs/DataServer.cpp srcs/location.cpp srcs/parssingfile.cpp srcs/request.cpp srcs/response.cpp srcs/socket.cpp
OBJS = $(SRCS:.cpp=.o)

%.o:%.cpp *.hpp
	@$(CC) $(FLAGS) -c $< -o $@
all : $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
clean :
	@rm -rf $(OBJS)
fclean : clean
	@rm -rf $(NAME)
re : fclean all