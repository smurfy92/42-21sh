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
	int		tabl[2];

	pipe(tabl);
	term->father = fork();
	if (term->father == 0)
		ft_father(term, tabl);
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
}

void		ft_check_in_path2(t_term *term)
{
	if (term->cmds[0][0] == '.' && term->cmds[0][1] == '/' && term->cmds[0][2])
		(access(&term->cmds[0][2], X_OK) == 0) ? term->path =\
		ft_strdup(&term->cmds[0][2]) : 0;
	if (term->cmds[0][0] == '/')
		(access(term->cmds[0], X_OK) == 0) ? term->path =\
		ft_strdup(term->cmds[0]) : 0;
}

int			ft_check_in_path(t_term *term)
{
	char		**tabl;
	int			i;

	i = -1;
	tabl = ft_strsplit(ft_get_env_by_name(term, "PATH"), ':');
	while (tabl && tabl[++i])
	{
		tabl[i] = ft_strjoin_nf(tabl[i], "/", 1);
		tabl[i] = ft_strjoin_nf(tabl[i], term->cmds[0], 1);
		if (access(tabl[i], X_OK) == 0)
			term->path = ft_strdup(tabl[i]);
		free(tabl[i]);
	}
	free(tabl);
	ft_check_in_path2(term);
	if (term->path)
	{
		term->exec = 1;
		term->historylen++;
		return (1);
	}
	ft_putendl_fd(ft_strjoin("jush: command not found: ",
	term->parselst->cmd), 2);
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
		term->env[i++] = ft_strdup(ft_strjoin(ft_strjoin(lst->var, "="),
		lst->val));
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
