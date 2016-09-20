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

void		ft_addredirectionsuite(t_parse *parse, int end, int start)
{
	char *tmp;

	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

void		ft_adddoubleredirection(t_term *term, t_parse *parse, int i)
{
	int		start;
	int		end;

	if (!parse->cmd[i])
	{
		term->fail = 1;
		return (ft_putendl_fd("jush : parse error near `\\n", 2));
	}
	start = i - 2;
	while (parse->cmd[i] && !ft_isalpha(parse->cmd[i]) &&
	!ft_isdigit(parse->cmd[i]) && parse->cmd[i] != '/')
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i && (term->fail = 1))
	{
		return (ft_putendl_fd(ft_strjoin(ft_strjoin("jush : parse error near `",
		&parse->cmd[end - 1]), "'"), 2));
	}
	parse->dbred = ft_strdup(ft_strsub(&parse->cmd[i], 0, end - i));
	parse->sgred = NULL;
	ft_addredirectionsuite(parse, end, start);
}

void		ft_addredirection(t_term *term, t_parse *parse, int i)
{
	int		start;
	int		end;

	if (!parse->cmd[i])
	{
		term->fail = 1;
		return (ft_putendl_fd("jush : parse error near `\\n'", 2));
	}
	start = i - 1;
	while (parse->cmd[i] && !ft_isalpha(parse->cmd[i]) &&
	!ft_isdigit(parse->cmd[i]) && parse->cmd[i] != '/')
		i++;
	end = i;
	while (parse->cmd[end] && !ft_end_of_red(parse->cmd[end]))
		end++;
	if (end == i && (term->fail = 1))
	{
		return (ft_putendl_fd(ft_strjoin(ft_strjoin("jush : parse error near `",
		&parse->cmd[end - 1]), "'"), 2));
	}
	parse->sgred = ft_strdup(ft_strsub(&parse->cmd[i], 0, end - i));
	parse->dbred = NULL;
	ft_addredirectionsuite(parse, end, start);
}

void		ft_addheredoc2(t_parse *parse, int end, int start)
{
	char	*tmp;

	parse->sgred = NULL;
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
		return (ft_putendl_fd("jush : parse error near `\\n'", 2));
	}
	tmp = ft_strsub(&parse->cmd[i], 0, end - i);
	if (!parse->heredoc)
		parse->heredoc = ft_strdup(tmp);
	else
		parse->heredoc = ft_strjoin(ft_strjoin(parse->heredoc, ";"), tmp);
	ft_addheredoc2(parse, end, start);
}
