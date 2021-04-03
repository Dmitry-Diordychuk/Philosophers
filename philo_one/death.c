/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 01:23:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t	get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	get_time(&cur_time);
	result = cur_time - philo->last_meal_time;
	return (result);
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
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
		if (g_data->is_done || g_data->done_counter == g_data->philos_num)
		{
			g_data->is_done = TRUE;
			pthread_join(philo->thread, NULL);
			return (NULL);
		}
	}
	return (NULL);
}
