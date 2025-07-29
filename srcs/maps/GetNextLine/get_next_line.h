/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:45:38 by abdnasse          #+#    #+#             */
/*   Updated: 2024/11/25 16:47:36 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1337
# endif

char	*get_next_line(int fd);
char	*f_realloc(char *s, size_t n);
size_t	f_newline(char *p);
size_t	ft_strlen(const char *s);
size_t	ft_strcat(char *s1, char *s2);
int		f_buffer_to_cache(char *buffer, char **cache, ssize_t bytes);
char	*f_set_line(char **cache);
char	*ft_strdup(const char *s);
void	*f_free(char **p);
void	ft_bzero(void *b, size_t len);

#endif
