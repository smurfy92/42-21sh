/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 11:20:05 by jtranchi          #+#    #+#             */
/*   Updated: 2016/03/03 11:20:06 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

t_env		*ft_parse_env(char **env)
{
	t_env		*lst;

	lst = NULL;
	while (*env)
	{
		lst = ft_add_lst(ft_create_lst(*env), lst);
		env++;
	}
	return (lst);
}

int			ft_get_val_exists(t_term *term, char *str)
{
	t_env		*lst;

	lst = term->lst;
	if (!lst)
		return (0);
	while (lst->next && ft_strcmp(lst->var, str))
		lst = lst->next;
	if (lst && ft_strcmp(lst->var, str) == 0)
		return (1);
	else
		return (0);
}

void		ft_check_env(t_term *term)
{
	char *buf;

	buf = NULL;
	if (!ft_get_val_exists(term, "HOME"))
		ft_process_setenv(term, "HOME", "/nfs/2015/j/jtranchi");
	if (!ft_get_val_exists(term, "LOGNAME"))
		ft_process_setenv(term, "LOGNAME", "jtranchi");
	if (!ft_get_val_exists(term, "SHLVL"))
		ft_process_setenv(term, "SHLVL", "2");
	if (!ft_get_val_exists(term, "PWD"))
		ft_process_setenv(term, "PWD", getwd(buf));
	if (!ft_get_val_exists(term, "OLDPWD"))
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	if (!ft_get_val_exists(term, "PATH"))
		ft_process_setenv(term, "PATH",
		"/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/local/munki");
}

t_term		*ft_set_term(char **env, t_env *lst)
{
	t_term		*term;
	char		*line;

	term = (t_term*)malloc(sizeof(t_term));
	term->buf = (char*)malloc(sizeof(char) * BUFFSIZE);
	term->u = NULL;
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->historylen = 0;
	term->inhistory = 0;
	term->i = open(".21sh_history", O_RDONLY);
	while ((get_next_line(term->i, &line)) > 0)
		term->historylen++;
	term->historycurrent = 0;
	term->i = 0;
	term->parenthese = 0;
	term->env = env;
	term->lst = lst;
	return (term);
}

char		*ft_get_val(t_term *term, char *str)
{
	t_env		*lst;

	lst = term->lst;
	while (lst->next && ft_strcmp(lst->var, str))
		lst = lst->next;
	if (lst && ft_strcmp(lst->var, str) == 0)
		return (lst->val);
	else
		return (NULL);
}
