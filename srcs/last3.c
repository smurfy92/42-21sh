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

char		*ft_check_cmdsplit(t_term *term, char *str)
{
	int		i;
	char	*tmp;

	i = -1;
	while (str && str[++i])
	{
		if (str[i] == '~')
		{
			if (ft_get_val_exists(term, "HOME") && str[i + 1])
			{
				tmp = ft_strdup(&str[i + 1]);
				str[i] = '\0';
				str = ft_strjoin(str, ft_get_val(term, "HOME"));
				str = ft_strjoin(str, tmp);
			}
		}
	}
	return (str);
}

int			ft_check_exec(t_term *term, char *cmd)
{
	char		**tabl;
	int			i;
	char		**cmdsplit;

	i = -1;
	tabl = ft_strsplit(ft_get_env_by_name(term, "PATH"), ':');
	cmdsplit = ft_strsplit(cmd, ' ');
	while (tabl && tabl[++i])
	{
		tabl[i] = ft_strjoin(tabl[i], "/");
		tabl[i] = ft_strjoin(tabl[i], cmdsplit[0]);
		if (access(tabl[i], X_OK) == 0)
			return (1);
	}
	ft_putendl_fd(ft_strjoin("jush: command not found: ",
	ft_strdup(cmd)), 2);
	return (0);
}

void		ft_check_parse(t_term *term)
{
	t_parse *tmp;

	tmp = term->parselst;
	while (term->parselst)
	{
		if (!ft_check_builtin(term) &&
		!ft_check_exec(term, term->parselst->cmd))
		{
			term->fail = 1;
			return ;
		}
		term->parselst = term->parselst->next;
	}
	term->parselst = tmp;
}
