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
# define BUFFSIZE 6
# define KEY(a, b, c, d) ((d << 24) + (c << 16) + (b << 8) + a)
# include "../libft/includes/libft.h"
# include <signal.h>
# include <termios.h>
# include <sys/stat.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <term.h>

typedef	struct			s_term
{
	struct s_env		*lst;
	struct s_history	*history;
	struct s_window		*window;
	struct s_parse		*parselst;
	pid_t				father;
	char				*path;
	char				*u;
	char				**env;
	char				*buf;
	char				*cmdactual;
	char				*copy;
	char				**cmds;
	char				**cmdsplit;
	int					inheredoc;
	int					i;
	int					fail;
	int					exec;
	int					cursorpos;
	int					cmdlength;
	int					historylen;
	int					inhistory;
	int					historycurrent;
	int					parenthese;
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

typedef	struct			s_parse
{
	char				*cmd;
	int					fd;
	int					close1;
	int					close2;
	char				*dbred;
	char				*sgred;
	char				*heredoc;
	char				*file;
	int					operato;
	struct s_parse		*next;
}						t_parse;

//debug

void					ft_out(char *str);

//official

void					ft_create_redirections(t_parse *parse);
int						ft_is_page_up(t_term *term);
int						ft_is_page_down(t_term *term);
int						ft_is_down_arrow(t_term *term);
int						ft_is_up_arrow(t_term *term);
int						ft_is_left_arrow(t_term *term);
int						ft_is_right_arrow(t_term *term);
int						ft_is_end(t_term *term);
int						ft_is_home(t_term *term);
int						ft_is_shift_end(t_term *term);
int						ft_is_shift_home(t_term *term);
int						ft_is_shift_left(t_term *term);
int						ft_is_shift_right(t_term *term);
int						ft_is_shift_up(t_term *term);
int						ft_is_shift_down(t_term *term);
int						ft_outchar(int c);
int						reset_shell(void);
int						ft_check_in_path(t_term *term);
int						ft_get_val_exists(t_term *term, char *str);
int						ft_check_in_env(t_term *term, char *cmd);
int						init_shell(int lflag);
int						ft_check_builtin(t_term *term);
int						ft_is_space(char c);
void					ft_display_parse(t_parse *parse);
void					ft_create_parse(t_term *term, char *cmd);
void					ft_write_in_tmp(t_term *term, char *cmd);
void					ft_parse(t_term *term, char *cmd);
void					ft_reset_term(t_term *term);
void					ft_get_window_sig();
void					ft_shift_left(t_term *term);
void					ft_prev_word(t_term *term);
void					ft_next_word(t_term *term);
void					ft_print_buf(t_term *term, char *str);
void					ft_replace_cursor(t_term *term);
void					ft_right_cursor(t_term *term);
void					ft_add_history(t_term *term, char *cmd);
void					ft_get_history(t_term *term);
void					ft_get_cmd(t_term *term, char *str);
void					ft_go_home(t_term *term);
void					ft_go_end(t_term *term);
void					ft_backspace(t_term *term);
void					ft_left_cursor(t_term *term);
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
void					ft_prompt(t_term *term, int nb);
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
t_term					*ft_get_term(void);
t_term					*ft_set_term(t_term *term, char **env, t_env *lst);
t_parse					*ft_parse_redirections(t_term *term, t_parse *parse);

#endif
