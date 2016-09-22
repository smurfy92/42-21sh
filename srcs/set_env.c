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
	int			i;

	i = -1;
	lst = NULL;
	while (env[++i])
		lst = ft_add_lst(ft_create_lst(env[i]), lst);
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
	if (!ft_get_val_exists(term, "SHLVL"))
		ft_process_setenv(term, "SHLVL", "1");
	if (!ft_get_val_exists(term, "PWD"))
		ft_process_setenv(term, "PWD", getwd(buf));
	if (!ft_get_val_exists(term, "OLDPWD"))
		ft_process_setenv(term, "OLDPWD", getwd(buf));
	if (!ft_get_val_exists(term, "PATH"))
		ft_process_setenv(term, "PATH",
"/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/opt/X11/bin:/usr/local/munki");
}

t_term		*ft_set_term(t_term *term, char **env, t_env *lst)
{
	term->buf = ft_strnew(BUFFSIZE);
	term->heredocfd = 0;
	term->test = 0;
	term->u = NULL;
	term->exec = 1;
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->copy = NULL;
	term->inhistory = 0;
	term->historycurrent = 0;
	term->i = 0;
	term->parenthese = 0;
	term->parselst = NULL;
	term->history = NULL;
	term->historylen = 0;
	term->historycurrent = 0;
	term->env = env;
	term->lst = lst;
	ft_check_env(term);
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
