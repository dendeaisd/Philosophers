# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/03 21:11:34 by fvoicu            #+#    #+#              #
#    Updated: 2024/01/07 20:09:11 by fvoicu           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	philo
CC			:=	gcc
FLAGS		:=	-g -Wall -Wextra -Werror -pthread
HEAD		:= -I ./include 

SRC			:=	$(wildcard src/*.c) 

OBJ			:=	$(SRC:.c=.o)

UP			:= \033[A
CUT			:= \033[K
GREY		:= \033[38;5;244m
PINK		:= \033[38;5;218m
RESET		:= \033[0m

all: $(NAME)

%.o:%.c
			@printf "$(UP)$(BEGIN)$(CUT)$(PINK)Compiling: $<...\n$(RESET)"
			@$(CC) $(FLAGS) $(HEAD) -c $< -o $@
			@printf "$(UP)$(BEGIN)$(CUT)$(GREY)Philosophers compiled succesfully!$(REST)🗿\n"

$(NAME):$(OBJ)
			@$(CC) $(FLAGS) $(OBJ) $(HEAD) -o $(NAME)
	
clean:
			@rm -f $(OBJ)

fclean: 	clean
			@rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re

