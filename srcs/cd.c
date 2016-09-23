/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/01 11:54:08 by jtranchi          #+#    #+#             */
/*   Updated: 2016/06/01 11:54:09 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_cd_home(t_term *term)
{
	if (!ft_get_val_exists(term, "HOME"))
		return (ft_putendl("NO HOME"));
	ft_process_unsetenv(term, "OLDPWD");
	ft_process_setenv(term, "OLDPWD", ft_get_val(term, "PWD"));
	chdir(ft_get_val(term, "HOME"));
	ft_process_unsetenv(term, "PWD");
	ft_process_setenv(term, "PWD", ft_get_val(term, "HOME"));
}

void		ft_cd_suite(t_term *term)
{
	char *buf;

	buf = NULL;
	ft_process_unsetenv(term, "OLDPWD");
	ft_process_setenv(term, "OLDPWD", ft_get_val(term, "PWD"));
	chdir(term->cmds[1]);
	ft_process_unsetenv(term, "PWD");
	ft_process_setenv(term, "PWD", getcwd(buf, 0));
}

void		ft_cd2(t_term *term)
{
	if (!ft_get_val_exists(term, "OLDPWD"))
		ft_process_setenv(term, "OLDPWD", ft_get_val(term, "PATH"));
	if (!ft_get_val_exists(term, "PWD"))
		ft_process_setenv(term, "PWD", ft_get_val(term, "PATH"));
	if (term->cmds[1] && ft_strcmp(term->cmds[1], "-") == 0)
	{
		term->cmds[1] = ft_strdup(ft_get_env_by_name(term, "OLDPWD"));
		if (!access(term->cmds[1], R_OK))
			ft_putendl_fd(term->cmds[1], 2);
	}
}

void		ft_cd(t_term *term)
{
	struct stat			bufstat;

	ft_cd2(term);
	if (term->cmds[1] && term->cmds[1][0] == '~' && term->cmds[1][1])
	{
		if (!ft_get_val_exists(term, "HOME"))
			return (ft_putendl("NO HOME"));
		term->cmds[1] = ft_strjoin(ft_get_val(term, "HOME"), &term->cmds[1][1]);
	}
	if (!term->cmds[1] || (term->cmds[1] && term->cmds[1][0] == '~'))
		return (ft_cd_home(term));
	else
	{
		if (access(term->cmds[1], R_OK))
			return (ft_cd_error(term->cmds[1]));
		if (lstat(term->cmds[1], &bufstat) == -1)
		{
			return (ft_putendl(ft_strjoin(
			"cd: no such file or directory: ", term->cmds[1])));
		}
	}
	ft_cd_suite(term);
}

void		ft_free_struct(t_term *term)
{
	t_parse 	*parse;
	t_env		*lst;
	t_history	*hist;
	int 		i;

	while (term->parselst)
	{
		ft_strdel(&(term->parselst->cmd));
		ft_strdel(&(term->parselst->dbred));
		ft_strdel(&(term->parselst->sgred));
		ft_strdel(&(term->parselst->heredoc));
		ft_strdel(&(term->parselst->file));
		parse = term->parselst->next;
		free(term->parselst);
		term->parselst = parse;
	}
	while (term->lst)
	{
		ft_strdel(&(term->lst->var));
		ft_strdel(&(term->lst->val));
		lst = term->lst->next;
		free(term->lst);
		term->lst = lst;
	}
	while (term->history->prev)
		term->history = term->history->prev;
	while (term->history)
	{
		if (term->history->var)
			ft_strdel(&(term->history->var));
		term->history->prev = NULL;
		hist = term->history->next;
		free(term->history);
		term->history = hist;
	}
	i = -1;
	while (term->cmdsplit[++i])
		ft_strdel(&(term->cmdsplit[i]));
	i = -1;
	while (term->cmds[++i])
		ft_strdel(&(term->cmds[i]));
	free(term->cmds);
	free(term->cmdsplit);
	free(term->history);
	free(term->lst);
	free(term->parselst);
	free(term->window);
	ft_strdel(&(term->path));
	ft_strdel(&(term->buf));
	ft_strdel(&(term->cmdtmp));
	ft_strdel(&(term->copy));
	free(term);
}

void		ft_create_builtin(t_term *term)
{
	char	*tmp;
	int		i;

	i = 0;
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
		ft_putendl("Exiting shell");
		if (term->cmds[1])
			i = ft_atoi(term->cmds[1]);
		ft_free_struct(term);
		exit(i);
	}
	term->i = 0;
	term->u = NULL;
}
