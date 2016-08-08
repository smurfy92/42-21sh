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

int			ft_is_space(char c)
{
	if (c == '\t' || c == ' ' || c == '\v' || c == '\f' || c == '\r'
	|| c == '\n')
		return (1);
	else
		return (0);
}

void		ft_write_in_tmp(char *cmd)
{
	int		fd;
	char	**args;
	int		child;

	args = ft_strsplit(cmd, ' ');
	fd = open("/tmp/.21shtmp", O_WRONLY | O_APPEND | O_CREAT,
	S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	child = fork();
	if (child == 0)
	{
		dup2(fd, STDOUT_FILENO);
		execve("/bin/ls", args, NULL);
	}
	close(fd);
}

int			main(int argc, char **argv, char **env)
{
	t_term		*term;

	argv = NULL;
	term = ft_get_term();
	term = ft_set_term(env, ft_parse_env(env));
	ft_check_env(term);
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
