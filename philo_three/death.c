/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 17:25:16 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void		*run_counter(void *args)
{
	size_t		counter;
	t_data		*data;

	data = (t_data*)args;
	counter = 0;
	while (counter < data->philos_num - 1)
	{
		sem_wait(&data->sem_ration);
		counter++;
	}
	set_done(TRUE, &data->sem_done);
	return (args);
}

void		*run_death_timer(void *args)
{
	t_philo		*philo;

	philo = (t_philo*)args;
	while (INFINITE_LOOP)
	{
		if (get_meal_time(philo) > philo->time_to_die)
		{
			set_done(TRUE, &philo->sem_done);
			mprint(philo->id, "dead", philo->start_time, &philo->sem_print);
			sem_close(&philo->sem_meal);
			sem_close(&philo->sem_print);
			sem_close(&philo->sem_done);
			exit(0);
		}
		if (usleep(1000) < 0)
			set_done(ERROR, &philo->sem_done);
	}
	return (NULL);
}
