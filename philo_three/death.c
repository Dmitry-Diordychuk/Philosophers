/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 03:09:14 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

uint64_t	get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	get_time(&cur_time);
	result = cur_time - philo->last_meal_time;
	return (result);
}

void		*run_death_detector(void *args)
{
	sem_wait(g_data->sem_death);
	g_data->is_done = TRUE;
	return (args);
}

void		*run_counter(void *args)
{
	size_t		counter;

	counter = 0;
	while (counter != g_data->philos_num)
	{
		sem_wait(g_data->sem_ration);
		counter++;
	}
	g_data->is_done = TRUE;
	return (args);
}

void		*run_death_timer(void *args)
{
	t_philo		*philo;

	philo = (t_philo*)args;
	while (!usleep(1000))
	{
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			mprint(philo->id, "dead", 1);
			g_data->is_done = TRUE;
			sem_post(g_data->sem_death);
			delete_philos(g_data->philos, g_data->philos_num);
			sem_close(g_data->sem_forks);
			sem_close(g_data->sem_print);
			exit(0);
		}
	}
	return (NULL);
}
