#.PHONY: all bonus clean fclean re
NAME = webserver

SRC = main.cpp src/*.cpp parsing/parse_confile/*.cpp parsing/parse_request/main.cpp parsing/parse_request/parse_request.cpp parsing/parse_request/set_extentions.cpp response/srcs/response.cpp cgi/execute_cgi.cpp
#SRC_BONUS =	checker.c get_next_line.c get_next_line_utils.c pile.c \
			operations.c to_check.c utils.c free.c
OBJECT = $(SRC:.cpp=.o)
#OBJECT_BONUS = $(SRC_BONUS:.cpp=.o)

all : $(NAME)

$(NAME):
	@c++ -Wall -Werror -Wextra -std=c++98 $(SRC) -o $(NAME) 
	@echo "✅✅✅"

debug: 
	@c++ -g3 -fsanitize=address -std=c++98 $(SRC) -o $(NAME) 
clean:
	@rm -f $(OBJECT)
	@echo "🗑️ 🗑️ 🗑️"
fclean: clean
	@rm -f $(NAME)
	@echo "🚮🚮🚮"
re: fclean all
