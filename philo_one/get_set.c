/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 16:22:03 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 20:21:56 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t	get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	if (pthread_mutex_lock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
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

int			get_done(void)
{
	t_bool is_done;

	if (pthread_mutex_lock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	is_done = g_data->is_done;
	if (pthread_mutex_unlock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	return (is_done);
}

int			set_done(t_bool bool)
{
	if (pthread_mutex_lock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	g_data->is_done = bool;
	if (pthread_mutex_unlock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	return (0);
}

int			done_counter(void)
{
	if (pthread_mutex_lock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	g_data->done_counter++;
	if (pthread_mutex_unlock(&g_data->mutex_done))
		return (MUTEX_ERROR);
	return (0);
}
