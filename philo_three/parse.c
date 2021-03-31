/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:48:37 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/27 15:14:30 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

t_bool check_argc(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments!\n");
		printf("Arguments should be like this: ");
		printf("number_of_philosophers time_to_die time_to_eat ");
		printf("time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]:\n");
		return (FALSE);
	}
	return (TRUE);
}

void choose_option(int i, int number, s_data *data)
{
	if (i == 1)
		data->number_of_philosophers = number;
	else if (i == 2)
		data->time_to_die = number;
	else if (i == 3)
		data->time_to_eat = number;
	else if (i == 4)
		data->time_to_sleep = number;
	else if (i == 5)
		data->number_of_times_each_philosopher_must_eat = number;
}

t_bool try_get_numbers(int argc, char **argv, s_data *data)
{
	int i;
	int j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (!ft_isdigit(argv[i][j]))
			{
				printf ("All arguments must be numbers!\n");
				return (FALSE);
			}
			choose_option(i, ft_atoi(argv[i]), data);
			j++;
		}
		i++;
	}
	return (TRUE);
}

int parse(int argc, char **argv, s_data *data)
{
	if (!check_argc(argc))
		return (ERROR);
	if (!try_get_numbers(argc, argv, data))
		return (ERROR);
	return (0);
}
