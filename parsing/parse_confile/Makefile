# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/30 18:02:55 by yhebbat           #+#    #+#              #
#    Updated: 2022/04/30 18:02:56 by yhebbat          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#.PHONY: all bonus clean fclean re
NAME = webserver

SRC = cgi.cpp  location.cpp  main.cpp  parse_confile.cpp  server.cpp
#SRC_BONUS =	checker.c get_next_line.c get_next_line_utils.c pile.c \
			operations.c to_check.c utils.c free.c
OBJECT = $(SRC:.cpp=.o)
#OBJECT_BONUS = $(SRC_BONUS:.cpp=.o)

all : $(NAME)

$(NAME):
	@c++  -Wall -Wextra -Werror $(SRC) -o $(NAME)
	@echo "✅✅✅"

#bonus:
#	@gcc -Wall -Wextra -Werror $(SRC_BONUS) -o $(NAME_BONUS)
#	@echo "🎁🎁🎁"

clean:
	@rm -f $(OBJECT)
	@echo "🗑️ 🗑️ 🗑️"
fclean: clean
	@rm -f $(NAME)
	@echo "🚮🚮🚮"
re: fclean all
	@echo "🔁🔁🔁"