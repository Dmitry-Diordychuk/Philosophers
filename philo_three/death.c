/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 14:16:32 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void		*run_counter(void *args)
{
	size_t		counter;

	counter = 0;
	while (counter < g_data->philos_num - 1)
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

	philo = (t_philo*)args;
	while (INFINITE_LOOP)
	{
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			set_done(TRUE);
			mprint(philo->id, "dead");
			exit(24);
		}
		if (usleep(1000) < 0)
			set_done(ERROR);
	}
	return (NULL);
}
