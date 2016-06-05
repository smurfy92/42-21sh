/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/22 13:06:36 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/22 13:06:37 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_shift_left(t_term *term)
{
	if (term->cursorpos == 0)
		return ;
	term->copy = ft_strdup(term->cmdactual);
	term->copy[term->cursorpos] = '\0';
}

void		ft_left_cursor(t_term *term)
{
	int y;

	term->cursorpos--;
	if (((term->cursorpos + 3) % term->window->width) == 0)
	{
		tputs(tgetstr("up", NULL), 1, ft_outchar);
		y = 0;
		while (y++ < term->window->width)
			tputs(tgetstr("nd", NULL), 0, ft_outchar);
	}
	else
		tputs(tgetstr("le", NULL), 0, ft_outchar);
}

void		ft_right_cursor(t_term *term)
{
	term->cursorpos++;
	if (((term->cursorpos + 3) % term->window->width) == 0)
		tputs(tgetstr("do", NULL), 0, ft_outchar);
	else
		tputs(tgetstr("nd", NULL), 0, ft_outchar);
}

void		ft_prev_word(t_term *term)
{
	while (term->cursorpos > 0 && term->cmdactual[term->cursorpos - 1] == ' ')
		ft_left_cursor(term);
	while (term->cursorpos > 0)
	{
		if (term->cursorpos > 0 && term->cmdactual[term->cursorpos - 1] == ' ')
			break ;
		ft_left_cursor(term);
	}
}

void		ft_next_word(t_term *term)
{
	int bol;

	bol = 0;
	while (term->cursorpos < term->cmdlength &&
	(term->cmdactual[term->cursorpos] == ' ' || bol == 0))
	{
		if (term->cmdactual[term->cursorpos] == ' ')
			bol = 1;
		ft_right_cursor(term);
	}
}
