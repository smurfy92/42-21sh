/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 21:43:34 by jtranchi          #+#    #+#             */
/*   Updated: 2016/06/05 21:43:35 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_is_shift_left(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 68)
		return (1);
	else
		return (0);
}

int			ft_is_shift_right(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 67)
		return (1);
	else
		return (0);
}

int			ft_is_shift_down(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 66)
		return (1);
	else
		return (0);
}

int			ft_is_shift_up(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 65)
		return (1);
	else
		return (0);
}

void		ft_print_buf(t_term *term, char *str)
{
	int i;

	i = term->cursorpos - 1;
	ft_get_cmd(term, str);
	if (term->cursorpos < term->cmdlength)
	{
		while (term->cmdactual[++i])
			ft_putchar(term->cmdactual[i]);
		if (((i + 3) % term->window->width) == 0)
			tputs(tgetstr("do", NULL), 0, ft_outchar);
		ft_replace_cursor(term);
	}
	if (((term->cursorpos + 3) % term->window->width) == 0)
		tputs(tgetstr("do", NULL), 0, ft_outchar);
}
