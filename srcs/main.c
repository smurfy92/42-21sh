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

void		ft_copy_redirections(t_term *term, t_parse *parse)
{
	char	*line;
	char 	*tmp;
	int		fd;
	int		fd2;

	tmp = ft_strjoin(ft_strjoin(ft_get_env_by_name(term, "HOME"), "/"),
	".21shtmp");
	fd2 = open(tmp, O_RDONLY);
	if (parse->sgred)
		fd = open(parse->sgred, O_WRONLY, S_IRUSR);
	if (parse->dbred)
		fd = open(parse->dbred, O_WRONLY, S_IRUSR);
	while ((get_next_line(fd2, &line)) > 0)
		ft_putendl_fd(line, fd);
}

void		ft_write_in_tmp(t_term *term, char *cmd)
{
	int		child;

	term->cmds = ft_strsplit(cmd, ' ');
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
		parse->fd = open(parse->sgred, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	if (parse->dbred)
		parse->fd = open(parse->dbred, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
}

void		ft_create_heredoc(t_term *term)
{
	int		i;
	char	**tabl;
	int		fd;

	i = -1;
	tabl = ft_strsplit(term->parselst->heredoc, ';');
	term->inheredoc = 1;
	while (tabl && tabl[++i])
	{
		fd = open(ft_strjoin(ft_strjoin(ft_get_env_by_name(term, "HOME"), "/"),".21shheredoctmp"), O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if (i == 0)
			ft_putstr("heredoc->");
		else
			ft_putstr("\nheredoc->");
		while (42)
		{
			ft_reset_term(term);
			while ((read(0, term->buf, BUFFSIZE)) && term->buf[0] != 10)
				ft_process(term);
			if (ft_strequ(term->cmdactual, tabl[i]))
				break ;
			ft_putendl_fd(term->cmdactual, fd);
			ft_putstr("\nheredoc->");
		}
	}
	ft_putchar('\n');
	close(fd);
	if (!term->parselst->file)
		term->parselst->file = ft_strjoin(ft_strjoin(ft_get_env_by_name(term, "HOME"), "/"),".21shheredoctmp");
	term->inheredoc = 0;
}

void		ft_create_file_dup(t_term *term)
{
	int 	tabl[2];
	int 	son;
	int 	fd;
	char	*line;

	line = NULL;
	pipe(tabl);
	son = fork();
	if (son == 0)
	{
		dup2(tabl[1], STDOUT_FILENO);
		close(tabl[0]);
		fd = open(term->parselst->file, O_RDONLY);
		while ((get_next_line(fd, &line)) > 0)
			ft_putendl(line);
		exit(0);
	}
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
}

void		ft_process_exec(t_term *term, char *cmdsplit)
{
	int father;

	term->cmds = NULL;
	term->parselst = NULL;
	term->fail = 0;
	ft_parse(term, cmdsplit);
	if (term->fail)
		return ;
	term->cmds = ft_strsplit(term->parselst->cmd, ' ');
	if (term->parselst->heredoc)
	{
		ft_create_heredoc(term);
	}
	if (ft_check_builtin(term))
		return ;
	if (ft_check_in_path(term))
		father = fork();
	else
		return ;
	if (father == 0)
	{
		while (term->parselst)
		{
			if (term->parselst->close1)
				close(1);
			if (term->parselst->close2)
				close(2);
			// ft_display_parse(term->parselst);
			term->cmds = ft_strsplit(term->parselst->cmd, ' ');
			if (!ft_check_builtin(term) && ft_check_in_path(term))
			{
				if (term->parselst->file)
					ft_create_file_dup(term);
				if (!term->parselst->next && !term->parselst->sgred && !term->parselst->dbred)
				{
					execve(term->path, term->cmds, term->env);
					break ;
				}
				if (term->parselst->sgred || term->parselst->dbred)
				{
					ft_write_in_tmp(term, term->parselst->cmd);
					ft_copy_redirections(term, term->parselst);
					if (!term->parselst->next)
						exit(0);
				}
				else
				{
					if (!term->parselst->file)
						ft_create_process(term);
				}
			}
			term->parselst = term->parselst->next;
		}
	}
	wait(0);
}

int			ft_is_separator(char c)
{
	if (c == 39 || c == '"' || c == '(' || c == ')' || c == '['
	|| c == ']' || c == '{' || c == '}')
		return (1);
	else
		return (0);
}

int 		ft_has_separator(t_term *term, char c)
{
	int i;

	i = -1;
	while (term->separators && term->separators[++i])
	{
		if (term->separators[i] == c)
			return (1);
	}
	return (0);
}

void		ft_del_last_separator_in_cmd(t_term *term, char c)
{
	int i;
	char *tmp;

	i = ft_strlen(term->cmdactual);
	while (term->cmdactual && term->cmdactual[--i])
	{
		if (term->cmdactual[i] == c)
		{
			if (!term->cmdactual[i + 1])
			{
				term->cmdactual[i] = '\0';
				return ;
			}
			tmp = &term->cmdactual[i + 1];
			term->cmdactual[i] = '\0';
			if (!term->cmdactual[0])
				term->cmdactual = ft_strdup(tmp);
			else
			{
				if (tmp)
					term->cmdactual = ft_strjoin(term->cmdactual, tmp);
			}
		}
	}
}

void		ft_del_last_seperator(char *str, char c)
{
	int i;
	char *tmp;

	i = ft_strlen(str);

	while (str && str[--i])
	{
		if (str[i] == c)
		{
			if (!str[i + 1])
			{
				str[i] = '\0';
				return ;
			}
			if (i == 0)
				str = (str + 1);
			tmp = &str[i + 1];
			str[i] = '\0';
			if (str[0] == '\0')
				str = ft_strdup(tmp);
			else
			{
				if (tmp)
					str = ft_strjoin(str, tmp);
			}
		}
	}
}

void		ft_del_first_seperator(char *str, char c)
{
	int i;
	char *tmp;

	i = -1;

	while (str && str[++i])
	{
		if (str[i] == c)
		{
			//ft_del_last_separator_in_cmd(term, c);
			if (!str[i + 1])
			{
				str[i] = '\0';
				return ;
			}
			tmp = &str[i + 1];
			str[i] = '\0';
			if (str[0] == '\0')
				str = ft_strdup(tmp);
			else
			{
				if (tmp)
					str = ft_strjoin(str, tmp);
			}
		}
	}
}

void		ft_check_last_separator_in_cmd(t_term *term, char c)
{
	if (c == '}')
	{
		ft_del_last_seperator(term->cmdactual, '}');
		ft_del_first_seperator(term->cmdactual, '{');
	}
	if (c == ']')
	{
		ft_del_last_seperator(term->cmdactual, ']');
		ft_del_first_seperator(term->cmdactual, '[');
	}
	if (c == ')')
	{
		ft_del_last_seperator(term->cmdactual, ')');
		ft_del_first_seperator(term->cmdactual, '(');
	}
	if (c == 39)
	{
		ft_del_last_seperator(term->cmdactual, 39);
		ft_del_last_seperator(term->cmdactual, 39);
	}
	if (c == '"')
	{
		ft_del_last_seperator(term->cmdactual, '"');
		ft_del_last_seperator(term->cmdactual, '"');
	}
}

void		ft_add_separator(t_term *term, char c)
{
	char *str;

	str = ft_strnew(1);
	str[0] = c;
	str[1] = '\0';
	ft_putendl("ici1");
	ft_putstr("char -> ");
	ft_putchar(c);
	ft_putendl(" ");
	ft_putstr("separatorsstart-> ");
	ft_putendl(term->separators);
	if (ft_has_separator(term,'(') && c == ')')
	{
		ft_del_last_seperator(term->separators, '(');
		return (ft_check_last_separator_in_cmd(term, c));
	}
	if (ft_has_separator(term,'[') && c == ']')
	{
		ft_del_last_seperator(term->separators, '[');
		return (ft_check_last_separator_in_cmd(term, c));
	}
	if (ft_has_separator(term,'{') && c == '{')
	{
		ft_del_last_seperator(term->separators, '{');
		return (ft_check_last_separator_in_cmd(term, c));
	}
	if (ft_has_separator(term, 39) && c == 39)
	{
		ft_del_last_seperator(term->separators, 39);
		return (ft_check_last_separator_in_cmd(term, c));
	}
	if (ft_has_separator(term,'"') && c == '"')
	{
		ft_del_last_seperator(term->separators, '"');
		return (ft_check_last_separator_in_cmd(term, c));
	}
	ft_putendl("ici2");
	ft_putstr("separatorsbefore-> ");
	ft_putendl(term->separators);
	(!term->separators) ? (term->separators = ft_strdup(str)) : (term->separators = ft_strjoin(term->separators, str));
	ft_putstr("separatorsafter-> ");
	ft_putendl(term->separators);
}

void		ft_check_separators(t_term *term)
{
	int i;

	i = -1;
	while (term->cmdactual && term->cmdactual[++i])
	{
		ft_putstr("term->cmdactual ->");
		ft_putendl(term->cmdactual);
		if (ft_is_separator(term->cmdactual[i]))
		{
			ft_add_separator(term, term->cmdactual[i]);
		}
	}
	ft_putstr("separators-> ");
	ft_putendl(term->separators);
}

int			main(int argc, char **argv, char **env)
{
	t_term		*term;

	argv = NULL;
	term = ft_set_term(ft_get_term(), env, ft_parse_env(env));
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
		ft_check_separators(term);
		term->cmdsplit = ft_strsplit(term->cmdactual, ';');
		while (term->cmdsplit && term->cmdsplit[++argc])
		{
			ft_process_exec(term, term->cmdsplit[argc]);
		}
	}
	return (0);
}
