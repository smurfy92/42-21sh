/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 10:31:24 by jtranchi          #+#    #+#             */
/*   Updated: 2016/08/08 10:31:25 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void			ft_display_parse(t_parse *parse)
{
	ft_putstr("command :");
	ft_putstr(parse->cmd);
	ft_putendl("|");
	ft_putstr("sgred :");
	ft_putstr(parse->sgred);
	ft_putendl("|");
	ft_putstr("dbred :");
	ft_putstr(parse->dbred);
	ft_putendl("|");
	ft_putstr("heredoc :");
	ft_putstr(parse->heredoc);
	ft_putendl("|");
	ft_putstr("file :");
	ft_putstr(parse->file);
	ft_putendl("|");
}

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
	char 		**tabl;
	int			i;
	t_parse		*tmp;

	i = -1;
	tabl = ft_strsplit(cmd, '|');
	while (tabl[++i])
		ft_create_parse(term, tabl[i]);
	term->cmdlength = 0;
	cmd = NULL;
	tmp = term->parselst;

}

void		ft_adddoubleredirection(t_term *term, t_parse *parse, int i)
{
	int start;
	char *tmp;
	int end;

	tmp = NULL;
	if (!parse->cmd[i])
	{
		term->fail = 1;
		return (ft_putendl("jush : parse error near `\\n"));
	}
	start = i - 2;
	while (parse->cmd[i] && !ft_isalpha(parse->cmd[i]))
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl(ft_strjoin(ft_strjoin("jush : parse error near `", &parse->cmd[end - 1]),"'")));
	}
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	parse->dbred = ft_strdup(tmp);
	parse->sgred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

void		ft_addredirection(t_term *term, t_parse *parse, int i)
{
	int start;
	char *tmp;
	int end;

	tmp = NULL;
	if (!parse->cmd[i])
	{
		term->fail = 1;
		return (ft_putendl("jush : parse error near `\\n'"));
	}
	start = i - 1;
	while (parse->cmd[i] && !ft_isalpha(parse->cmd[i]))
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl(ft_strjoin(ft_strjoin("jush : parse error near `", &parse->cmd[end - 1]),"'")));
	}
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	parse->sgred = ft_strdup(tmp);
	parse->dbred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

void		ft_addheredoc(t_term *term, t_parse *parse, int i)
{
	int		start;
	int		end;
	char	*tmp;

	start = i - 2;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i])
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl("jush : parse error near `\\n'"));
	}
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	if (!parse->heredoc)
		parse->heredoc = ft_strdup(tmp);
	else
		parse->heredoc = ft_strjoin(ft_strjoin(parse->heredoc, ";"), tmp);
	parse->sgred = NULL;
	parse->dbred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

void		ft_addfile(t_term *term, t_parse *parse, int i)
{
	int		start;
	int		end;
	char	*tmp;

	start = i - 1;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i])
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl("jush : parse error near `\\n'"));
	}
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	parse->file = ft_strdup(tmp);
	parse->sgred = NULL;
	parse->dbred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

t_parse		*ft_parse_redirections(t_term *term, t_parse *parse)
{
	int i;

	i = 0;
	while (parse->cmd[i])
	{
		if (parse->cmd[i] == '>' && parse->cmd[i + 1] && parse->cmd[i + 1] == '>')
			ft_adddoubleredirection(term, parse, i + 2);
		else if (parse->cmd[i] == '>')
			ft_addredirection(term, parse , i + 1);
		else if (parse->cmd[i] == '<' && parse->cmd[i + 1] && parse->cmd[i + 1] == '<')
			ft_addheredoc(term, parse, i + 2);
		else if (parse->cmd[i] == '<')
			ft_addfile(term, parse , i + 1);
		else
			i++;
		if (term->fail)
		{
			break ;
		}
		if (parse->sgred || parse->dbred)
			ft_create_redirections(parse);
		// else if (parse->cmd[i] == '<' &&
		// parse->cmd[i + 1] && parse->cmd[i + 1] == '<')

		// else if (parse->cmd[i] == '<')

		// else if (parse->cmd[i] == '>')
	}
	return (parse);
}

void		ft_create_parse(t_term *term, char *cmd)
{
	t_parse		*tmp;
	t_parse		*tmp2;

	tmp = (t_parse*)malloc(sizeof(t_parse));
	while (ft_is_space(cmd[0]) && cmd[1])
		cmd = &cmd[1];
	tmp->cmd = ft_strdup(cmd);
	tmp->next = NULL;
	tmp->dbred = NULL;
	tmp->sgred = NULL;
	tmp = ft_parse_redirections(term, tmp);
	if (!term->parselst)
		term->parselst = tmp;
	else
	{
		tmp2 = term->parselst;

		while (tmp2 && tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}
