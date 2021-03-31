/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 16:22:03 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 16:27:02 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t	get_meal_time(t_philo *philo)
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

int			set_meal(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	if (pthread_mutex_unlock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	return (0);
}

t_bool		get_done(void)
{
	t_bool is_done;

	pthread_mutex_lock(&g_data->mutex_done);
	is_done = g_data->is_done;
	pthread_mutex_unlock(&g_data->mutex_done);
	return (is_done);
}

void		set_done(t_bool bool)
{
	pthread_mutex_lock(&g_data->mutex_done);
	g_data->is_done = bool;
	pthread_mutex_unlock(&g_data->mutex_done);
}

t_bool		done_counter(void)
{
	pthread_mutex_lock(&g_data->mutex_done);
	g_data->done_counter++;
	pthread_mutex_unlock(&g_data->mutex_done);
	return (FALSE);
}
