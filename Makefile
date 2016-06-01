# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/28 21:30:36 by jtranchi          #+#    #+#              #
#    Updated: 2016/02/28 21:30:37 by jtranchi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
LIB = libft/libft.a
SRC =	srcs/main.c \
		srcs/set_env.c \
		srcs/aymeri.c \
		srcs/errors.c \
		srcs/bonus.c \
		srcs/rest.c \
		srcs/history.c \
		srcs/termcaps.c \
		srcs/termcaps2.c \
		srcs/termcaps3.c \
		srcs/builtin.c \
		srcs/cd.c
OBJ = $(SRC:.c=.o)
FLAG = -Wall -Werror -Wextra
CG = \033[92m
CY =  \033[93m
CE = \033[0m
CB = \033[34m

all: start $(NAME)

$(NAME): $(OBJ)
	@echo "$(CY)[MINISH] :$(CE) $(CG)Creating Library$(CE)";
	-@make -C libft nohd
	@echo "$(CY)[MINISH] :$(CE) $(CG)Compiling minishell ...$(CE)";
	@gcc -g -o $(NAME) $(FLAG) $(SRC) $(LIB) -lncurses

%.o: %.c
	@echo "$(CY)[MINISH] :$(CE) $(CG)Compiling $<$(CE)";
	@gcc $(FLAG) -c $< -o $@

start:
	@echo "\n\n"
	@echo "					$(CG)                          00000000000000000000$(CE)";
	@echo "					$(CG)                       00000000000000000000000000$(CE)";
	@echo "					$(CG)                    000000000000000000000000000000000         00   0000$(CE)";
	@echo "					$(CG)    000000        00000000000000000000000000000000000000       000000000$(CE)";
	@echo "					$(CG) 00 000000      0000000000    0000000000000    0000000000       0000000$(CE)";
	@echo "					$(CG) 000000000     0000000000      00000000000      00000000000    0000000$(CE)";
	@echo "					$(CG)   0000000    00000000000      00000000000      0000000000000000000000$(CE)";
	@echo "					$(CG)   00000000000000000000000    0000000000000    00000000000000  00000$(CE)";
	@echo "					$(CG)    000000000000000000000000000000000000000000000000000000000     000$(CE)";
	@echo "					$(CG)     000   000000000000000000000000000000000000000000000000000     0000$(CE)";
	@echo "					$(CG)    0000   000000000000000000000000000000000000000000000000000       000$(CE)";
	@echo "					$(CG)    000    0000000000000000000000000000000000000000000000 0000000000000000$(CE)";
	@echo "					$(CG)   0000000000000  0000000000000000000000000000000000000   00000000000000000$(CE)";
	@echo "					$(CG)   0000000000000   0000000000000000000000000000000000     00000000000$(CE)";
	@echo "					$(CG)  0000       0000    000000000000000000000000000000      000$(CE)";
	@echo "					$(CG)             00000     0000000000000000000000000         00$(CE)";
	@echo "					$(CG)               0000          000000000000000           000$(CE)";
	@echo "					$(CG)                00000                                0000$(CE)";
	@echo "					$(CG)                 000000      00000000000000        0000$(CE)";
	@echo "					$(CG)                   00000000     0000000000000   000000$(CE)";
	@echo "					$(CG)                      00000000000  0000000000000000$(CE)";
	@echo "					$(CG)                         000000000000000000000$(CE)";
	@echo "					$(CG)                                 00000000000000$(CE)";
	@echo "					$(CG)                                     00000000000$(CE)";
	@echo "					$(CG)                                      0000000000$(CE)";
	@echo "					$(CG)                                       0000000$(CE)";
	@echo "\n\n"
	@echo "								$(CG) ___   __         _          $(CE)";
	@echo "								$(CG)|__ \ /_ |       | |         $(CE)";
	@echo "								$(CG)   ) | | |  ___  | |__       $(CE)";
	@echo "								$(CG)  / /  | | / __| | '_ \      $(CE)";
	@echo "								$(CG) / /_  | | \__ \ | | | |     $(CE)";
	@echo "								$(CG)|____| |_| |___/ |_| |_|     $(CE)";

clean: start
	@echo "$(CY)[MINISH] :$(CE) $(CG)Cleaning Library ...$(CE)";
	-@make -C libft nohdclean
	@echo "$(CY)[MINISH] :$(CE) $(CG)Cleaning minishell objects$(CE)";
	@/bin/rm -rf $(OBJ)

fclean: start clean
	@echo "$(CY)[MINISH] :$(CE) $(CG)FCleaning Library ...$(CE)";
	-@make -C libft nohdfclean
	@echo "$(CY)[MINISH] :$(CE) $(CG)Cleaning minishell ...$(CE)";
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
