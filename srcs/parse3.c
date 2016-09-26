/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 16:01:59 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/08 16:02:00 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

int			ft_end_of_red(char c)
{
	if (c == '\t' || c == ' ' || c == '\v' || c == '\f' || c == '\r'
	|| c == '\n' || c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

void		ft_parse(t_term *term, char *cmd)
{
	char		**tabl;
	int			i;
	int			y;
	t_parse		*tmp;

	i = -1;
	tabl = ft_strsplit(cmd, '|');
	y = ft_count_pipes(cmd);
	while (tabl && tabl[++i])
	{
		ft_create_parse(term, tabl[i]);
		ft_strdel(&tabl[i]);
	}
	if (tabl)
		free(tabl);
	if (y != i)
	{
		term->fail = 1;
		return (ft_putendl("Invalid null command"));
	}
	term->cmdlength = 0;
	cmd = NULL;
	tmp = term->parselst;
	while (tmp)
	{
		i = ft_strlen(tmp->cmd);
		while (tmp->cmd && tmp->cmd[--i] == ' ')
			tmp->cmd[i] = '\0';
		tmp = tmp->next;
	}
}

int			ft_check_builtin(t_term *term)
{
	char	**tabl;
	int		i;
	int		y;

	i = 0;
	y = -1;
	tabl = ft_strsplit(term->parselst->cmd, ' ');
	if (ft_strcmp(tabl[0], "cd") == 0)
		i = 1;
	else if (ft_strcmp(tabl[0], "env") == 0)
		i = 1;
	else if (ft_strcmp(tabl[0], "setenv") == 0)
		i = 1;
	else if (ft_strcmp(tabl[0], "unsetenv") == 0)
		i = 1;
	else if (ft_strcmp(tabl[0], "exit") == 0)
		i = 1;
	while (tabl && tabl[++y])
		ft_strdel(&tabl[y]);
	if (tabl)
		free(tabl);
	return (i);
}

void		ft_create_file_dup(t_term *term)
{
	int		tabl[2];
	int		father;
	int		fd;
	char	*line;

	line = NULL;
	pipe(tabl);
	father = fork();
	if (father == 0)
	{
		dup2(tabl[1], STDOUT_FILENO);
		close(tabl[0]);
		fd = open(term->parselst->file, O_RDONLY);
		while ((get_next_line(fd, &line)) > 0)
			ft_putendl(line);
		close(fd);
		exit(0);
	}
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
}

void		ft_father_exec(t_term *term)
{
	if (term->parselst->sgred || term->parselst->dbred)
	{
		ft_write_in_tmp(term);
		ft_copy_redirections(term, term->parselst);
		if (!term->parselst->next)
		{
			ft_free_struct(term);
			exit(0);
		}
	}
	else
	{
		if (!term->parselst->file)
		{
			ft_create_process(term);
		}
	}
}
