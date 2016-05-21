/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/21 11:32:12 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/21 11:32:13 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21.h"

void		ft_go_home(t_term *term)
{
	int i;
	int verif;

	verif = 0;
	if (term->cursorpos > 0)
	{
		while (--term->cursorpos > 0)
		{
			if (((term->cursorpos + 3) % term->window->width) == 0)
			{
				i = 0;
				verif = 1;
				tputs(tgetstr("up", NULL), 0, ft_outchar);
				while (i++ < term->window->width)
					tputs(tgetstr("nd", NULL), 0, ft_outchar);
			}
			else
				tputs(tgetstr("le", NULL), 0, ft_outchar);
		}
		(!verif) ? (tputs(tgetstr("le", NULL), 0, ft_outchar)) : 0;
	}
}

void		ft_go_end(t_term *term)
{
	if (term->cursorpos < term->cmdlength)
	{
		tputs(tgetstr("nd", NULL), 0, ft_outchar);
		while (++term->cursorpos < term->cmdlength)
		{
			if (((term->cursorpos + 4) % term->window->width) == 0)
				tputs(tgetstr("do", NULL), 0, ft_outchar);
			else
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
	}
}

void		ft_backspace(t_term *term)
{
	char *tmp;

	tmp = &term->cmdactual[term->cursorpos];
	term->cursorpos--;
	term->cmdlength--;
	term->cmdactual[term->cursorpos] = '\0';
	term->cmdactual = ft_strjoin(term->cmdactual, tmp);
	tputs(tgetstr("le", NULL), 0, ft_outchar);
	tputs(tgetstr("dc", NULL), 0, ft_outchar);
	tputs(tgetstr("cd", NULL), 0, ft_outchar);
	tputs(tgetstr("sc", NULL), 0, ft_outchar);
	ft_putstr(&term->cmdactual[term->cursorpos]);
	tputs(tgetstr("rc", NULL), 0, ft_outchar);
}

void		ft_left_arrow(t_term *term)
{
	int i;

	if (((term->cursorpos + 3) % term->window->width) == 0)
	{
		i = 0;
		tputs(tgetstr("up", NULL), 0, ft_outchar);
		while (++i < term->window->width)
			tputs(tgetstr("nd", NULL), 0, ft_outchar);
	}
	else
		tputs(tgetstr("le", NULL), 0, ft_outchar);
	term->cursorpos--;
}

void		ft_right_arrow(t_term *term)
{
	term->cursorpos++;
	if (((term->cursorpos + 3) % term->window->width) == 0)
		tputs(tgetstr("do", NULL), 0, ft_outchar);
	else
		tputs(tgetstr("nd", NULL), 0, ft_outchar);
}
