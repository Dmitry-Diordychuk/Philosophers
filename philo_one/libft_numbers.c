/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:46:05 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 14:50:03 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int				ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

uint64_t		ft_atoi(const char *str)
{
	uint64_t	num;
	uint64_t	minus;

	minus = 1;
	while (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' ||
			*str == '\r' || *str == ' ')
		str++;
	if (*str == '+')
		str++;
	num = 0;
	while (*str != '\0' && ft_isdigit(*str))
	{
		num += *str - '0';
		if (*(str + 1) == '\0')
			break ;
		num *= 10;
		str++;
	}
	return (num);
}

uint64_t		convert_uint(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec * 0.001));
}

struct timeval	convert(uint64_t ms)
{
	struct timeval	result;

	result.tv_sec = ms / 1000;
	result.tv_usec = (ms % 1000) * 1000;
	return (result);
}

int				sub_time(struct timeval *z, struct timeval x, struct timeval y)
{
	int nsec;

	if (x.tv_usec < y.tv_usec)
	{
		nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
		y.tv_usec -= 1000000 * nsec;
		y.tv_sec += nsec;
	}
	if (x.tv_usec - y.tv_usec > 1000000)
	{
		nsec = (x.tv_usec - y.tv_usec) / 1000000;
		y.tv_usec += 1000000 * nsec;
		y.tv_sec -= nsec;
	}
	if (z)
	{
		z->tv_sec = x.tv_sec - y.tv_sec;
		z->tv_usec = x.tv_usec - y.tv_usec;
	}
	return (x.tv_sec < y.tv_sec);
}
