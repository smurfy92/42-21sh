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
	return (write(1, &c, 1));
}

void		ft_process_unsetenv(t_term **term, char *s1)
{
	t_env *lst;

	lst = (*term)->lst;
	if (!s1)
	{
		ft_putendl("minish: unsetenv: Invalid option");
		ft_putendl("unsetenv NAME");
	}
	else
	{
		if (ft_strcmp(lst->var, s1) == 0)
			(*term)->lst = lst->next;
		while (lst->next && ft_strcmp((lst->next)->var, s1) != 0)
			lst = lst->next;
		(lst->next) ? (lst->next = lst->next->next) : 0;
	}
}

void		ft_process_setenv(t_term **term, char *s1, char *s2)
{
	t_env *lst;

	lst = (*term)->lst;
	if (!s1 || !s2)
	{
		ft_putendl("minish: setenv: Invalid option");
		ft_putendl("setenv NAME VALUE");
	}
	else
	{
		lst = (t_env*)malloc(sizeof(t_env));
		lst->var = s1;
		lst->val = s2;
		lst->next = NULL;
		(*term)->lst = ft_add_lst(lst, (*term)->lst);
	}
}

void		ft_cd(t_term **term)
{
	char *buf;

	buf = NULL;
	if (ft_strcmp((*term)->cmds[1], "-") == 0)
	{
		chdir(ft_get_env_by_name(*term, "OLDPWD"));
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", ft_get_env_by_name(*term, "PWD"));
	}
	else
	{
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	}
	if ((*term)->cmds[1] && ft_strcmp((*term)->cmds[1], "~") != 0)
		chdir((*term)->cmds[1]);
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
	exit(0);
	return (0);
}

int			ft_check_builtin(t_term **term)
{
	int i;
	char *tmp;

	tmp = NULL;
	if (ft_strcmp((*term)->cmds[0], "cd") == 0)
		ft_cd(term);
	else if (ft_strcmp((*term)->cmds[0], "env") == 0)
		(!(*term)->cmds[1]) ? ft_display_env(*term) : ft_env_options(term);
	else if (ft_strcmp((*term)->cmds[0], "setenv") == 0)
		ft_process_setenv(term, (*term)->cmds[1], (*term)->cmds[2]);
	else if (ft_strcmp((*term)->cmds[0], "unsetenv") == 0)
		ft_process_unsetenv(term, (*term)->cmds[1]);
	else if (ft_strcmp((*term)->cmds[0], "exit") == 0)
		reset_shell();
	else
		return (0);
	i = 0;
	(!(*term)->cmds[1]) ? (tmp = (*term)->cmds[0]) : 0;
	while ((*term)->cmds[++i])
	{
		if (!tmp)
			tmp = ft_strdup((*term)->cmds[i - 1]);
		tmp = ft_strjoin(tmp, " ");
		tmp = ft_strjoin(tmp, (*term)->cmds[i]);
	}

	i = open(".21sh_history", O_WRONLY|O_APPEND|O_CREAT);
	tmp = ft_strjoin(";", tmp);
	tmp = ft_strjoin(ft_itoa(++(*term)->historylen), tmp);
	write(i, tmp, ft_strlen(tmp));
	write(i, "\n", 1);
	close(i);
	(*term)->i = 0;
	(*term)->u = NULL;
	return (1);
}

void	ft_process(t_term **term)
{
	// printf("\n0 : %d\n",(*term)->buf[0]);
	// printf("1 : %d\n",(*term)->buf[1]);
	// printf("2 : %d\n",(*term)->buf[2]);
	char  *tmp;
	if (!(*term)->buf[1] && (*term)->buf[0] != 10 && (*term)->buf[0] != 127)
	{
		if ((*term)->cursorpos < (*term)->cmdlength)
		{
			tputs(tgetstr("im", NULL), 1, ft_outchar);
			tmp = ft_strjoin(&(*term)->buf[0], ((*term)->cmdactual + (*term)->cursorpos));
			(*term)->cmdactual[(*term)->cursorpos ] = '\0';
			(*term)->cmdactual = ft_strjoin((*term)->cmdactual, tmp);
		}
		else if (!(*term)->cmdactual)
			(*term)->cmdactual = ft_strdup((*term)->buf);
		else
			(*term)->cmdactual = ft_strjoin((*term)->cmdactual, (*term)->buf);
		ft_putchar((*term)->buf[0]);
		tputs(tgetstr("ei", NULL), 1, ft_outchar);
		if ((*term)->cursorpos == (*term)->cmdlength)
			(*term)->cmdlength++;
		(*term)->cursorpos++;
	}
	else if ((*term)->buf[0] != 10)
	{
		if ((*term)->buf[0] == 127 && (*term)->cursorpos > 0)
		{
			tmp = &(*term)->cmdactual[(*term)->cursorpos];
			(*term)->cursorpos--;
			(*term)->cmdlength--;
			(*term)->cmdactual[(*term)->cursorpos] = '\0';
			(*term)->cmdactual = ft_strjoin((*term)->cmdactual, tmp);
			tputs(tgetstr("le", NULL), 0, ft_outchar);
			tputs(tgetstr("dc", NULL), 0, ft_outchar);
		}
		if ((*term)->buf[0] == 27 && (*term)->buf[2] == 68 && (*term)->cursorpos > 0)
		{
			(*term)->cursorpos--;
			tputs(tgetstr("le", NULL), 0, ft_outchar);
		}
		if ((*term)->buf[0] == 27 && (*term)->buf[2] == 67 && (*term)->cursorpos < (*term)->cmdlength)
		{
			(*term)->cursorpos++;
			tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
		if ((*term)->buf[0] == 27 && (*term)->buf[2] == 65 && (*term)->cursorpos < (*term)->cmdlength)
		{
			//ft_history_prev();
		}
		if ((*term)->buf[0] == 27 && (*term)->buf[2] == 66 && (*term)->cursorpos < (*term)->cmdlength)
		{
			//ft_history_next();
		}
	}
	ft_bzero((*term)->buf, ft_strlen((*term)->buf));
	tputs(tgetstr("ve", NULL), 1, ft_outchar);
}

int			main(int argc, char **argv, char **env)
{
	char		*buf;
	static t_term		*term;

	argc = 0;
	argv = NULL;
	buf = NULL;
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(&term);
	signal(SIGINT, ft_prompt);
	signal(SIGTSTP, ft_prompt);
	init_shell((~ICANON & ~ECHO));
	while (42)
	{
		argc = -1;
		write(1, "$> ", 3);
		term->cursorpos = 0;
		term->cmdlength = 0;
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
			ft_process(&term);
		ft_putchar('\n');
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			(!ft_check_builtin(&term)) ? ft_check_in_path(term) : 0;
		}
		ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
		ft_bzero(term->buf, ft_strlen(term->buf));
		term->cursorpos = 0;
	}
	return (0);
}
