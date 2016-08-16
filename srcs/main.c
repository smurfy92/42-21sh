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

void		ft_copy_redirections(t_parse *parse)
{
	char	**tablsg;
	char	**tabldb;
	char	*line;
	int		fd;
	int		fd2;
	int		i;

	tablsg = ft_strsplit(parse->sgred, ';');
	tabldb = ft_strsplit(parse->dbred, ';');
	i = -1;
	while (tablsg[++i])
	{
		fd = open(tablsg[i], O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		fd2 = open("/tmp.21shtmp", O_WRONLY ,
		S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		while ((get_next_line(fd2, &line)) > 0)
			write(fd, line, ft_strlen(line));
	}


}

void		ft_write_in_tmp(t_term *term, char *cmd)
{
	int		fd;
	int		child;

	term->cmds = ft_strsplit(cmd, ' ');
	if (ft_check_in_path(term))
	{
		fd = open("/tmp/.21shtmp", O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		child = fork();
		if (child == 0)
		{
			dup2(fd, STDOUT_FILENO);
			execve(term->path, term->cmds, term->env);
		}
		close(fd);
	}
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
			while (term->parselst)
			{
				ft_display_parse(term->parselst);
				if (term->parselst->next)
				{
					ft_write_in_tmp(term, term->parselst->cmd);
					ft_copy_redirections(term->parselst);

				}
				term->parselst = term->parselst->next;
			}
			//term->cmds = ft_strsplit(term->cmdsplit[argc], ' ');
			//(!ft_check_builtin(term)) ? ft_check_in_path(term) : 0;
		}
	}
	return (0);
}
