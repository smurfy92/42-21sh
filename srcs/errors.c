/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/29 10:43:58 by jtranchi          #+#    #+#             */
/*   Updated: 2016/03/29 10:44:00 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_permission_denied(char *str)
{
	write(2, "jush: permission denied: ",
	ft_strlen("jush: permission denied: "));
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
}

void		ft_env_options(t_term *term)
{
	int i;

	i = 0;
	while (term->cmds[++i] && !ft_check_in_env(term, term->cmds[i]))
	{
		(ft_strcmp(term->cmds[i], "-i") == 0 ||\
		ft_strcmp(term->cmds[i], "-") == 0) ? (term->i = 1) : 0;
		if (ft_strcmp(term->cmds[i], "-u") == 0)
		{
			if (!term->cmds[++i])
			{
				ft_putendl("env: option requires an argument -u");
				return ;
			}
		}
		else if (!ft_strchr(term->cmds[i], '=') &&\
		ft_strcmp(term->cmds[i], "-i") != 0)
		{
			ft_putstr(ft_strjoin("jush: ", term->cmds[i]));
			ft_putendl(" No such file or directory");
			return ;
		}
	}
	if (!term->cmds[i])
		(!term->i) ? ft_newenv_display(term) : ft_display_i(term);
}

void		ft_display_env(t_term *term)
{
	t_env		*lst;
	int			fd;

	lst = term->lst;
	if (term->parselst->sgred || term->parselst->dbred)
		fd = term->parselst->fd;
	else
		fd = 1;
	while (lst)
	{
		if (!term->u || !ft_strstr(term->u, lst->var))
		{
			ft_putstr_fd(lst->var, fd);
			ft_putchar_fd('=', fd);
			ft_putendl_fd(lst->val, fd);
		}
		lst = lst->next;
	}
}

t_env		*ft_create_lst(char *str)
{
	t_env		*lst;
	char		**tabl;
	char		*tmp;

	lst = (t_env*)malloc(sizeof(t_env));
	tabl = ft_strsplit(str, '=');
	lst->var = ft_strdup(tabl[0]);
	if (ft_strcmp(lst->var, "SHLVL") == 0)
	{
		tmp = ft_itoa(ft_atoi(tabl[1]) + 1);
		lst->val = ft_strdup(tmp);
		free(tmp);
	}
	else
		lst->val = ft_strdup(tabl[1]);
	free(tabl[0]);
	free(tabl[1]);
	free(tabl);
	lst->next = NULL;
	return (lst);
}

t_env		*ft_add_lst(t_env *tmp, t_env *lst)
{
	t_env		*tmp2;

	if (!lst)
		return (tmp);
	tmp2 = lst;
	while (lst->next)
		lst = lst->next;
	lst->next = tmp;
	return (tmp2);
}
