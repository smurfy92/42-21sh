/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 10:20:55 by jtranchi          #+#    #+#             */
/*   Updated: 2016/08/08 10:24:55 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_get_window(t_term *term)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	if (!term->window)
		term->window = (t_window*)malloc(sizeof(t_window));
	term->window->width = w.ws_col;
	term->window->heigth = w.ws_row;
}

t_term		*ft_get_term(void)
{
	static t_term *term;

	if (!term)
		term = (t_term*)ft_memalloc(sizeof(t_term));
	ft_get_window(term);
	return (term);
}

void		ft_get_window_sig(void)
{
	t_term			*term;

	term = ft_get_term();
	ft_get_window(term);
}

void		ft_reset_term(t_term *term)
{
	int		fd;
	char	*tmp;

	tmp = ft_strjoin(ft_get_env_by_name(term, "HOME"), "/");
	tmp = ft_strjoin_nf(tmp, ".21shtmp", 1);
	fd = open(tmp,
	O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP |
	S_IWGRP | S_IWUSR);
	free(tmp);
	close(fd);
	init_shell();
	term->cursorpos = 0;
	term->cmdlength = 0;
	ft_strdel(&(term->path));
	term->cmdactual = NULL;
	ft_bzero(term->cmdactual, ft_strlen(term->cmdactual));
	ft_bzero(term->buf, ft_strlen(term->buf));
	ft_get_window(term);
	if (term->inheredoc || term->cmdtmp)
		return ;
	if (term->exec)
		write(2, "\033[92mjush-> \033[0m", 17);
	else
		write(2, "\033[91mjush-> \033[0m", 17);
}
