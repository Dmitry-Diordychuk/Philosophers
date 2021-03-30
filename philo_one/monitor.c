/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 20:26:44 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 18:48:59 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	kill(t_philo **philos)
{
	size_t	i;

	i = 0;
	while (i < g_data->philos_num)
	{
		if (pthread_detach(philos[i]->thread))
			return (THREAD_ERROR);
		i++;
	}
	return (0);
}

int	die(t_philo *ph, t_philo **philos)
{

	int		error;

	if ((error = mprint(ph->thread_start_time, ph->id, "died")) < 0)
		return (PRINTF_ERROR);
	return (kill(philos));
}

uint64_t get_last_meal(t_philo *philo)
{
	uint64_t time;

	if (pthread_mutex_lock(&philo->mutex_meal))
		return (0);
	time = philo->last_meal_time;
	if (pthread_mutex_unlock(&philo->mutex_meal))
		return (0);
	return (time);
}

void	*monitor_ratio(void *args)
{
	size_t	i;
	size_t	left;
	size_t	right;
	t_philo	**philos;

	philos = (t_philo**)args;
	i = 0;
	while (INFINITE_LOOP)
	{
		if (i != 0)
			left = i - 1;
		else
			left = g_data->philos_num - 1;
		if (status(philos[i], GET) == THINK)
			if (get_last_meal(philos[i]) <= get_last_meal(philos[left]))
				if (get_last_meal(philos[i]) <= get_last_meal(philos[right = (i + 1) % (g_data->philos_num - 1)]))
				{
					pthread_mutex_unlock(&philos[i]->mutex_pass);
					pthread_mutex_lock(&philos[i]->mutex_pass);
				}
		i = (i + 1) % (g_data->philos_num - 1);
	}
	return (NULL);
}

int	monitor(t_philo **philos)
{
	uint64_t	cur_time;
	uint64_t	sub_result;
	size_t		i;
	pthread_t	thread;

	g_data->ratio_finished = FALSE;
	pthread_mutex_init(&g_data->ratio_mutex, NULL);
	pthread_create(&thread, NULL, monitor_ratio, philos);
	i = 0;
	while (status(philos[i], GET) == NOT_STARTED)
	{
		pthread_mutex_unlock(&philos[i]->mutex_pass);
		pthread_mutex_lock(&philos[i]->mutex_pass);
		i = (i + 1) % (g_data->philos_num - 1);
	}
	i = 0;
	while (INFINITE_LOOP)
	{
		if (get_time(&cur_time) < 0)
			return (TIME_ERROR);
		sub_result = cur_time - get_last_meal(philos[i]);
		if (sub_result >= g_data->time_to_die)
		{
			pthread_mutex_lock(&g_data->ratio_mutex);
			g_data->ratio_finished = TRUE;
			pthread_detach(thread);
			return (die(philos[i], philos));
		}
		i = (i + 1) % (g_data->philos_num - 1);
	}
	return (ERROR);
}
