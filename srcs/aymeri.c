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

#include "../includes/21.h"

void		ft_create_process(t_term *term)
{
	int 	y;
	int 	i;
	char 	*tmp;

	ft_refresh_env(&term);
	term->father = fork();
	tmp = NULL;
	if (term->father != 0)
		wait(0);
	if (term->father == 0)
	{
		ft_check_cmds(term);
		if (term->i)
			ft_env_i(term);
		y = -1;
		while (term->cmds[++y])
			(ft_strcmp(term->cmds[y], term->path) == 0) ?\
			term->cmds = &term->cmds[y] : 0;
		i = 0;
		(!term->cmds[1]) ? (tmp = term->cmds[0]) : 0;
		while (term->cmds[++i])
		{
			if (!tmp)
				tmp = ft_strdup(term->cmds[i - 1]);
			tmp = ft_strjoin(tmp, " ");
			tmp = ft_strjoin(tmp, term->cmds[i]);

		}
		i = open(".21sh_history", O_WRONLY|O_APPEND|O_CREAT);
		write(i, tmp, ft_strlen(tmp));
		write(i, "\n", 1);
		close(i);
		execve(term->path, term->cmds, term->env);
		term->path = NULL;
	}
}

int			ft_check_in_path(t_term *term)
{
	char		**tabl;
	int			i;

	i = -1;
	tabl = ft_strsplit(ft_get_env_by_name(term, "PATH"), ':');
	while (tabl[++i])
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
		ft_create_process(term);
		term->historylen++;
		return (1);
	}
	else
		ft_putendl(ft_strjoin("21sh: command not found: ", term->cmds[0]));
	return (0);
}

void		ft_refresh_env(t_term **term)
{
	t_env		*lst;
	int			i;

	lst = (*term)->lst;
	i = 0;
	while (lst)
	{
		(*term)->env[i++] = ft_strjoin(ft_strjoin(lst->var, "="), lst->val);
		lst = lst->next;
	}
}

void		ft_prompt(int nb)
{
	nb = 0;
	ft_putstr("\n$> ");
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
