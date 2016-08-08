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

void			ft_display_parse(t_term *term)
{
	t_parse *tmp;

	tmp = term->parselst;
	while (tmp)
	{
		ft_putstr("command :");
		ft_putendl(tmp->cmd);
		ft_putstr("sgred :");
		ft_putendl(tmp->sgred);
		ft_putstr("dbred :");
		ft_putendl(tmp->dbred);
		tmp = tmp->next;
	}
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
	ft_display_parse(term);

}

void		ft_adddoubleredirection(t_parse *parse, int i)
{
	int start;
	char *tmp;

	if (!parse->cmd[i])
		return (ft_putendl("zsh : parse error near `\\n'"));
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i + 1])
		i++;
	start = i;
	while (parse->cmd[i] && !ft_is_space(parse->cmd[i]))
		i++;
	tmp = ft_strdup(&parse->cmd[start]);
	tmp[i - start] = '\0';
	// en cas yen ai plusieurs
	if (!parse->dbred)
		parse->dbred = ft_strdup(tmp);
	else
		parse->dbred = ft_strjoin(ft_strjoin(parse->dbred, ";"), tmp);
	parse->cmd[start] = '\0';
	tmp = ft_strdup(&parse->cmd[i]);
	parse->cmd = ft_strjoin(parse->cmd, tmp);
	parse->cmd[start] = parse->cmd[i];
}

void		ft_addredirection(t_parse *parse, int i)
{
	int start;
	char *tmp;

	if (!parse->cmd[i])
		return (ft_putendl("zsh : parse error near `\\n'"));
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i + 1])
		i++;
	start = i;
	while (parse->cmd[i] && !ft_is_space(parse->cmd[i]))
		i++;
	tmp = ft_strdup(&parse->cmd[start]);
	tmp[i - start] = '\0';
	if (!parse->sgred)
		parse->sgred = ft_strdup(tmp);
	else
		parse->sgred = ft_strjoin(ft_strjoin(parse->sgred, ";"), tmp);
	parse->cmd[start] = parse->cmd[i];
}

t_parse		*ft_parse_redirections(t_parse *parse)
{
	int i;

	i = -1;
	while (parse->cmd[++i])
	{
		if (parse->cmd[i] == '>' && parse->cmd[i + 1] && parse->cmd[i + 1] == '>')
			ft_adddoubleredirection(parse, i + 2);
		else if (parse->cmd[i] == '>')
			ft_addredirection(parse , i + 1);
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
	tmp = ft_parse_redirections(tmp);
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
