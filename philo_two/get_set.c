/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 16:22:03 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 01:37:08 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

uint64_t	get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	if (sem_wait(philo->sem_meal) < 0)
		return (SEM_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	result = cur_time - philo->last_meal_time;
	if (sem_post(philo->sem_meal) < 0)
		return (SEM_ERROR);
	return (result);
}

int			set_meal(t_philo *philo)
{
	if (sem_wait(philo->sem_meal) < 0)
		return (SEM_ERROR);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	if (sem_post(philo->sem_meal) < 0)
		return (SEM_ERROR);
	return (0);
}

int			get_done(void)
{
	t_bool is_done;

	if (sem_wait(g_data->sem_done) < 0)
		return (SEM_ERROR);
	is_done = g_data->is_done;
	if (sem_post(g_data->sem_done) < 0)
		return (SEM_ERROR);
	return (is_done);
}

int			set_done(t_bool bool)
{
	if (sem_wait(g_data->sem_done) < 0)
		return (SEM_ERROR);
	g_data->is_done = bool;
	if (sem_post(g_data->sem_done) < 0)
		return (SEM_ERROR);
	return (0);
}

int			done_counter(void)
{
	if (sem_wait(g_data->sem_done) < 0)
		return (SEM_ERROR);
	g_data->done_counter++;
	if (sem_post(g_data->sem_done) < 0)
		return (SEM_ERROR);
	return (0);
}
