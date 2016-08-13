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
	ft_putendl(parse->cmd);
	ft_putstr("sgred :");
	ft_putstr(parse->sgred);
	ft_putendl("|");
	ft_putstr("dbred :");
	ft_putstr(parse->dbred);
	ft_putendl("|");
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
		return (ft_putendl("zsh : parse error near `\\n'"));
	}
	start = i - 2;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i + 1])
		i++;
	end = i;
	while (parse->cmd[end] && ft_isalpha(parse->cmd[end]))
		end++;
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	if (!parse->dbred)
		parse->dbred = ft_strdup(tmp);
	else
		parse->dbred = ft_strjoin(ft_strjoin(parse->dbred, ";"), tmp);
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
		return (ft_putendl("zsh : parse error near `\\n'"));
	}
	start = i - 1;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i + 1])
		i++;
	end = i;
	while (parse->cmd[end] && ft_isalpha(parse->cmd[end]))
		end++;
	if (end == i)
	{
		term->fail = 1;
		return (ft_putendl("zsh : parse error near `\\n'"));
	}
	tmp = ft_strsub(&parse->cmd[i], 0 , end - i);
	if (!parse->sgred)
		parse->sgred = ft_strdup(tmp);
	else
		parse->sgred = ft_strjoin(ft_strjoin(parse->sgred, ";"), tmp);
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
		// ft_putstr("la commande est ->  ");
		// ft_putendl(parse->cmd);
		if (parse->cmd[i] == '>' && parse->cmd[i + 1] && parse->cmd[i + 1] == '>')
			ft_adddoubleredirection(term, parse, i + 2);
		else if (parse->cmd[i] == '>')
			ft_addredirection(term, parse , i + 1);
		else
			i++;
		if (term->fail)
		{
			term->fail = 0;
			break ;
		}

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
		while (tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}
