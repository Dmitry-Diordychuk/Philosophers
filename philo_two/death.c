/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 01:39:33 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

size_t		get_done_counter(void)
{
	size_t counter;

	sem_wait(g_data->sem_done);
	counter = g_data->done_counter;
	sem_post(g_data->sem_done);
	return (counter);
}

void		*run_death_timer(void *args)
{
	t_philo		*philo;

	philo = (t_philo*)args;
	while (!go_sleep(1000))
	{
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			mprint(philo->id, "dead");
			set_done(TRUE);
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
		if (get_done() || get_done_counter() == g_data->philos_num)
		{
			set_done(TRUE);
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
	}
	return (NULL);
}
