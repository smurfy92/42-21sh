/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/18 11:32:07 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/18 11:32:09 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_outchar(int c)
{
	return (write(2, &c, 1));
}

void		ft_add_history(t_term *term, char *cmd)
{
	int			fd;
	t_history	*tmp;
	t_history	*tmp2;

	fd = open("/tmp/.21sh_history", O_WRONLY | O_APPEND | O_CREAT,
	S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	ft_putendl_fd(cmd, fd);
	tmp2 = term->history;
	tmp = (t_history*)malloc(sizeof(t_history));
	tmp->var = cmd;
	tmp->next = NULL;
	close(fd);
	if (!term->history)
	{
		term->history = tmp;
		return ;
	}
	while (term->history->next)
		term->history = term->history->next;
	term->history->next = tmp;
	tmp->prev = term->history;
	term->history = term->history->next;
	term->inhistory = 0;
}

void		ft_history_prev(t_term *term)
{
	if (!term->history || !term->history->prev)
		return ;
	if (!term->history->next && term->cmdactual && !term->inhistory)
	{
		ft_add_history(term, term->cmdactual);
		(term->history->prev) ? (term->history = term->history->prev) : 0;
	}
	ft_clean_line(term);
	if (term->history->prev && term->inhistory)
		term->history = term->history->prev;
	term->inhistory = 1;
	term->cursorpos = ft_strlen(term->history->var);
	term->cmdlength = ft_strlen(term->history->var);
	term->cmdactual = ft_strdup(term->history->var);
	ft_putstr_fd(term->history->var, 2);
}

void		ft_history_next(t_term *term)
{
	if (!term->history)
		return ;
	if (term->cmdactual)
		ft_clean_line(term);
	if (term->history->next)
	{
		term->history = term->history->next;
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr_fd(term->history->var, 2);
	}
	else
		term->inhistory = 0;
}

void		ft_get_history(t_term *term)
{
	int			fd;
	t_history	*tmp;
	char		*line;

	fd = open("/tmp/.21sh_history", O_RDONLY);
	term->history = NULL;
	term->historylen = 0;
	term->historycurrent = 0;
	while ((get_next_line(fd, &line)) > 0)
	{
		tmp = (t_history*)malloc(sizeof(t_history));
		tmp->var = ft_strdup(line);
		tmp->next = NULL;
		tmp->prev = NULL;
		term->historylen++;
		if (!term->history)
			term->history = tmp;
		else
		{
			term->history->next = tmp;
			tmp->prev = term->history;
			term->history = term->history->next;
		}
		ft_strdel(&line);
	}
	close(fd);
}
