/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 15:40:06 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/08 15:40:07 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_del_last_in_seperator2(t_term *term, int i)
{
	char	*tmp;

	tmp = &term->separators[i + 1];
	term->separators[i] = '\0';
	if (term->separators[0] == '\0')
		term->separators = ft_strdup(tmp);
	else
	{
		if (tmp)
			term->separators = ft_strjoin(term->separators, tmp);
	}
}

void		ft_del_last_in_seperator(t_term *term, char c)
{
	int		i;

	i = ft_strlen(term->separators);
	while (term->separators && term->separators[--i])
	{
		if (term->separators[i] == c)
		{
			ft_check_last_in_cmd(term, c);
			if (!term->separators[i + 1])
			{
				if (i == 0)
					term->separators = NULL;
				else
					term->separators[i] = '\0';
				return ;
			}
			if (i == 0)
			{
				term->separators = &term->separators[1];
				return ;
			}
			ft_del_last_in_seperator2(term, i);
			return ;
		}
	}
}
