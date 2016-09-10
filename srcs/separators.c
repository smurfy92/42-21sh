/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:25:10 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/06 20:24:40 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_del_first_in_cmd(t_term *term, char c)
{
	int i;

	i = -1;
	while (term->cmdactual && term->cmdactual[++i])
	{
		if (term->cmdactual[i] == c)
		{
			term->cmdactual[i] = ' ';
			return ;
		}
	}
}

void		ft_del_first_in_separator2(t_term *term, int i)
{
	char	*tmp;

	tmp = &term->separators[i + 1];
	term->separators[i] = '\0';
	if (term->separators[0] == '\0')
		term->separators = ft_strdup(tmp);
	else
		(tmp) ? (term->separators =
			ft_strjoin(term->separators, tmp)) : 0;
}

void		ft_del_first_in_separator(t_term *term, char c)
{
	int		i;

	i = -1;
	while (term->separators && term->separators[++i])
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
			ft_del_first_in_separator2(term, i);
			return ;
		}
	}
}

void		ft_add_separator(t_term *term, char c)
{
	char *str;

	str = ft_strnew(1);
	str[0] = c;
	str[1] = '\0';
	if (ft_has_separator(term, '(') && c == ')')
		return (ft_del_first_in_separator(term, '('));
	if (ft_has_separator(term, '[') && c == ']')
		return (ft_del_first_in_separator(term, '['));
	if (ft_has_separator(term, '{') && c == '}')
		return (ft_del_first_in_separator(term, '{'));
	if (ft_has_separator(term, 39) && c == 39)
		return (ft_del_first_in_separator(term, 39));
	if (ft_has_separator(term, '"') && c == '"')
		return (ft_del_first_in_separator(term, '"'));
	(!term->separators) ? (term->separators = ft_strdup(str)) :
	(term->separators = ft_strjoin(term->separators, str));
}

void		ft_check_separators(t_term *term)
{
	int i;

	i = -1;
	while (term->cmdactual && term->cmdactual[++i])
	{
		if (ft_is_separator(term->cmdactual[i]))
			ft_add_separator(term, term->cmdactual[i]);
	}
}
