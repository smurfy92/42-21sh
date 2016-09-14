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

void		ft_replace_vars(t_term *term, t_parse *parse, int i)
{
	int		start;
	char	*tmp;
	char	*tmp2;

	start = i;
	while (parse->cmd[i] && ft_isalpha(parse->cmd[i]))
		i++;
	tmp = ft_strsub(&parse->cmd[start], 0, i - start);
	if (!ft_get_val_exists(term, tmp))
		term->minus = 1;
	else
	{
		if (!parse->cmd[i + 1])
		{
			parse->cmd[start - 1] = '\0';
			parse->cmd = ft_strjoin(parse->cmd, ft_get_val(term, tmp));
		}
		else
		{
			tmp2 = ft_strdup(&parse->cmd[i]);
			parse->cmd[start - 1] = '\0';
			parse->cmd = ft_strjoin(ft_strjoin(parse->cmd,
			ft_get_val(term, tmp)), tmp2);
		}
	}
}

void		ft_parse_redirections2(t_term *term, t_parse *parse, int i)
{
	if (parse->cmd[i] == '>' && parse->cmd[i + 1] &&
	parse->cmd[i + 1] == '>')
		ft_adddoubleredirection(term, parse, i + 2);
	else if (parse->cmd[i] == '>')
		ft_addredirection(term, parse, i + 1);
	else if (parse->cmd[i] == '<' && parse->cmd[i + 1] &&
	parse->cmd[i + 1] == '<')
		ft_addheredoc(term, parse, i + 2);
	else if (parse->cmd[i] == '<')
		ft_addfile(term, parse, i + 1);
	else if (parse->cmd[i] == '1' && parse->cmd[i + 3] && parse->cmd[i + 1]
	== '>' && parse->cmd[i + 2] == '&' && parse->cmd[i + 3] == '-')
		ft_check_close1(parse, i);
	else if (parse->cmd[i] == '2' && parse->cmd[i + 3] && parse->cmd[i + 1]
	== '>' && parse->cmd[i + 2] == '&' && parse->cmd[i + 3] == '-')
		ft_check_close2(parse, i);
	else if (parse->cmd[i] == '$' && parse->cmd[i + 1])
		ft_replace_vars(term, parse, i + 1);
	else
		term->minus = 1;
}

t_parse		*ft_parse_redirections(t_term *term, t_parse *parse)
{
	int i;

	i = 0;
	while (parse->cmd[i])
	{
		term->minus = 0;
		ft_parse_redirections2(term, parse, i);
		if (term->fail)
			break ;
		if (parse->sgred || parse->dbred)
			ft_create_redirections(parse);
		if (term->minus)
			i++;
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
	tmp->heredoc = 0;
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
