/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:33:37 by ahakki            #+#    #+#             */
/*   Updated: 2024/10/24 13:33:39 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*arr;

	if (num > SIZE_MAX / size)
		arr = (malloc(0));
	else
		arr = malloc(num * size);
	if (arr == NULL)
		return (NULL);
	ft_bzero(arr, num * size);
	return (arr);
}
