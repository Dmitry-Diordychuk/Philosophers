/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 17:34:51 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void		*run_death_detector(void *args)
{
	sem_wait(g_data->sem_death);
	set_done(TRUE);
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
	set_done(TRUE);
	return (args);
}

void		*run_death_timer(void *args)
{
	t_philo		*philo;
	size_t		i;

	philo = (t_philo*)args;
	while (!go_sleep(1000))
	{
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			set_done(TRUE);
			mprint(philo->id, "dead");
			sem_post(g_data->sem_death);
			i = -1;
			while (++i < g_data->philos_num)
				sem_close(g_data->philos[i]->sem_meal);
			delete_philos(g_data->philos, g_data->philos_num);
			sem_close(g_data->sem_forks);
			sem_close(g_data->sem_print);
			exit(0);
		}
	}
	return (NULL);
}
