/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/26 10:46:39 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/26 10:46:39 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_free_cmds(t_term *term)
{
	int i;

	i = -1;
	while (term->cmds[++i])
		ft_strdel(&(term->cmds[i]));
	free(term->cmds);
}

void		ft_free_parse(t_term *term)
{
	t_parse		*parse;

	while (term->parselststart)
	{
		ft_strdel(&(term->parselststart->cmd));
		ft_strdel(&(term->parselststart->dbred));
		ft_strdel(&(term->parselststart->sgred));
		ft_strdel(&(term->parselststart->file));
		(term->parselststart->heredoc) ? ft_strdel(&(term->parselststart->heredoc)) : 0;
		parse = term->parselststart->next;
		free(term->parselststart);
		term->parselststart = parse;
	}
	free(term->parselststart);
}

void		ft_free_struct2(t_term *term)
{
	int i;

	i = -1;
	while (term->cmdsplit[++i])
		ft_strdel(&(term->cmdsplit[i]));
	ft_free_cmds(term);
	free(term->window);
	ft_strdel(&(term->path));
	ft_strdel(&(term->buf));
	ft_strdel(&(term->cmdtmp));
	ft_strdel(&(term->copy));
	free(term->cmdsplit);
}

void		ft_free_struct(t_term *term)
{
	t_env		*lst;
	t_history	*hist;

	ft_free_parse(term);
	while (term->lst)
	{
		ft_strdel(&(term->lst->var));
		ft_strdel(&(term->lst->val));
		lst = term->lst->next;
		free(term->lst);
		term->lst = lst;
	}
	while (term->history->prev)
		term->history = term->history->prev;
	while (term->history)
	{
		if (term->history->var)
			ft_strdel(&(term->history->var));
		term->history->prev = NULL;
		hist = term->history->next;
		free(term->history);
		term->history = hist;
	}
	ft_free_struct2(term);
	free(term);
}
