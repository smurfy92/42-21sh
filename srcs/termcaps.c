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
		if (((term->cursorpos + 7) % term->window->width) == 0)
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

void		ft_process4(t_term *term)
{
	if (term->buf[0] == 4 && ft_strlen(term->cmdactual) == 0)
	{
		ft_putchar_fd('\n', 2);
		reset_shell();
		exit(0);
	}
	else if (term->buf[0] == 4 && term->cursorpos < term->cmdlength)
	{
		ft_right_cursor(term);
		ft_backspace(term);
	}
	else if (ft_is_printable(term->buf))
		ft_print_buf(term, term->buf);
}

void		ft_process3(t_term *term)
{
	int i;

	i = 1;
	if (ft_is_shift_down(term))
	{
		if (!term->copy)
			return ;
		ft_print_buf(term, term->copy);
		while (i++ < (int)ft_strlen(term->copy))
			ft_right_arrow(term);
	}
	else if (term->buf[0] == 4 && term->inheredoc)
		ft_ctrl_d(term);
	else
		ft_process4(term);
}

void		ft_process2(t_term *term)
{
	if (ft_is_page_up(term))
		ft_go_up(term);
	else if (ft_is_page_down(term))
		ft_go_down(term);
	else if (term->buf[0] == 127)
		(term->cursorpos > 0) ? ft_backspace(term) : 0;
	else if (ft_is_left_arrow(term))
		ft_left_arrow(term);
	else if (ft_is_right_arrow(term))
		ft_right_arrow(term);
	else if (ft_is_up_arrow(term))
		ft_history_prev(term);
	else if (ft_is_down_arrow(term))
		ft_history_next(term);
	else if (ft_is_left_arrow(term))
		ft_left_arrow(term);
	else if (ft_is_right_arrow(term))
		ft_right_arrow(term);
	else if (term->buf[0] == 9)
		return ;
	else
		ft_process3(term);
}

void		ft_process(t_term *term)
{
	if (ft_is_shift_up(term))
		(term->cmdlength > 0) ? (term->copy = ft_strdup(term->cmdactual)) : 0;
	else if (ft_is_shift_right(term))
		(term->cursorpos < term->cmdlength) ? (term->copy = \
		ft_strdup(&term->cmdactual[term->cursorpos])) : 0;
	else if (ft_is_shift_left(term))
		ft_shift_left(term);
	else if (ft_is_shift_home(term))
		ft_prev_word(term);
	else if (ft_is_shift_end(term))
		ft_next_word(term);
	else if (ft_is_home(term))
		ft_go_home(term);
	else if (ft_is_end(term))
		ft_go_end(term);
	else if (term->buf[0] == 127)
		(term->cursorpos > 0) ? ft_backspace(term) : 0;
	else if (ft_is_up_arrow(term))
		ft_history_prev(term);
	else if (ft_is_down_arrow(term))
		ft_history_next(term);
	else
		ft_process2(term);
	ft_bzero(term->buf, ft_strlen(term->buf));
}
