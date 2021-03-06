/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 21:03:16 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/06 21:03:17 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_copy_redirections(t_term *term, t_parse *parse)
{
	char	*line;
	char	*tmp;
	int		fd;
	int		fd2;

	tmp = ft_strjoin_nf(ft_strjoin(ft_get_env_by_name(term, "HOME"), "/"),
	".21shtmp", 1);
	fd2 = open(tmp, O_RDONLY);
	if (parse->sgred)
		fd = open(parse->sgred, O_WRONLY, S_IRUSR);
	if (parse->dbred)
		fd = open(parse->dbred, O_WRONLY, S_IRUSR);
	while ((get_next_line(fd2, &line)) > 0)
		ft_putendl_fd(line, fd);
	close(fd);
	close(fd2);
	ft_strdel(&tmp);
}

void		ft_write_in_tmp(t_term *term)
{
	int		child;

	if ((!ft_check_builtin(term)) && ft_check_in_path(term))
	{
		child = fork();
		if (child == 0)
		{
			dup2(term->parselst->fd, STDOUT_FILENO);
			execve(term->path, term->cmds, term->env);
		}
		wait(0);
		close(term->parselst->fd);
	}
}

void		ft_create_redirections(t_parse *parse)
{
	if (parse->sgred)
		parse->fd = open(parse->sgred, O_WRONLY |
		O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	if (parse->dbred)
		parse->fd = open(parse->dbred, O_WRONLY | O_CREAT |
		O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
}

void		ft_create_heredoc2(t_term *term, char *str, int fd, int i)
{
	term->inheredoc = 1;
	term->cmdlength = ft_strlen(term->cmdactual) - 12;
	ft_go_end(term);
	(i == 0) ? ft_putstr_fd("heredoc-> ", 2) : ft_putstr_fd("\nheredoc-> ", 2);
	while (42)
	{
		ft_reset_term(term);
		while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
		{
			if (!term->inheredoc)
				break ;
			ft_process(term);
			if (!term->inheredoc)
				break ;
		}
		if (!term->inheredoc)
		{
			reset_shell();
			break ;
		}
		if (ft_strequ(term->cmdactual, str))
			break ;
		ft_putendl_fd(term->cmdactual, fd);
		ft_putstr_fd("\nheredoc-> ", 2);
	}
}

void		ft_create_heredoc(t_term *term)
{
	int		i;
	char	**tabl;
	char	*tmp;

	i = -1;
	tabl = ft_strsplit(term->parselst->heredoc, ';');
	while (tabl && tabl[++i])
	{
		tmp = ft_strjoin(ft_get_env_by_name(term, "HOME"), "/");
		tmp = ft_strjoin_nf(tmp, ".21shheredoctmp", 1);
		term->heredocfd = open(tmp, O_WRONLY | O_CREAT | O_APPEND | O_TRUNC,
		S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		ft_strdel(&tmp);
		ft_create_heredoc2(term, tabl[i], term->heredocfd, i);
		ft_strdel(&tabl[i]);
	}
	free(tabl);
	ft_putchar_fd('\n', 2);
	close(term->heredocfd);
	if (!term->parselst->file)
	{
		tmp = ft_strjoin(ft_get_env_by_name(term, "HOME"), "/");
		term->parselst->file = ft_strjoin_nf(tmp, ".21shheredoctmp", 1);
	}
	term->inheredoc = 0;
}
