/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:46:05 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 17:44:58 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int				ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

uint64_t		ft_atoi(const char *str)
{
	uint64_t	num;

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

int				get_time(uint64_t *result)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) < 0)
		return (TIME_ERROR);
	*result = (time.tv_sec * (uint64_t)1000) + (time.tv_usec / (uint64_t)1000);
	return (0);
}

int				go_sleep(useconds_t usec)
{
	struct timeval	start;
	struct timeval	next;

	if (gettimeofday(&start, NULL) < 0)
		return (TIME_ERROR);
	if (gettimeofday(&next, NULL) < 0)
		return (TIME_ERROR);
	while ((next.tv_sec - start.tv_sec) * (uint64_t)1000000 +
										(next.tv_usec - start.tv_usec) < usec)
	{
		usleep(1);
		if (gettimeofday(&next, NULL) < 0)
			return (TIME_ERROR);
	}
	return (0);
}
