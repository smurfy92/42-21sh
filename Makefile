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
		srcs/termcaps4.c \
		srcs/termcaps5.c \
		srcs/termcaps6.c \
		srcs/builtin.c \
		srcs/cd.c \
		srcs/init.c \
		srcs/parse.c \
		srcs/parse2.c \
		srcs/parse3.c \
		srcs/separators.c \
		srcs/separators2.c \
		srcs/separators3.c \
		srcs/redirections.c \
		srcs/last.c \
		srcs/last2.c \
		srcs/last3.c \
		srcs/process.c \
		srcs/free.c

#-fsanitize=address
OBJ = $(SRC:.c=.o)
FLAG = -Wall -Werror -Wextra -g
CG = \033[92m
CY =  \033[93m
CE = \033[0m
CB = \033[34m

all: start $(NAME)

$(NAME): $(OBJ)
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Creating Library$(CE)\033[1A";
	-@make -C libft nohd
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Compiling 21sh ...$(CE)";
	@gcc -o $(NAME) $(FLAG) $(SRC) $(LIB) -lncurses

%.o: %.c
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Compiling $<$(CE) \033[1A";
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
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Cleaning Library ...$(CE)\033[1A";
	-@make -C libft nohdclean
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Cleaning 21sh objects$(CE)\033[1A";
	@/bin/rm -rf $(OBJ)

fclean: start clean
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)FCleaning Library ...$(CE)\033[1A";
	-@make -C libft nohdfclean
	@echo "\033[K$(CY)[21SH] :$(CE) $(CG)Cleaning 21sh ...$(CE)\033[1A";
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
