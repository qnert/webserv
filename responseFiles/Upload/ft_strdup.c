/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: altaleb <altaleb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 12:56:36 by altaleb           #+#    #+#             */
/*   Updated: 2024/02/04 12:56:39 by altaleb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(char *src)
{
	int		len;
	char	*dest;
	int		i;

	len = 0;
	while (src[len])
	{
		len++;
	}
	dest = (char *)malloc(sizeof(char) * len);
	if (!dest)
	{
		return (NULL);
	}
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

