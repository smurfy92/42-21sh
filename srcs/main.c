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

void		ft_create_parse(t_term *term, char *cmd, int redirection)
{
	t_parse		*tmp;
	t_parse		*tmp2;

	tmp = (t_parse*)malloc(sizeof(t_parse));
	while ((cmd[0] == '\t' || cmd[0] == ' ') && cmd[1])
		cmd = &cmd[1];
	tmp->cmd = ft_strdup(cmd);
	tmp->fdin = 0;
	tmp->fdout = 0;
	tmp->redirection = redirection;
	tmp->next = NULL;
	if (!term->parselst)
		term->parselst = tmp;
	else
	{
		tmp2 = term->parselst;
		while (tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}

void		ft_parse(t_term *term, char *cmd)
{
	int		i;
	char	*tmp;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '>' || cmd[i] == '<' || cmd[i] == '|')
		{
			if (!cmd[i + 1])
			{
				ft_putendl("21sh: parse error near `\\n'");
			}
			tmp = ft_strdup(cmd);
			tmp[i] = '\0';
			if (cmd[i] == '>' && cmd[i + 1] && cmd[i + 1] == '>')
			{
				ft_create_parse(term, tmp, 4);
				if (!cmd[i + 2])
					break ;
				cmd = &cmd[i + 2];
			}
			else if (cmd[i] == '<' && cmd[i + 1] && cmd[i + 1] == '<')
			{
				ft_create_parse(term, tmp, 5);
				if (!cmd[i + 2])
					break ;
				cmd = &cmd[i + 2];
			}
			else if (cmd[i] == '>')
			{
				ft_create_parse(term, tmp, 1);
				cmd = &cmd[i + 1];
			}
			else if (cmd[i] == '<')
			{
				ft_create_parse(term, tmp, 2);
				cmd = &cmd[i + 1];
			}
			else if (cmd[i] == '|')
			{
				ft_create_parse(term, tmp, 3);
				cmd = &cmd[i + 1];
			}
			i = -1;
		}
	}
	ft_create_parse(term, cmd, 0);
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

t_term		*ft_get_term(void)
{
	static t_term *term;

	if (!term)
		term = (t_term*)malloc(sizeof(t_term));
	ft_get_window(term);
	return (term);
}

void		ft_get_window_sig()
{
	struct winsize	w;
	t_term			*term;

	ioctl(0, TIOCGWINSZ, &w);
	term = ft_get_term();
	ft_clean_line(term);
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
	t_term		*term;

	argv = NULL;
	term = ft_get_term();
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(term);
	ft_get_history(term);
	signal(SIGWINCH, ft_get_window_sig);
	while (42)
	{
		argc = -1;
		ft_reset_term(term);
		ft_get_history(term);
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
			ft_parse(term, term->cmdsplit[argc]);
			term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			(!ft_check_builtin(term)) ? ft_check_in_path(term) : 0;
		}
	}
	return (0);
}
