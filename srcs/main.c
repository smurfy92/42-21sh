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

void		ft_separators(t_term *term)
{
	term->separators = NULL;
	if (term->cmdtmp)
		(term->cmdactual) ? (term->cmdtmp = ft_strjoin_nf(term->cmdtmp,
		term->cmdactual, 1)) : 0;
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

void		ft_boucle(t_term *term)
{
	int		ret;

	while (42)
	{
		while ((ret = (read(0, term->buf, BUFFSIZE))) && term->buf[0] != 10)
		{
			if (!term->intro)
				term->intro = ft_strdup(term->buf);
			else
				term->intro = ft_strjoin_nf(term->intro, term->buf, 1);
			ft_process(term);
		}
		if (ret == 0)
		{
			term->intro[ft_strlen(term->intro) - 1] = '\0';
			ft_putchar_fd('\n', 2);
			ft_process_exec(term, term->intro);
			exit(0);
		}
		ft_strdel(&(term->intro));
		ft_separators(term);
		if (!term->separators)
			break ;
	}
	if (term->cmdtmp)
	{
		(term->cmdactual) ? ft_strdel(&(term->cmdactual)) : 0;
		term->cmdactual = ft_strdup(term->cmdtmp);
	}
	ft_strdel(&(term->cmdtmp));
}

void		ft_cmdsplit(t_term *term, int argc)
{
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
		ft_cmdsplit(term, argc);
	}
	return (0);
}
