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
	return (write(1, &c, 1));
}

void		ft_add_history(t_term *term, char *cmd)
{
	int			fd;
	t_history	*tmp;
	t_history	*tmp2;

	fd = open(".21sh_history", O_WRONLY | O_APPEND | O_CREAT,
	S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	write(fd, cmd, ft_strlen(cmd));
	tmp2 = term->history;
	tmp = (t_history*)malloc(sizeof(t_history));
	tmp->var = cmd;
	tmp->next = NULL;
	write(fd, "\n", 1);
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
}

void		ft_history_prev(t_term *term)
{
	if (!term->history->next && term->cmdactual && !term->inhistory)
	{
		ft_add_history(term, term->cmdactual);
		(term->history->prev) ? (term->history = term->history->prev) : 0;
	}
	ft_clean_line(term);
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->cmdactual = NULL;
	if (term->history->prev && !term->inhistory)
	{
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr(term->history->var);
		term->inhistory = 1;
	}
	else if (term->history->prev)
	{
		term->history = term->history->prev;
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr(term->history->var);
	}
}

void		ft_history_next(t_term *term)
{
	ft_clean_line(term);
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->cmdactual = NULL;
	if (term->history->next)
	{
		term->history = term->history->next;
		term->cursorpos = ft_strlen(term->history->var);
		term->cmdlength = ft_strlen(term->history->var);
		term->cmdactual = ft_strdup(term->history->var);
		ft_putstr(term->history->var);
	}
	else
		term->inhistory = 0;
}

void		ft_get_history(t_term *term)
{
	int			fd;
	t_history	*tmp;
	char		*line;

	fd = open(".21sh_history", O_RDONLY);
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
	}
}
