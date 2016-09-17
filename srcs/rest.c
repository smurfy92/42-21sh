/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/08 12:46:12 by jtranchi          #+#    #+#             */
/*   Updated: 2016/04/08 12:46:13 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_env_i(t_term *term)
{
	int y;
	int i;

	y = -1;
	while (term->env[++y])
		term->env[y] = NULL;
	i = -1;
	while (term->cmds[++i])
	{
		if (ft_strchr(term->cmds[i], '='))
		{
			y = 0;
			while (term->env[y])
				y++;
			term->env[y] = ft_strdup(term->cmds[i]);
		}
	}
}

void		ft_get_cmd(t_term *term, char *str)
{
	char	*tmp;

	if (term->cursorpos < term->cmdlength)
	{
		tmp = ft_strjoin(str, (term->cmdactual + term->cursorpos));
		term->cmdactual[term->cursorpos] = '\0';
		term->cmdactual = ft_strjoin(term->cmdactual, tmp);
		term->cmdlength = ft_strlen(term->cmdactual);
		return ;
	}
	else if (!term->cmdactual)
		term->cmdactual = ft_strdup(str);
	else
		term->cmdactual = ft_strjoin(term->cmdactual, str);
	term->cursorpos += ft_strlen(str);
	term->cmdlength = ft_strlen(term->cmdactual);
	if (term->inhistory)
		term->history->var = term->cmdactual;
	ft_putstr_fd(str, 2);
}

int			init_shell(int lflag)
{
	char			*name;
	struct termios	term;
	t_term			*t;

	t = ft_get_term();
	if ((name = getenv("TERM")) == NULL)
		name = ft_strdup("xterm-256color");
	if (tgetent(NULL, name) == ERR)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	t->cpy_term = term;
	term.c_lflag = term.c_lflag & lflag;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, 0, &term))
		return (-1);
	return (1);
}

int			reset_shell(void)
{
	t_term			*t;

	t = ft_get_term();
	if (tcsetattr(0, 0, &(t->cpy_term)) == -1)
		return (-1);
	return (0);
}

void		ft_clean_line(t_term *term)
{
	ft_go_end(term);
	if (term->cmdlength == 0)
		return ;
	while (term->cmdlength-- > 0)
	{
		ft_left_arrow(term);
		tputs(tgetstr("dc", NULL), 0, ft_outchar);
		tputs(tgetstr("cd", NULL), 0, ft_outchar);
	}
	term->cmdlength = 0;
	term->cmdactual = NULL;
}
