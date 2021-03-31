/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 19:34:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

size_t		get_done_counter(void)
{
	size_t counter;

	if (pthread_mutex_lock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	counter = g_data->done_counter;
	if (pthread_mutex_unlock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	return (counter);
}

void		*run_death_timer(void *args)
{
	t_philo		*philo;

	philo = (t_philo*)args;
	while (INFINITE_LOOP)
	{
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			mprint(philo->id, "dead");
			set_done(TRUE);
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
		if (usleep(1000) < 0)
			set_done(ERROR);
		if (get_done() || get_done_counter() == g_data->philos_num)
		{
			set_done(TRUE);
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
	}
	return (NULL);
}
