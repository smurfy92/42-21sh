/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/28 13:27:10 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/28 13:27:10 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_builtin_fork(t_term *term)
{
	int			tabl[2];
	int			father;
	t_parse		*tmp;

	pipe(tabl);
	father = fork();
	if (father == 0)
	{
		dup2(tabl[1], STDOUT_FILENO);
		close(tabl[0]);
		ft_create_builtin(term);
		exit(0);
	}
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
	tmp = term->parselst->next;
	ft_strdel(&(term->parselst->cmd));
	ft_strdel(&(term->parselst->dbred));
	ft_strdel(&(term->parselst->sgred));
	ft_strdel(&(term->parselst->file));
	free(term->parselst);
	term->parselst = tmp;
	ft_father_process(term);
}

void		ft_check(t_term *term)
{
	if (ft_check_builtin(term))
		ft_builtin_fork(term);
	if (term->parselst->file)
	{
		ft_create_file_dup(term);
		term->parselst->file = NULL;
	}
}

void		ft_father_process(t_term *term)
{
	int		i;

	i = -1;
	while (term->parselst)
	{
		(term->parselst->close1) ? close(1) : 0;
		(term->parselst->close2) ? close(2) : 0;
		term->cmds = ft_strsplit(term->parselst->cmd, ' ');
		if (ft_check_builtin(term) || ft_check_in_path(term))
		{
			ft_refresh_env(term);
			ft_check(term);
			if (!term->parselst->next && !term->parselst->sgred &&
			!term->parselst->dbred)
			{
				execve(term->path, term->cmds, term->env);
				break ;
			}
			ft_father_exec(term);
		}
		i = -1;
		ft_free_cmds(term);
		term->parselst = term->parselst->next;
	}
}

int			ft_process_exec2(t_term *term)
{
	if (ft_check_builtin(term) && !term->parselst->next)
	{
		ft_create_builtin(term);
		ft_free_cmds(term);
		return (0);
	}
	if (ft_check_in_path(term))
		term->father = fork();
	else
	{
		ft_free_cmds(term);
		return (0);
	}
	return (1);
}

void		ft_process_exec(t_term *term, char *cmdsplit)
{
	int		i;

	term->cmds = NULL;
	term->parselst = NULL;
	term->fail = 0;
	term->path = NULL;
	i = -1;
	ft_parse(term, cmdsplit);
	term->parselststart = term->parselst;
	if (term->fail)
		return ;
	term->cmds = ft_strsplit(term->parselst->cmd, ' ');
	(term->parselst->heredoc) ? ft_create_heredoc(term) : 0;
	if (term->fail)
		return ;
	if (!ft_process_exec2(term))
		return ;
	term->inexec = 1;
	ft_free_cmds(term);
	(term->father == 0) ? ft_father_process(term) : 0;
	wait(0);
	term->inexec = 0;
}
