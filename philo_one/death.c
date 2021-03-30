/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 19:44:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 22:25:40 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int get_done()
{
	t_bool is_done;

	pthread_mutex_lock(&g_data->mutex_done);
	is_done = g_data->is_done;
	pthread_mutex_unlock(&g_data->mutex_done);
	return (is_done);
}

void set_done(t_bool bool)
{
	pthread_mutex_lock(&g_data->mutex_done);
	g_data->is_done = bool;
	pthread_mutex_unlock(&g_data->mutex_done);
}

uint64_t get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	if (pthread_mutex_lock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	get_time(&cur_time);
	result = cur_time - philo->last_meal_time;
	if (pthread_mutex_unlock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	return (result);
}

void	*run_death_timer(void *args)
{
	t_philo		*philo;

	philo = (t_philo*)args;
	while (INFINITE_LOOP)
	{
		if (get_done())
		{
			pthread_detach(philo->thread);
			return (NULL);
		}
		if (get_meal_time(philo) > g_data->time_to_die)
		{
			mprint(philo->id, "dead");
			set_done(TRUE);
			pthread_detach(philo->thread);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
