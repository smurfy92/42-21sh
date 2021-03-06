/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/08 12:30:34 by jtranchi          #+#    #+#             */
/*   Updated: 2016/04/08 12:30:36 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_check_cmds(t_term *term)
{
	int i;
	int y;

	i = -1;
	y = -1;
	while (term->cmds[++i])
	{
		if (ft_strcmp(term->cmds[i], "-u") == 0)
			ft_check_u(term, i++);
		else
		{
			if (ft_strchr(term->cmds[i], '='))
			{
				while (term->env[y])
					y++;
				term->env[y] = ft_strdup(term->cmds[i]);
			}
		}
	}
}

int			ft_check_in_env2(t_term *term, char *cmd)
{
	int father;

	if (cmd[0] == '.' && cmd[1] == '/')
		(access(&cmd[2], X_OK) == 0) ? term->path =\
		ft_strdup(&cmd[2]) : ft_permission_denied(&cmd[0]);
	if (term->path)
	{
		ft_refresh_env(term);
		father = fork();
		if (father == 0)
		{
			if (term->i)
				execve(term->path, term->cmds, NULL);
			else
				execve(term->path, term->cmds, term->env);
		}
		wait(0);
		return (1);
	}
	else
		return (0);
}

int			ft_check_in_env(t_term *term, char *cmd)
{
	char		**tabl;
	int			i;
	t_env		*lst;

	lst = term->lst;
	if (!ft_get_val_exists(term, "PATH"))
		return (0);
	i = -1;
	while (lst && ft_strcmp(lst->var, "PATH") != 0)
		lst = lst->next;
	tabl = ft_strsplit(lst->val, ':');
	while (tabl && tabl[++i])
	{
		tabl[i] = ft_strjoin(ft_strjoin(tabl[i], "/"), cmd);
		if (access(tabl[i], X_OK) == 0)
		{
			term->path = ft_strdup(tabl[i]);
			break ;
		}
	}
	return (ft_check_in_env2(term, cmd));
}

void		ft_newenv_display(t_term *term)
{
	int			i;
	t_env		*lst;

	lst = term->lst;
	while (lst)
	{
		i = -1;
		while (term->cmds[++i])
		{
			if (ft_strcmp(term->cmds[i], "-u") == 0)
				if (ft_strcmp(term->cmds[i + 1], lst->var) == 0)
					break ;
		}
		if (!term->cmds[i])
		{
			ft_putstr(lst->var);
			ft_putstr("=");
			ft_putendl(lst->val);
		}
		lst = lst->next;
	}
	i = 0;
	while (term->cmds[++i])
		(ft_strcmp(term->cmds[i], "-u") == 0) ? i++ :\
		ft_putendl(term->cmds[i]);
}

void		ft_display_i(t_term *term)
{
	int i;

	i = 0;
	while (term->cmds[++i])
	{
		if (ft_strchr(term->cmds[i], '='))
			ft_putendl(term->cmds[i]);
	}
}
