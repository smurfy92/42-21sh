/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 17:39:35 by jtranchi          #+#    #+#             */
/*   Updated: 2016/02/15 17:39:36 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_parse(char *cmd)
{
	int i;

	i = -1;
	while (cmd[++i])
		if (cmd[i] == '>' || cmd[i] ==  '<')
			ft_putendl("redirection found");
}

void		ft_get_window(t_term *term)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	if (!term->window)
		term->window = (t_window*)malloc(sizeof(t_window));
	term->window->width = w.ws_col;
	term->window->heigth = w.ws_row;
}

void		ft_reset_term(t_term *term)
{
	init_shell((~ICANON & ~ECHO));
	term->cursorpos = 0;
	term->cmdlength = 0;
	term->cmdsplit = NULL;
	term->path = NULL;
	term->cmdactual = NULL;
	ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
	ft_bzero(term->buf, ft_strlen(term->buf));
	write(1, "$> ", 3);
	ft_get_window(term);
}

int			main(int argc, char **argv, char **env)
{
	static t_term		*term;

	argv = NULL;
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(term);
	ft_get_history(term);
	while (42)
	{
		argc = -1;
		ft_reset_term(term);
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
			ft_process(term);
		(ft_strlen(term->cmdactual) > 0) ?
		ft_add_history(term, term->cmdactual) : 0;
		reset_shell();
		ft_putchar('\n');
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			term->cmds = NULL;
			ft_parse(term->cmdsplit[argc]);
			term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			(!ft_check_builtin(term)) ? ft_check_in_path(term) : 0;
		}
	}
	return (0);
}
