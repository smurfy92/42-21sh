/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/21 10:02:32 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/21 10:02:33 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_go_up(t_term *term)
{
	int i;
	int y;

	i = -1;
	while (++i < term->window->width && term->cursorpos > 0)
	{
		if (((term->cursorpos + 3) % term->window->width) == 0)
		{
			y = 0;
			tputs(tgetstr("up", NULL), 0, ft_outchar);
			while (y++ < term->window->width)
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
		else
			tputs(tgetstr("le", NULL), 0, ft_outchar);
		term->cursorpos--;
	}
}

void		ft_go_down(t_term *term)
{
	int i;

	i = -1;
	while (++i < term->window->width && term->cursorpos < term->cmdlength)
	{
		term->cursorpos++;
		if (((term->cursorpos + 3) % term->window->width) == 0)
			tputs(tgetstr("do", NULL), 0, ft_outchar);
		else
			tputs(tgetstr("nd", NULL), 0, ft_outchar);
	}
}

void		ft_replace_cursor(t_term *term)
{
	int i;
	int y;

	i = -1;
	while (++i < (int)ft_strlen(&term->cmdactual[term->cursorpos]))
	{
		if (((term->cursorpos + 3 + (int)ft_strlen(&term->cmdactual
		[term->cursorpos]) - i) % term->window->width) == 0)
		{
			tputs(tgetstr("up", NULL), 1, ft_outchar);
			y = 0;
			while (y++ < term->window->width)
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
		else
			tputs(tgetstr("le", NULL), 0, ft_outchar);
	}
}

void		ft_print_buf(t_term *term, char *str)
{
	int i;

	ft_get_cmd(term, str);
	if (term->cursorpos < term->cmdlength)
	{
		i = term->cursorpos - 1;
		while (term->cmdactual[++i])
			ft_putchar(term->cmdactual[i]);
		if (((i + 3) % term->window->width) == 0)
			tputs(tgetstr("do", NULL), 0, ft_outchar);
		ft_replace_cursor(term);
	}
}

void		ft_shift_left(t_term *term)
{
	term->copy = ft_strdup(term->cmdactual);
	term->copy[term->cursorpos] = '\0';
}

void		ft_go_left_word(t_term *term)
{
	int y;

	while (term->cursorpos > 0)
	{
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
		if (term->cursorpos > 0 && term->cmdactual[term->cursorpos - 1] == ' ')
			break;
	}

}

void		ft_process2(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 53)
		ft_go_up(term);
	else if(term->buf[0] == 27 && term->buf[2] == 49 && term->buf[5] == 65)
		term->copy = ft_strdup(term->cmdactual);
	else if(term->buf[0] == 27 && term->buf[2] == 49 && term->buf[5] == 66)
		ft_print_buf(term, term->copy);
	else if(term->buf[0] == 27 && term->buf[2] == 49 && term->buf[5] == 67)
		term->copy = ft_strdup(&term->cmdactual[term->cursorpos]);
	else if(term->buf[0] == 27 && term->buf[2] == 49 && term->buf[5] == 68)
		ft_shift_left(term);
	else if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 54)
		ft_go_down(term);
	else if (term->buf[0] == 45)
		ft_go_left_word(term);
	else if (term->buf[0] == 43)
		ft_putstr("c'est bien un plus");
}

void		ft_process(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 72)
		ft_go_home(term);
	else if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 70)
		ft_go_end(term);
	else if (term->buf[0] == 127 && term->cursorpos > 0)
		ft_backspace(term);
	else if (term->buf[0] == 27 && term->buf[2] == 68 && term->cursorpos > 0)
		ft_left_arrow(term);
	else if (term->buf[0] == 27 && term->buf[2] == 67
	&& term->cursorpos < term->cmdlength)
		ft_right_arrow(term);
	else if (term->buf[0] == 27 && term->buf[2] == 65)
		ft_history_prev(term);
	else if (term->buf[0] == 27 && term->buf[2] == 66)
		ft_history_next(term);
	else if (term->buf[0] != 27 && term->buf[0] != 127 && term->buf[0] != 43
	&& term->buf[0] != 45 && ft_isprint(term->buf[0]))
		ft_print_buf(term, term->buf);
	else
		ft_process2(term);
	ft_bzero(term->buf, ft_strlen(term->buf));
}
