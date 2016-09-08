/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 16:43:44 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/08 16:43:59 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_addfile(t_term *term, t_parse *parse, int i)
{
	int		start;
	int		end;
	char	*tmp;

	start = i - 1;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i])
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl("jush : parse error near `\\n'"));
	}
	tmp = ft_strsub(&parse->cmd[i], 0, end - i);
	parse->file = ft_strdup(tmp);
	parse->sgred = NULL;
	parse->dbred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}
