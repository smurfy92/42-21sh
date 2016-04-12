/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/29 16:00:42 by jtranchi          #+#    #+#             */
/*   Updated: 2015/11/29 17:19:39 by jtranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strcat(char *restrict s1, const char *restrict s2)
{
	int i;

	i = ft_strlen(s1);
	while (*s2)
		s1[i++] = *(s2++);
	s1[i] = '\0';
	return (s1);
}
