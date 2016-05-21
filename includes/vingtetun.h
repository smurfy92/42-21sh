/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vingtetun.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/21 15:01:48 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/21 15:02:08 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VINGTETUN_H
# define VINGTETUN_H
# define BUFFSIZE 4
# include "../libft/includes/libft.h"
# include <signal.h>
# include <termios.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <term.h>

typedef	struct			s_term
{
	char				*path;
	pid_t				father;
	struct s_env		*lst;
	int					i;
	char				*u;
	struct s_history	*history;
	struct s_window		*window;
	char				**env;
	int					cursorpos;
	int					cmdlength;
	int					historylen;
	int					inhistory;
	int					historycurrent;
	int					parenthese;
	char				*buf;
	char				*cmdactual;
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

typedef	struct			s_history
{
	char				*var;
	struct s_history	*next;
	struct s_history	*prev;
}						t_history;

typedef	struct			s_window
{
	int					width;
	int					heigth;
}						t_window;

int						ft_outchar(int c);
int						reset_shell(void);
int						ft_check_in_path(t_term *term);
int						ft_get_val_exists(t_term *term, char *str);
int						ft_check_in_env(t_term *term, char *cmd);
int						init_shell(int lflag);
int						ft_check_builtin(t_term *term);
void					ft_add_history(t_term *term, char *cmd);
void					ft_get_history(t_term *term);
void					ft_get_cmd(t_term *term);
void					ft_go_home(t_term *term);
void					ft_go_end(t_term *term);
void					ft_backspace(t_term *term);
void					ft_left_arrow(t_term *term);
void					ft_right_arrow(t_term *term);
void					ft_process(t_term *term);
void					ft_clean_line(t_term *term);
void					ft_add_history(t_term *term, char *cmd);
void					ft_history_prev(t_term *term);
void					ft_history_next(t_term *term);
void					ft_display_i(t_term *term);
void					ft_newenv_display(t_term *term);
void					ft_display_env(t_term *term);
void					ft_check_env(t_term *term);
void					ft_check_cmds(t_term *term);
void					ft_env_i(t_term *term);
void					ft_permission_denied(char *str);
void					ft_env_options(t_term *term);
void					ft_display_env(t_term *term);
void					ft_prompt(int nb);
void					ft_refresh_env(t_term *term);
void					ft_create_process(t_term *term);
void					ft_check_env(t_term *term);
void					ft_process_unsetenv(t_term *term, char *s1);
void					ft_process_setenv(t_term *term, char *s1, char *s2);
char					*ft_get_env_by_name(t_term *term, char *name);
char					*ft_get_val(t_term *term, char *str);
t_env					*ft_parse_env(char **env);
t_env					*ft_add_lst(t_env *tmp, t_env *lst);
t_env					*ft_create_lst(char *str);
t_term					*ft_set_term(char **env, t_env *lst);

#endif
