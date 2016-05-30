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
	}
	else if (!term->cmdactual)
		term->cmdactual = ft_strdup(str);
	else
		term->cmdactual = ft_strjoin(term->cmdactual, str);
	term->cursorpos += ft_strlen(str);
	term->cmdlength = ft_strlen(term->cmdactual);
	ft_putstr(str);
}

int			init_shell(int lflag)
{
	char			*name;
	struct termios	term;

	if ((name = getenv("TERM")) == NULL)
		name = "xterm-256color";
	if (tgetent(NULL, name) == ERR)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag = term.c_lflag & lflag;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, 0, &term))
		return (-1);
	return (1);
}

int			reset_shell(void)
{
	struct termios term;

	tputs(tgetstr("me", NULL), 1, ft_outchar);
	tputs(tgetstr("ve", NULL), 1, ft_outchar);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag = (ICANON | ECHO);
	if (tcsetattr(0, 0, &term) == -1)
		return (-1);
	return (0);
}

void		ft_clean_line(t_term *term)
{
	int i;
	int tmp;
	int y;

	i = -1;
	tmp = term->cursorpos;
	ft_go_end(term);
	while (term->cursorpos-- > 0)
	{
		if (((term->cursorpos + 3 + (int)ft_strlen(&term->cmdactual
		[term->cursorpos]) - i) % term->window->width) == 0)
		{
			tputs(tgetstr("up", NULL), 1, ft_outchar);
			y = 0;
			while (y++ < term->window->width)
				tputs(tgetstr("nd", NULL), 0, ft_outchar);
		}
		else
		{
			tputs(tgetstr("le", NULL), 0, ft_outchar);
			tputs(tgetstr("dc", NULL), 0, ft_outchar);
		}
	}
	term->cursorpos = tmp;
}
