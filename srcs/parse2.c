/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 21:32:33 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/06 21:32:34 by jtranchi         ###   ########.fr       */
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
	while (tmp)
	{
		i = ft_strlen(tmp->cmd);
		while (tmp->cmd[--i] == ' ')
			tmp->cmd[i] = '\0';
		tmp = tmp->next;
	}

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
