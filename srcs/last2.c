/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/14 15:10:50 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/14 15:10:51 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_check_close1(t_parse *parse, int i)
{
	char *tmp;

	if (parse->cmd[i + 4])
	{
		tmp = ft_strdup(&parse->cmd[i + 4]);
		parse->cmd[i] = '\0';
		parse->cmd = ft_strjoin(parse->cmd, tmp);
	}
	else
		parse->cmd[i] = '\0';
	parse->close1 = 1;
}

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
