/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/21 15:18:25 by jtranchi          #+#    #+#             */
/*   Updated: 2016/05/21 15:18:27 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_process_unsetenv(t_term *term, char *s1)
{
	t_env *lst;

	lst = term->lst;
	if (!s1)
	{
		ft_putendl("21sh: unsetenv: Invalid option");
		ft_putendl("unsetenv NAME");
	}
	else
	{
		if (ft_strcmp(lst->var, s1) == 0)
			term->lst = lst->next;
		while (lst->next && ft_strcmp((lst->next)->var, s1) != 0)
			lst = lst->next;
		(lst->next) ? (lst->next = lst->next->next) : 0;
	}
}

void		ft_process_setenv(t_term *term, char *s1, char *s2)
{
	t_env *lst;

	lst = term->lst;
	if (!s1 || !s2)
	{
		ft_putendl("21sh: setenv: Invalid option");
		ft_putendl("setenv NAME VALUE");
	}
	else
	{
		lst = (t_env*)malloc(sizeof(t_env));
		lst->var = s1;
		lst->val = s2;
		lst->next = NULL;
		term->lst = ft_add_lst(lst, term->lst);
	}
}

void		ft_cd(t_term *term)
{
	char *buf;

	buf = NULL;
	if (ft_strcmp(term->cmds[1], "-") == 0)
	{
		chdir(ft_get_env_by_name(term, "OLDPWD"));
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", ft_get_env_by_name(term, "PWD"));
	}
	else
	{
		ft_process_unsetenv(term, "OLDPWD");
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	}
	if (term->cmds[1] && ft_strcmp(term->cmds[1], "~") != 0)
		chdir(term->cmds[1]);
	else
		chdir(ft_get_val(term, "HOME"));
	ft_process_unsetenv(term, "PWD");
	ft_process_setenv(term, "PWD", getwd(buf));
}

int			ft_check_builtin(t_term *term)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strcmp(term->cmds[0], "cd") == 0)
		ft_cd(term);
	else if (ft_strcmp(term->cmds[0], "env") == 0)
		(!term->cmds[1]) ? ft_display_env(term) : ft_env_options(term);
	else if (ft_strcmp(term->cmds[0], "setenv") == 0)
		ft_process_setenv(term, term->cmds[1], term->cmds[2]);
	else if (ft_strcmp(term->cmds[0], "unsetenv") == 0)
		ft_process_unsetenv(term, term->cmds[1]);
	else if (ft_strcmp(term->cmds[0], "exit") == 0)
	{
		reset_shell();
		exit(0);
	}
	else
		return (0);
	term->i = 0;
	term->u = NULL;
	return (1);
}
