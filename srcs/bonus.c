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

#include "../includes/21.h"

void		ft_check_u(t_term *term, int i)
{
	int y;

	y = 0;
	while (ft_strcmp(term->cmds[i + 1], \
	ft_strsplit(term->env[y], '=')[0]) != 0)
		y++;
	while (term->env[++y])
	{
		term->env[y - 1] = ft_strdup(term->env[y]);
		(ft_strcmp(term->env[y - 1], term->env[y]) == 0) ?\
		(term->env[y] = NULL) : 0;
	}
}

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
				while (term->env[++y]);
				term->env[y] = ft_strdup(term->cmds[i]);
			}
		}
	}
}

int			ft_check_in_env(t_term *term, char *cmd)
{
	char		**tabl;
	int			i;
	t_env		*lst;

	lst = term->lst;
	i = -1;
	while (lst && ft_strcmp(lst->var, "PATH") != 0)
		lst = lst->next;
	tabl = ft_strsplit(lst->val, ':');
	while (tabl[++i])
	{
		tabl[i] = ft_strjoin(ft_strjoin(tabl[i], "/"), cmd);
		if (access(tabl[i], X_OK) == 0)
			term->path = "/usr/bin/env";
	}
	if (cmd[0] == '.' && cmd[1] == '/')
		(access(&cmd[2], X_OK) == 0) ? term->path =\
		ft_strdup(&cmd[2]) : ft_permission_denied(&cmd[0]);
	if (term->path)
	{
		ft_create_process(term);
		return (1);
	}
	else
		return (0);
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
