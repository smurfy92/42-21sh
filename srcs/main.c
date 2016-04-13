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
	(*term)->i = 0;
	(*term)->u = NULL;
	return (1);
}

void	ft_process(t_term **term)
{
	if ((*term)->buf[0] == 27 && (*term)->buf[2] == 68)
		tputs(tgetstr("le", NULL), 1, ft_outchar);
	if ((*term)->buf[0] == 27 && (*term)->buf[2] == 67)
		tputs(tgetstr("ri", NULL), 0, ft_outchar);
	if ((*term)->buf[0] == 27 && (*term)->buf[2] == 65)
		ft_putendl("up arrow");
	if ((*term)->buf[0] == 27 && (*term)->buf[2] == 66)
		ft_putendl("down arrow");
	if (!(*term)->cmdactual && (*term)->buf[0] != 10)
		(*term)->cmdactual = ft_strdup((*term)->buf);
	else if ((*term)->buf[0] != 10)
		(*term)->cmdactual = ft_strjoin((*term)->cmdactual, (*term)->buf);
	if ((*term)->buf[0] == 10)
	return ;
}

int			main(int argc, char **argv, char **env)
{
	char		*buf;
	t_term		*term;

	argc = 0;
	argv = NULL;
	buf = NULL;
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(&term);
	signal(SIGINT, ft_prompt);
	signal(SIGTSTP, ft_prompt);
	init_shell((~ICANON));
	while (42)
	{
		argc = -1;
		write(1, "$> ", 3);
		while (term->buf[0] != 10 && (read(0, term->buf, BUFFSIZE)))
			ft_process(&term);
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			(!ft_check_builtin(&term)) ? ft_check_in_path(term) : 0;
		}
		ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
		ft_bzero(term->buf, ft_strlen(term->buf));
	}
	return (0);
}
