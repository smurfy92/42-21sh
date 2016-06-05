/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 22:03:08 by jtranchi          #+#    #+#             */
/*   Updated: 2016/06/05 22:03:09 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_is_shift_end(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 49
	&& term->buf[3] == 59 && term->buf[4] == 50 && term->buf[5] == 70)
		return (1);
	else
		return (0);
}

int			ft_is_home(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 72
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_end(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 70
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_left_arrow(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 68
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}

int			ft_is_right_arrow(t_term *term)
{
	if (term->buf[0] == 27 && term->buf[1] == 91 && term->buf[2] == 67
	&& term->buf[3] == 0 && term->buf[4] == 0 && term->buf[5] == 0)
		return (1);
	else
		return (0);
}
