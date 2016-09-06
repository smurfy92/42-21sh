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

void		ft_check_close1(t_parse *parse, int i)
{
	char *tmp;
	if (parse->cmd[i + 4])
	{
		tmp = ft_strdup(&parse->cmd[i + 4]);
		parse->cmd[i] = '\0';
		parse->cmd = ft_strjoin(parse->cmd, tmp);
	}
	else
		parse->cmd[i] = '\0';
	parse->close1 = 1;
}

void		ft_check_close2(t_parse *parse, int i)
{
	char *tmp;
	if (parse->cmd[i + 4])
	{
		tmp = ft_strdup(&parse->cmd[i + 4]);
		parse->cmd[i] = '\0';
		parse->cmd = ft_strjoin(parse->cmd, tmp);
	}
	else
		parse->cmd[i] = '\0';
	parse->close2 = 1;
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
		else if (parse->cmd[i] == '1' && parse->cmd[i + 3] && parse->cmd[i + 1] == '>' && parse->cmd[i + 2] == '&' && parse->cmd[i + 3] == '-')
			ft_check_close1(parse, i);
		else if (parse->cmd[i] == '2' && parse->cmd[i + 3] && parse->cmd[i + 1] == '>' && parse->cmd[i + 2] == '&' && parse->cmd[i + 3] == '-')
			ft_check_close2(parse, i);
		else
			i++;
		if (term->fail)
			break ;
		if (parse->sgred || parse->dbred)
			ft_create_redirections(parse);
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
	tmp->close1 = 0;
	tmp->close2 = 0;
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
