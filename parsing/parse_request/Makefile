#.PHONY: all bonus clean fclean re
NAME = webserver

SRC = parse_request.cpp main.cpp set_extentions.cpp
#SRC_BONUS =	checker.c get_next_line.c get_next_line_utils.c pile.c \
			operations.c to_check.c utils.c free.c
OBJECT = $(SRC:.cpp=.o)
#OBJECT_BONUS = $(SRC_BONUS:.cpp=.o)

all : $(NAME)

$(NAME):
	@c++ -std=c++98 -Wall -Wextra -Werror $(SRC) -o $(NAME)
	@echo "✅✅✅"


clean:
	@rm -f $(OBJECT)
	@echo "🗑️ 🗑️ 🗑️"
fclean: clean
	@rm -f $(NAME)
	@echo "🚮🚮🚮"
re: fclean all
	@echo "🔁🔁🔁"