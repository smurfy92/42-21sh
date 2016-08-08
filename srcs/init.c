/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 10:20:55 by jtranchi          #+#    #+#             */
/*   Updated: 2016/08/08 10:24:55 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_get_window(t_term *term)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	if (!term->window)
		term->window = (t_window*)malloc(sizeof(t_window));
	term->window->width = w.ws_col;
	term->window->heigth = w.ws_row;
}

t_term		*ft_get_term(void)
{
	static t_term *term;

	if (!term)
		term = (t_term*)malloc(sizeof(t_term));
	ft_get_window(term);
	return (term);
}

void		ft_get_window_sig(void)
{
	struct winsize	w;
	t_term			*term;

	ioctl(0, TIOCGWINSZ, &w);
	term = ft_get_term();
}

void		ft_reset_term(t_term *term)
{
	init_shell((~ICANON & ~ECHO));
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->cmdsplit = NULL;
	term->path = NULL;
	term->cmdactual = NULL;
	ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
	ft_bzero(term->buf, ft_strlen(term->buf));
	write(1, "$> ", 3);
	ft_get_window(term);
}
