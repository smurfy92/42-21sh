/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/08 12:46:12 by jtranchi          #+#    #+#             */
/*   Updated: 2016/04/08 12:46:13 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minish.h"

void		ft_env_i(t_term *term)
{
	int y;
	int i;

	y = -1;
	while (term->env[++y])
		term->env[y] = NULL;
	i = -1;
	while (term->cmds[++i])
	{
		if (ft_strchr(term->cmds[i], '='))
		{
			y = 0;
			while (term->env[y])
				y++;
			term->env[y] = ft_strdup(term->cmds[i]);
		}
	}
}
