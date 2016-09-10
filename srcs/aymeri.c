/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aymeric.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 14:19:02 by jtranchi          #+#    #+#             */
/*   Updated: 2016/03/03 14:19:05 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_father(t_term *term, int *tabl)
{
	char	*tmp;
	int		y;

	tmp = NULL;
	dup2(tabl[1], STDOUT_FILENO);
	close(tabl[0]);
	(y = -1) ? ft_check_cmds(term) : 0;
	(term->i) ? ft_env_i(term) : 0;
	while (term->cmds[++y])
		(ft_strcmp(term->cmds[y], term->path) == 0) ?\
		term->cmds = &term->cmds[y] : 0;
	y = 0;
	(!term->cmds[1]) ? (tmp = term->cmds[0]) : 0;
	while (term->cmds[++y])
	{
		(!tmp) ? (tmp = ft_strdup(term->cmds[y - 1])) : 0;
		tmp = ft_strjoin(ft_strjoin(tmp, " "), term->cmds[y]);
	}
	execve(term->path, term->cmds, term->env);
	term->path = NULL;
}

void		ft_create_process(t_term *term)
{
	int		tabl[2];

	pipe(tabl);
	term->father = fork();
	ft_refresh_env(term);
	if (term->father == 0)
	{
		ft_father(term, tabl);
	}
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
}

int			ft_check_in_path(t_term *term)
{
	char		**tabl;
	int			i;

	i = -1;
	tabl = ft_strsplit(ft_get_env_by_name(term, "PATH"), ':');
	while (tabl && tabl[++i])
	{
		tabl[i] = ft_strjoin(tabl[i], "/");
		tabl[i] = ft_strjoin(tabl[i], term->cmds[0]);
		if (access(tabl[i], X_OK) == 0)
			term->path = ft_strdup(tabl[i]);
	}
	if (term->cmds[0][0] == '.' && term->cmds[0][1] == '/')
		(access(&term->cmds[0][2], X_OK) == 0) ? term->path =\
		ft_strdup(&term->cmds[0][2]) : ft_permission_denied(term->cmds[0]);
	if (term->path)
	{
		term->exec = 1;
		term->historylen++;
		return (1);
	}
	else
		ft_putendl(ft_strjoin("jush: command not found: ", term->cmds[0]));
	term->exec = 0;
	return (0);
}

void		ft_refresh_env(t_term *term)
{
	t_env		*lst;
	int			i;

	lst = term->lst;
	i = 0;
	while (lst)
	{
		term->env[i++] = ft_strjoin(ft_strjoin(lst->var, "="), lst->val);
		lst = lst->next;
	}
}

char		*ft_get_env_by_name(t_term *term, char *name)
{
	t_env	*lst;

	lst = term->lst;
	while (lst)
	{
		if (ft_strcmp(lst->var, name) == 0)
			return (lst->val);
		lst = lst->next;
	}
	return (NULL);
}
