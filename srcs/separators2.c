/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 20:24:44 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/06 20:24:44 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_is_separator(char c)
{
	if (c == 39 || c == '"' || c == '(' || c == ')' || c == '['
	|| c == ']' || c == '{' || c == '}')
		return (1);
	else
		return (0);
}

int			ft_has_separator(t_term *term, char c)
{
	int i;

	i = -1;
	while (term->separators && term->separators[++i])
	{
		if (term->separators[i] == c)
			return (1);
	}
	return (0);
}

void		ft_check_last_in_cmd(t_term *term, char c)
{
	if (c == '{')
	{
		ft_del_first_in_cmd(term, '}');
		ft_del_last_in_cmd(term, '{');
	}
	if (c == '[')
	{
		ft_del_first_in_cmd(term, ']');
		ft_del_last_in_cmd(term, '[');
	}
	if (c == '(')
	{
		ft_del_first_in_cmd(term, ')');
		ft_del_last_in_cmd(term, '(');
	}
	if (c == 39)
	{
		ft_del_first_in_cmd(term, 39);
		ft_del_first_in_cmd(term, 39);
	}
	if (c == '"')
	{
		ft_del_first_in_cmd(term, '"');
		ft_del_first_in_cmd(term, '"');
	}
}

void		ft_del_last_in_cmd(t_term *term, char c)
{
	int		i;

	i = ft_strlen(term->cmdactual);
	while (i > 1 && term->cmdactual && term->cmdactual[--i])
	{
		if (term->cmdactual[i] == c)
		{
			term->cmdactual[i] = ' ';
			return ;
		}
	}
}
