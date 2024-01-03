# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/03 21:11:34 by fvoicu            #+#    #+#              #
#    Updated: 2024/01/03 21:28:00 by fvoicu           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	philo
CC		:=	gcc
FLAGS	:=	-Wall -Wextra -Werror -lpthread
HEAD	:= -I ./include 

SRC		:=	$(wilcard src/*.c) \
				$(wilcard)	

OBJ		:= $(SRC:.c=.o)


all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) $(HEAD) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(HEAD) -o $(NAME)


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re

