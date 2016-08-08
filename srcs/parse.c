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

void		ft_parse(t_term *term, char *cmd)
{
	char 	**tabl;
	int		i;

	i = -1;
	tabl = ft_strsplit(cmd, '|');
	while (tabl[++i])
		ft_create_parse(term, tabl[i]);
	term->cmdlength = 0;
	cmd = NULL;
	while (term->parselst)
	{
		ft_putendl(term->parselst->cmd);
		term->parselst = term->parselst->next;
	}
	ft_write_in_tmp("ls -l /");
}

void		ft_adddoubleredirection(t_parse *parse, int i)
{
	int y;

	if (!parse->cmd[i])
		return (ft_putendl("zsh : parse error near `\\n'"));
	y = i;
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i + 1])
		i++;
}

t_parse		*ft_parse_redirections(t_parse *parse)
{
	int i;

	i = -1;
	while (parse->cmd[++i])
	{
		if (parse->cmd[i] == '>' && parse->cmd[i + 1]
		&& parse->cmd[i + 1] == '>')
			ft_adddoubleredirection(parse, i + 2);
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
	tmp->fdin = 0;
	tmp->next = NULL;
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
