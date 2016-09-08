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

void		ft_cd_error(char *cmd)
{
	ft_putendl(ft_strjoin("cd: no such file or directory: ", cmd));
}

void		ft_cd_else(t_term *term)
{
	struct stat			bufstat;

	(access(term->cmds[1], R_OK)) ? ft_cd_error(term->cmds[1]) : 0;
	if (lstat(term->cmds[1], &bufstat) == -1)
	{
		return (ft_putendl(ft_strjoin(
		"cd: no such file or directory: ", term->cmds[1])));
	}
}

void		ft_cd_home(t_term *term)
{
	char				*buf;

	buf = NULL;
	if (!ft_get_val_exists(term, "HOME"))
		return (ft_putendl("NO HOME"));
	ft_process_unsetenv(term, "OLDPWD");
	ft_process_setenv(term, "OLDPWD", getwd(buf));
	chdir(ft_get_val(term, "HOME"));
}

void		ft_cd(t_term *term)
{
	char	*buf;

	buf = NULL;
	if (!ft_get_val_exists(term, "OLDPWD"))
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	if (!ft_get_val_exists(term, "PWD"))
		ft_process_setenv(term, "PWD", getwd(buf));
	if (term->cmds[1] && ft_strcmp(term->cmds[1], "-") == 0)
		chdir(ft_get_env_by_name(term, "OLDPWD"));
	else if (!term->cmds[1] || (term->cmds[1] &&
	ft_strcmp(term->cmds[1], "~") == 0))
		return (ft_cd_home(term));
	else
		ft_cd_else(term);
	ft_process_unsetenv(term, "OLDPWD");
	ft_process_setenv(term, "OLDPWD", getwd(buf));
	chdir(term->cmds[1]);
	ft_process_unsetenv(term, "PWD");
	ft_process_setenv(term, "PWD", getwd(buf));
}

void			ft_create_builtin(t_term *term)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strcmp(term->cmds[0], "cd") == 0)
		ft_cd(term);
	else if (ft_strcmp(term->cmds[0], "env") == 0)
	{
		(!term->cmds[1]) ? ft_display_env(term) : ft_env_options(term);
	}
	else if (ft_strcmp(term->cmds[0], "setenv") == 0)
		ft_process_setenv(term, term->cmds[1], term->cmds[2]);
	else if (ft_strcmp(term->cmds[0], "unsetenv") == 0)
		ft_process_unsetenv(term, term->cmds[1]);
	else if (ft_strcmp(term->cmds[0], "exit") == 0)
	{
		reset_shell();
		ft_putendl("Exiting shell");
		exit(0);
	}
	term->i = 0;
	term->u = NULL;
}

int			ft_check_builtin(t_term *term)
{
	if (ft_strcmp(term->parselst->cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(term->parselst->cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(term->parselst->cmd, "setenv") == 0)
		return (1);
	else if (ft_strcmp(term->parselst->cmd, "unsetenv") == 0)
		return (1);
	else if (ft_strcmp(term->parselst->cmd, "exit") == 0)
		return (1);
	else
		return (0);
}
