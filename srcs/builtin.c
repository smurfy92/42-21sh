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

void		ft_print_buf(t_term *term, char *str)
{
	int i;

	ft_get_cmd(term, str);
	if (term->cursorpos < term->cmdlength)
	{
		i = term->cursorpos - 1;
		while (term->cmdactual[++i])
			ft_putchar(term->cmdactual[i]);
		if (((i + 3) % term->window->width) == 0)
			tputs(tgetstr("do", NULL), 0, ft_outchar);
		ft_replace_cursor(term);
	}
}
