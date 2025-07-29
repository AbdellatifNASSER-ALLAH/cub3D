/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:50:09 by abdnasse          #+#    #+#             */
/*   Updated: 2024/11/28 19:56:43 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*buffer;
	ssize_t		bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!cache)
	{
		cache = malloc(((size_t)BUFFER_SIZE + 1) * sizeof(char));
		if (!cache)
			return (NULL);
		ft_bzero(cache, (size_t)BUFFER_SIZE + 1);
	}
	while (1)
	{
		buffer = malloc(((size_t)BUFFER_SIZE + 1) * sizeof(char));
		if (!buffer)
			return (f_free(&cache));
		bytes = read(fd, buffer, (size_t)BUFFER_SIZE);
		if (f_buffer_to_cache(buffer, &cache, bytes) == 1)
			break ;
		if (bytes < 0)
			return (NULL);
	}
	return (f_set_line(&cache));
}

char	*f_set_line(char **cache)
{
	char	*line;
	char	*next_cache;
	size_t	index;

	index = f_newline(*cache);
	if (index > 0)
	{
		next_cache = ft_strdup(*cache + index);
		if (!next_cache)
			return (f_free(cache));
		line = f_realloc(*cache, index);
		if (!line)
			return (NULL);
		*cache = next_cache;
		return (line);
	}
	if (**cache)
	{
		line = ft_strdup(*cache);
		if (!line)
			return (NULL);
		f_free(cache);
		return (line);
	}
	return (f_free(cache));
}

int	f_buffer_to_cache(char *buffer, char **cache, ssize_t bytes)
{
	if (bytes > 0)
	{
		buffer[bytes] = '\0';
		*cache = f_realloc(*cache, ft_strlen(*cache) + bytes);
		if (!(*cache))
			return (0);
		ft_strcat(*cache, buffer);
		f_free(&buffer);
		if (f_newline(*cache))
			return (1);
		return (-1);
	}
	f_free(&buffer);
	if (bytes == 0)
		return (1);
	f_free(cache);
	return (0);
}

char	*ft_strdup(const char *s)
{
	char	*p;
	size_t	len;

	len = ft_strlen(s);
	p = (char *)malloc((len + 1) * sizeof(char));
	if (!p)
		return (0);
	*(p + len) = 0;
	while (len--)
		*(p + len) = *(s + len);
	return (p);
}

void	*f_free(char **p)
{
	free(*p);
	*p = NULL;
	return (NULL);
}
