/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:48:37 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 20:25:19 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

t_bool	check_argc(int argc)
{
	if (argc != 5 && argc != 6)
		return (FALSE);
	return (TRUE);
}

void	choose_option(int i, uint64_t number, t_data **ret_data)
{
	if (i == 1)
		(*ret_data)->philos_num = (size_t)number;
	else if (i == 2)
		(*ret_data)->time_to_die = number;
	else if (i == 3)
		(*ret_data)->time_to_eat = (unsigned long)number;
	else if (i == 4)
		(*ret_data)->time_to_sleep = (unsigned long)number;
	else if (i == 5)
	{
		(*ret_data)->max_eat = number;
		(*ret_data)->last_argument = TRUE;
	}
}

t_bool	check_is_number(char **argv, int i)
{
	int64_t	j;

	j = 0;
	while (argv[i][j] == '\t' || argv[i][j] == '\n' || argv[i][j] == '\v' ||
	argv[i][j] == '\f' || argv[i][j] == '\r' || argv[i][j] == ' ')
		j++;
	if (argv[i][j] == '\0')
		return (FALSE);
	if (argv[i][j] == '+')
		j++;
	if (argv[i][j] == '\0')
		return (FALSE);
	while (argv[i][j] != '\0')
	{
		if (!ft_isdigit(argv[i][j]))
			return (FALSE);
		j++;
	}
	return (TRUE);
}

t_bool	try_get_numbers(int argc, char **argv, t_data **ret_data)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (check_is_number(argv, i))
			choose_option(i, ft_atoi(argv[i]), ret_data);
		else
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int		parse(int argc, char **argv, t_data **ret_data)
{
	(*ret_data)->last_argument = FALSE;
	(*ret_data)->done_counter = 0;
	(*ret_data)->is_done = FALSE;
	if (!check_argc(argc))
	{
		if (printf("Wrong number of arguments!\n") < 0 ||
		printf("Arguments should be like this: ") < 0 ||
		printf("number_of_philosophers time_to_die time_to_eat ") < 0 ||
		printf("time_to_sleep ") < 0 ||
		printf("[number_of_times_each_philosopher_must_eat]:\n") < 0)
			return (PRINTF_ERROR);
		return (PARSE_ERROR);
	}
	if (!try_get_numbers(argc, argv, ret_data))
	{
		if (printf("All arguments must be positive numbers!\n") < 0)
			return (PRINTF_ERROR);
		return (PARSE_ERROR);
	}
	return (0);
}
