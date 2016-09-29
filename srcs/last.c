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
	while (!ft_isalpha(parse->cmd[i]) && parse->cmd[i] && parse->cmd[i] != '/')
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
	int				order[2];

	sig = 0;
	reset_shell();
	term = ft_get_term();
	order[0] = 3;
	order[1] = 0;
	term->test = 1;
	if (term->inheredoc)
	{
		term->inheredoc = 0;
		term->parselst->file = NULL;
		term->fail = 1;
		term->test = 0;
	}
	ioctl(0, TIOCSTI, "\n");
	ft_reset_term(term);
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

int			ft_is_printable(char *order)
{
	int	i;

	i = -1;
	while (order[++i] != '\0')
		if (ft_isprint(order[i]) == false)
			return (0);
	return (1);
}
