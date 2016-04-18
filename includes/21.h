/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/12 17:02:13 by jtranchi          #+#    #+#             */
/*   Updated: 2016/04/12 17:02:15 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VINETUNSH_H
# define VINETUNSH_H
# define BUFFSIZE 3
# include "../libft/includes/libft.h"
# include <signal.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

typedef	struct			s_term
{
	char				*path;
	pid_t				father;
	struct s_env		*lst;
	int					i;
	char				*u;
	char				**env;
	int					cursorpos;
	int					cmdlength;
	int 				historylen;
	int 				historycurrent;
	char				*buf;
	char 				*cmdactual;
	char				**cmds;
	char				**cmdsplit;
}						t_term;

typedef	struct			s_u
{
	char				*var;
	struct s_env		*next;
}						t_u;

typedef	struct			s_env
{
	char				*var;
	char				*val;
	struct s_env		*next;
}						t_env;

int						ft_check_in_path(t_term *term);
int						ft_get_val_exists(t_term **term, char *str);
int						ft_check_in_env(t_term *term, char *cmd);
void					ft_display_i(t_term *term);
void					ft_newenv_display(t_term **term);
void					ft_display_env(t_term *term);
void					ft_check_env(t_term **term);
void					ft_check_cmds(t_term *term);
void					ft_env_i(t_term *term);
void					ft_permission_denied(char *str);
void					ft_env_options(t_term **term);
void					ft_display_env(t_term *term);
void					ft_prompt(int nb);
void					ft_refresh_env(t_term **term);
void					ft_create_process(t_term *term);
void					ft_check_env(t_term **term);
void					ft_process_unsetenv(t_term **term, char *s1);
void					ft_process_setenv(t_term **term, char *s1, char *s2);
char					*ft_get_env_by_name(t_term *term, char *name);
char					*ft_get_val(t_term **term, char *str);
t_env					*ft_parse_env(char **env);
t_env					*ft_add_lst(t_env *tmp, t_env *lst);
t_env					*ft_create_lst(char *str);
t_term					*ft_set_term(char **env, t_env *lst);

#endif
