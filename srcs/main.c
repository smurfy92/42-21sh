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
		if (term->parselst->close1)
			close(1);
		if (term->parselst->close2)
			close(2);
		term->cmds = ft_strsplit(term->parselst->cmd, ' ');
		if (ft_check_builtin(term) || ft_check_in_path(term))
		{
			ft_refresh_env(term);
			if (ft_check_builtin(term))
				ft_builtin_fork(term);
			if (term->parselst->file)
				ft_create_file_dup(term);
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
	term->parselst = NULL;
	term->fail = 0;
	ft_parse(term, cmdsplit);
	if (term->fail)
		return ;
	term->cmds = ft_strsplit(term->parselst->cmd, ' ');
	if (term->parselst->heredoc)
		ft_create_heredoc(term);
	if (ft_check_builtin(term) && !term->parselst->next)
	{
		ft_create_builtin(term);
		return ;
	}
	if (ft_check_in_path(term))
		father = fork();
	else
		return ;
	if (father == 0)
		ft_father_process(term);
	wait(0);
}

void		ft_boucle(t_term *term)
{
	while (term->separators)
	{
		term->separators = NULL;
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
			ft_process(term);
		ft_check_separators(term);
		if (term->separators)
		{
			term->cmdactual = ft_strjoin(term->cmdactual, "\n");
			term->cmdlength++;
			term->cursorpos++;
			ft_putstr("\n> ");
		}
	}
}

int			main(int argc, char **argv, char **env)
{
	t_term		*term;

	argv = NULL;
	term = ft_set_term(ft_get_term(), env, ft_parse_env(env));
	signal(SIGWINCH, ft_get_window_sig);
	while (42)
	{
		argc = -1;
		ft_get_history(term);
		ft_reset_term(term);
		term->separators = "";
		ft_boucle(term);
		(ft_strlen(term->cmdactual) > 0) ?
		ft_add_history(term, term->cmdactual) : 0;
		reset_shell();
		ft_putchar('\n');
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
			ft_process_exec(term, term->cmdsplit[argc]);
	}
	return (0);
}
