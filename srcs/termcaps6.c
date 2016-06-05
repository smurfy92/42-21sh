/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 22:04:05 by jtranchi          #+#    #+#             */
/*   Updated: 2016/06/05 22:04:06 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_is_shift_home(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 72)
		return (1);
	else
		return (0);
}

int			ft_is_up_arrow(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 65
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_down_arrow(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 66
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_page_up(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 53
	&& term->buf[3] == 126 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_page_down(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 54
	&& term->buf[3] == 126 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}
