/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 17:39:35 by jtranchi          #+#    #+#             */
/*   Updated: 2016/02/15 17:39:36 by jtranchi         ###   ########.fr       */
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
			if (ft_check_builtin(term))
				ft_builtin_fork(term);
			if (term->parselst->file)
			{
				ft_create_file_dup(term);
				term->parselst->file = NULL;
			}
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

void		ft_process_exec(t_term *term, char *cmdsplit)
{
	pid_t	father;
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
	if (term->parselst->heredoc)
		ft_create_heredoc(term);
	if (term->fail)
		return ;
	if (ft_check_builtin(term) && !term->parselst->next)
	{
		ft_create_builtin(term);
		ft_free_cmds(term);
		return ;
	}
	if (ft_check_in_path(term))
		father = fork();
	else
	{
		ft_free_cmds(term);
		return ;
	}
	ft_free_cmds(term);
	(father == 0) ? ft_father_process(term) : 0;
	wait(0);
}

void		ft_boucle(t_term *term)
{
	while (term->separators)
	{
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
			ft_process(term);
		term->separators = NULL;
		if (term->cmdtmp)
			term->cmdtmp = ft_strjoin_nf(term->cmdtmp, term->cmdactual, 3);
		else
			(term->cmdactual) ? term->cmdtmp = ft_strdup(term->cmdactual) : 0;
		if (term->cmdtmp)
		{
			(term->cmdactual) ? ft_strdel(&(term->cmdactual)) : 0;
			term->cmdactual = ft_strdup(term->cmdtmp);
		}
		ft_strdel(&(term->cmdtmp));
		ft_check_separators(term);
		if (term->separators)
		{
			term->cmdactual = ft_strjoin_nf(term->cmdactual, "\n", 1);
			term->cmdtmp = ft_strdup(term->cmdactual);
			ft_strdel(&(term->cmdactual));
			ft_go_end(term);
			ft_reset_term(term);
			ft_putstr_fd("\n> ", 2);
		}
	}
	if (term->cmdtmp)
	{
		ft_strdel(&(term->cmdactual));
		term->cmdactual = ft_strdup(term->cmdtmp);
	}
	ft_strdel(&(term->cmdtmp));
}

int			main(int argc, char **argv, char **env)
{
	t_term		*term;

	argv = NULL;
	term = ft_set_term(ft_get_term(), env, ft_parse_env(env));
	ft_handle_signal();
	while (42)
	{
		argc = -1;
		(!term->history) ? ft_get_history(term) : 0;
		(!term->test) ? ft_reset_term(term) : (term->test = 0);
		ft_boucle(term);
		(ft_strlen(term->cmdactual) > 0) ?
		ft_add_history(term, term->cmdactual) : 0;
		reset_shell();
		(!term->test) ? ft_putchar_fd('\n', 2) : 0;
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			ft_process_exec(term, term->cmdsplit[argc]);
			(term->parselst) ? ft_free_parse(term) : 0;
			ft_strdel(&(term->cmdsplit[argc]));
			(term->path) ? ft_strdel(&(term->path)) : 0;
		}
		free(term->cmdsplit);
	}
	return (0);
}
