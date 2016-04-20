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

#include "../includes/21.h"

void		ft_permission_denied(char *str)
{
	char *tmp;

	tmp = "21sh: permission denied: ";
	write(2, tmp, ft_strlen(tmp));
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
}

void		ft_env_options(t_term **term)
{
	int i;

	i = 0;
	while ((*term)->cmds[++i] && !ft_check_in_env(*term, (*term)->cmds[i]))
	{
		(ft_strcmp((*term)->cmds[i], "-i") == 0 ||\
		ft_strcmp((*term)->cmds[i], "-") == 0) ? ((*term)->i = 1) : 0;
		if (ft_strcmp((*term)->cmds[i], "-u") == 0)
		{
			if (!(*term)->cmds[++i])
			{
				ft_putendl("env: option requires an argument -u");
				return ;
			}
		}
		else if (!ft_strchr((*term)->cmds[i], '=') &&\
		ft_strcmp((*term)->cmds[i], "-i") != 0)
		{
			ft_putstr(ft_strjoin("21sh: ", (*term)->cmds[i]));
			ft_putendl(" No such file or directory");
			return ;
		}
	}
	if (!(*term)->cmds[i])
		(!(*term)->i) ? ft_newenv_display(term) : ft_display_i(*term);
}

void		ft_display_env(t_term *term)
{
	t_env		*lst;

	lst = term->lst;
	while (lst)
	{
		if (!term->u || !ft_strstr(term->u, lst->var))
		{
			ft_putstr(lst->var);
			ft_putchar('=');
			ft_putendl(lst->val);
		}
		lst = lst->next;
	}
}

t_env		*ft_create_lst(char *str)
{
	t_env		*lst;
	char		**tabl;

	lst = (t_env*)malloc(sizeof(t_env));
	tabl = ft_strsplit(str, '=');
	lst->var = tabl[0];
	if (ft_strcmp(lst->var, "SHLVL") == 0)
		lst->val = ft_itoa(ft_atoi(tabl[1]) + 1);
	else
		lst->val = tabl[1];
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
