/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 16:43:44 by jtranchi          #+#    #+#             */
/*   Updated: 2016/09/08 16:43:59 by jtranchi         ###   ########.fr       */
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
	tmp = ft_strsub(&parse->cmd[i], 0, end - i);
	parse->file = ft_strdup(tmp);
	parse->sgred = NULL;
	parse->dbred = NULL;
	while (ft_is_space(parse->cmd[end]) && parse->cmd[end])
		end++;
	tmp = ft_strdup(&parse->cmd[end]);
	parse->cmd[start] = '\0';
	parse->cmd = ft_strjoin(parse->cmd, tmp);
}

void		ft_ctrl_c(int sig)
{
	t_term			*term;

	sig = 0;
	term = ft_get_term();
	ft_putchar_fd('\n', 2);
	ft_reset_term(term);
	term->test = 1;
}

void		ft_handle_signal(void)
{
	signal(SIGTSTP, ft_ctrl_c);
	signal(SIGINT, ft_ctrl_c);
	signal(SIGQUIT, ft_ctrl_c);
	signal(SIGWINCH, ft_get_window_sig);
}

void		ft_check_u(t_term *term, int i)
{
	int y;

	y = 0;
	while (ft_strcmp(term->cmds[i + 1], \
	ft_strsplit(term->env[y], '=')[0]) != 0)
		y++;
	while (term->env[++y])
	{
		term->env[y - 1] = ft_strdup(term->env[y]);
		(ft_strcmp(term->env[y - 1], term->env[y]) == 0) ?\
		(term->env[y] = NULL) : 0;
	}
}
