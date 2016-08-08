/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 10:19:12 by jtranchi          #+#    #+#             */
/*   Updated: 2016/08/08 10:19:13 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vingtetun.h"

void		ft_out(char *str)
{
	int fd;

	fd = open("/dev/ttys001", O_WRONLY | O_NONBLOCK | O_NOCTTY);
	write(fd, str, ft_strlen(str));
}
