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

void		ft_create_process(t_term *term)
{
	pipe(term->pipe);
	term->father = fork();
	if (term->father == 0)
		ft_father(term);
	dup2(term->pipe[0], STDIN_FILENO);
	close(term->pipe[1]);
}

int			ft_check_in_path2(t_term *term)
{
	if (term->cmds[0][0] == '.' && term->cmds[0][1] == '/' && term->cmds[0][2])
		(access(&term->cmds[0][2], X_OK) == 0) ? term->path =\
		ft_strdup(&term->cmds[0][2]) : 0;
	if (term->cmds[0][0] == '/')
		(access(term->cmds[0], X_OK) == 0) ? term->path =\
		ft_strdup(term->cmds[0]) : 0;
	if (term->path)
	{
		term->exec = 1;
		term->historylen++;
		return (1);
	}
	return (0);
}

int			ft_check_in_path(t_term *term)
{
	char		**tabl;
	int			i;
	char		*tmp;

	i = -1;
	tabl = ft_strsplit(ft_get_env_by_name(term, "PATH"), ':');
	if (term->path)
		ft_strdel(&(term->path));
	while (tabl && tabl[++i])
	{
		tmp = ft_strjoin(tabl[i], "/");
		ft_strdel(&tabl[i]);
		tmp = ft_strjoin_nf(tmp, term->cmds[0], 1);
		if (access(tmp, X_OK) == 0 && !term->path)
		{
			term->path = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		ft_strdel(&tmp);
	}
	free(tabl);
	if (ft_check_in_path2(term))
		return (1);
	ft_path_error(term);
	return (0);
}

void		ft_refresh_env(t_term *term)
{
	t_env		*lst;
	int			i;
	char		*tmp;

	lst = term->lst;
	i = 0;
	while (lst)
	{
		tmp = ft_strjoin(lst->var, "=");
		term->env[i++] = ft_strjoin_nf(tmp, lst->val, 1);
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
