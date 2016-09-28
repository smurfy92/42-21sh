/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/18 10:22:34 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/18 10:22:35 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_end_of_red(char c)
{
	if (c == '\t' || c == ' ' || c == '\v' || c == '\f' || c == '\r'
	|| c == '\n' || c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

void		ft_redirection_error(t_parse *parse, int end)
{
	char *tmp;

	tmp = ft_strjoin_nf(ft_strjoin("jush : parse error near `",
	&parse->cmd[end - 1]), "'", 1);
	ft_putendl_fd(tmp, 2);
	ft_strdel(&tmp);
}

void		ft_path_error(t_term *term)
{
	char *tmp;

	tmp = ft_strjoin("jush: command not found: ", term->parselst->cmd);
	ft_putendl_fd(tmp, 2);
	ft_strdel(&tmp);
	term->exec = 0;
}
