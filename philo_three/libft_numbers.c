/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:46:05 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/27 14:46:55 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' ||
																	c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long int	num;
	int			minus;

	minus = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		minus = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	num = 0;
	while (*str != '\0' && ft_isdigit(*str))
	{
		num += *str - '0';
		num *= 10;
		str++;
	}
	return (num / 10 * minus);
}
