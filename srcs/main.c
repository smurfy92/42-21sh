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
	int tabl[2];
	int father;

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
	term->parselst = term->parselst->next;
	ft_father_process(term);
}

void		ft_father_process(t_term *term)
{
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
		term->parselst = term->parselst->next;
	}
}

void		ft_process_exec(t_term *term, char *cmdsplit)
{
	int father;

	term->cmds = NULL;
	term->fail = 0;
	term->parselst = NULL;
	ft_parse(term, ft_check_cmdsplit(term, cmdsplit));
	//ft_check_parse(term);
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
		return ;
	}
	if (ft_check_in_path(term))
		father = fork();
	else
		return ;
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
			term->cmdtmp = ft_strjoin(term->cmdtmp, term->cmdactual);
		else
			(term->cmdactual) ? term->cmdtmp = ft_strdup(term->cmdactual) : 0;
		(term->cmdtmp) ? term->cmdactual = ft_strdup(term->cmdtmp) : 0;
		ft_check_separators(term);
		if (term->separators)
		{
			term->cmdactual = ft_strjoin(term->cmdactual, "\n");
			term->cmdtmp = ft_strdup(term->cmdactual);
			ft_go_end(term);
			ft_reset_term(term);
			ft_putstr_fd("\n> ", 2);
		}
	}
	(term->cmdtmp) ? term->cmdactual = ft_strdup(term->cmdtmp) : 0;
	term->cmdtmp = NULL;
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
		ft_get_history(term);
		(!term->test) ? ft_reset_term(term) : (term->test = 0);
		ft_boucle(term);
		(ft_strlen(term->cmdactual) > 0) ?
		ft_add_history(term, term->cmdactual) : 0;
		reset_shell();
		(!term->test) ? ft_putchar_fd('\n', 2) : 0;
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
			ft_process_exec(term, term->cmdsplit[argc]);
	}
	return (0);
}
