/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 17:39:35 by jtranchi          #+#    #+#             */
/*   Updated: 2016/02/15 17:39:36 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21.h"

int			ft_outchar(int c)
{
	signal(SIGINT, ft_prompt);
	signal(SIGTSTP, ft_prompt);
	return (write(1, &c, 1));
}

void		ft_process_unsetenv(t_term *term, char *s1)
{
	t_env *lst;

	lst = term->lst;
	if (!s1)
	{
		ft_putendl("21sh: unsetenv: Invalid option");
		ft_putendl("unsetenv NAME");
	}
	else
	{
		if (ft_strcmp(lst->var, s1) == 0)
			term->lst = lst->next;
		while (lst->next && ft_strcmp((lst->next)->var, s1) != 0)
			lst = lst->next;
		(lst->next) ? (lst->next = lst->next->next) : 0;
	}
}

void		ft_process_setenv(t_term *term, char *s1, char *s2)
{
	t_env *lst;

	lst = term->lst;
	if (!s1 || !s2)
	{
		ft_putendl("21sh: setenv: Invalid option");
		ft_putendl("setenv NAME VALUE");
	}
	else
	{
		lst = (t_env*)malloc(sizeof(t_env));
		lst->var = s1;
		lst->val = s2;
		lst->next = NULL;
		term->lst = ft_add_lst(lst, term->lst);
	}
}

void		ft_cd(t_term *term)
{
	char *buf;

	buf = NULL;
	if (ft_strcmp(term->cmds[1], "-") == 0)
	{
		chdir(ft_get_env_by_name(term, "OLDPWD"));
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", ft_get_env_by_name(term, "PWD"));
	}
	else
	{
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	}
	if (term->cmds[1] && ft_strcmp(term->cmds[1], "~") != 0)
		chdir(term->cmds[1]);
	else
		chdir(ft_get_val(term, "HOME"));
	ft_process_unsetenv(term, "PWD");
	ft_process_setenv(term, "PWD", getwd(buf));
}

int			init_shell(int lflag)
{
	char	*name;
	struct termios	term;

	if ((name = getenv("TERM")) == NULL)
		name = "xterm-256color";
	if (tgetent(NULL, name) == ERR)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag = term.c_lflag & lflag;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, 0, &term))
		return (-1);
	return (1);
}

int			reset_shell(void)
{
	struct termios term;

	tputs(tgetstr("me", NULL), 1, ft_outchar);
	tputs(tgetstr("ve", NULL), 1, ft_outchar);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag = (ICANON | ECHO);
	if (tcsetattr(0, 0, &term) == -1)
		return (-1);
	return (0);
}

int			ft_check_builtin(t_term *term)
{
	int i;
	char *tmp;

	tmp = NULL;
	if (ft_strcmp(term->cmds[0], "cd") == 0)
		ft_cd(term);
	else if (ft_strcmp(term->cmds[0], "env") == 0)
		(!term->cmds[1]) ? ft_display_env(term) : ft_env_options(term);
	else if (ft_strcmp(term->cmds[0], "setenv") == 0)
		ft_process_setenv(term, term->cmds[1], term->cmds[2]);
	else if (ft_strcmp(term->cmds[0], "unsetenv") == 0)
		ft_process_unsetenv(term, term->cmds[1]);
	else if (ft_strcmp(term->cmds[0], "exit") == 0)
	{
		reset_shell();
		exit(0);
	}
	else
		return (0);
	i = 0;
	(!term->cmds[1]) ? (tmp = term->cmds[0]) : 0;
	while (term->cmds[++i])
	{
		if (!tmp)
			tmp = ft_strdup(term->cmds[i - 1]);
		tmp = ft_strjoin(tmp, " ");
		tmp = ft_strjoin(tmp, term->cmds[i]);
	}
	term->i = 0;
	term->u = NULL;
	return (1);
}

void 		ft_clean_line(t_term *term)
{
	int i;

	if (term->cursorpos < term->cmdlength)
		tputs(tgetstr("sc", NULL), 0, ft_outchar);
	i = term->cursorpos;
	if (i > 0)
	{
		while (--i > 0)
		{
			tputs(tgetstr("le", NULL), 0, ft_outchar);
			tputs(tgetstr("dc", NULL), 0, ft_outchar);
		}
		tputs(tgetstr("le", NULL), 0, ft_outchar);
		tputs(tgetstr("dc", NULL), 0, ft_outchar);
	}
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->cmdactual = NULL;
}

void		ft_add_history(t_term *term, char *cmd)
{
	int fd;
	t_history *tmp;
	t_history *tmp2;

	fd = open(".21sh_history", O_WRONLY|O_APPEND|O_CREAT);
	write(fd, cmd, ft_strlen(cmd));
	tmp2 = term->history;
	tmp = (t_history*)malloc(sizeof(t_history));
	tmp->var = cmd;
	tmp->next = NULL;
	write(fd, "\n", 1);
	close(fd);
	if (!term->history)
	{
		term->history = tmp;
		return ;
	}
	while (term->history->next)
		term->history = term->history->next;
	term->history->next = tmp;
	tmp->prev = term->history;
	term->history = term->history->next;
}

void	ft_history_prev(t_term *term)
{
	if (!term->history->next && term->cmdactual && !term->inhistory)
		ft_add_history(term, term->cmdactual);
	ft_clean_line(term);
	if (term->history->prev)
	{
		term->history = term->history->prev;
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr(term->history->var);
	}
}

void	ft_history_next(t_term *term)
{
	ft_clean_line(term);
	if (term->history->next)
	{
		term->history = term->history->next;
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr(term->history->var);
	}
	else
		term->inhistory = 0;
}

void	ft_process(t_term *term)
{
	char  *tmp;

	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 72)
	{
		if (term->cursorpos > 0)
		{
			while (--term->cursorpos > 0)
				tputs(tgetstr("le", NULL), 0, ft_outchar);
			tputs(tgetstr("le", NULL), 0, ft_outchar);
		}
	}
	else if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 70)
	{
		if (term->cursorpos < term->cmdlength)
		{
			tputs(tgetstr("nd", NULL), 0, ft_outchar);
			while (++term->cursorpos < term->cmdlength)
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
	}
	else if (term->buf[0] == 127 && term->cursorpos > 0)
	{
		tmp = &term->cmdactual[term->cursorpos];
		term->cursorpos--;
		term->cmdlength--;
		term->cmdactual[term->cursorpos] = '\0';
		term->cmdactual = ft_strjoin(term->cmdactual, tmp);
		tputs(tgetstr("le", NULL), 0, ft_outchar);
		tputs(tgetstr("dc", NULL), 0, ft_outchar);
		tputs(tgetstr("cd", NULL), 0, ft_outchar);
		tputs(tgetstr("sc", NULL), 0, ft_outchar);
		ft_putstr(&term->cmdactual[term->cursorpos]);
		tputs(tgetstr("rc", NULL), 0, ft_outchar);
	}
	else if (term->buf[0] == 27 && term->buf[2] == 68 && term->cursorpos > 0)
	{
		if (((term->cursorpos + 3) % term->window->width) == 0)
		{
			int i;
			i = 0;
			tputs(tgetstr("up", NULL), 0, ft_outchar);
			while (++i < term->window->width)
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
		else
			tputs(tgetstr("le", NULL), 0, ft_outchar);
		term->cursorpos--;
	}
	else if (term->buf[0] == 27 && term->buf[2] == 67 && term->cursorpos < term->cmdlength)
	{
		term->cursorpos++;
		tputs(tgetstr("nd", NULL), 0, ft_outchar);
	}
	else if (term->buf[0] == 27 && term->buf[2] == 65)
		ft_history_prev(term);
	else if (term->buf[0] == 27 && term->buf[2] == 66)
		ft_history_next(term);
	else if (term->buf[0] != 27 && term->buf[0] != 127 && ft_isprint(term->buf[0]))
	{
		if (term->cursorpos < term->cmdlength)
		{
			tmp = ft_strjoin(term->buf, (term->cmdactual + term->cursorpos));
			term->cmdactual[term->cursorpos] = '\0';
			term->cmdactual = ft_strjoin(term->cmdactual, tmp);
		}
		else if (!term->cmdactual)
			term->cmdactual = ft_strdup(term->buf);
		else
			term->cmdactual = ft_strjoin(term->cmdactual, term->buf);
		term->cursorpos += ft_strlen(term->buf);
		term->cmdlength = ft_strlen(term->cmdactual);
		ft_putstr(term->buf);
		if (((term->cursorpos + 3) % term->window->width) == 0)
			tputs(tgetstr("do", NULL), 0, ft_outchar);
		if (term->cursorpos < term->cmdlength)
		{
			tputs(tgetstr("sc", NUL
			ft_putstr(&term->cmdactual[term->cursorpos]);
			tputs(tgetstr("rc", NULL), 0, ft_outchar);
		}
	}
	// ft_putchar('\n');
	// ft_putnbr(term->cursorpos);
	ft_bzero(term->buf, ft_strlen(term->buf));
}

void		ft_get_history(t_term *term)
{
	int fd;
	t_history *tmp;
	char *line;

	fd = open(".21sh_history", O_RDONLY);
	term->history = NULL;
	term->historylen = 0;
	term->historycurrent = 0;
	while ((get_next_line(fd, &line)) > 0)
	{
		tmp = (t_history*)malloc(sizeof(t_history));
		tmp->var = ft_strdup(line);
		tmp->next = NULL;
		tmp->prev = NULL;
		term->historylen++;
		if (!term->history)
			term->history = tmp;
		else
		{
			term->history->next = tmp;
			tmp->prev = term->history;
			term->history = term->history->next;
		}
	}
}

void		ft_get_window(t_term *term)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	if (!term->window)
		term->window = (t_window*)malloc(sizeof(t_window));
	term->window->width = w.ws_col;
	term->window->heigth = w.ws_row;
}

int			main(int argc, char **argv, char **env)
{
	static t_term		*term;

	argv = NULL;
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(term);
	ft_get_history(term);
	while (42)
	{
		argc = -1;
		init_shell((~ICANON & ~ECHO));
		term->cursorpos = 0;
		term->cmdlength = 0;
		term->cmdsplit = NULL;
		term->path = NULL;
		term->cmdactual = NULL;
		ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
		ft_bzero(term->buf, ft_strlen(term->buf));
		write(1, "$> ", 3);
		ft_get_window(term);
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
			ft_process(term);
		(ft_strlen(term->cmdactual) > 0) ? ft_add_history(term, term->cmdactual) : 0;
		reset_shell();
		ft_putchar('\n');
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			term->cmds = NULL;
			term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			(!ft_check_builtin(term)) ? ft_check_in_path(term) : 0;
		}
	}
	return (0);
}
