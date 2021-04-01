/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 16:22:03 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 17:20:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

uint64_t	get_meal_time(t_philo *philo)
{
	uint64_t	result;
	uint64_t	cur_time;

	if (sem_wait(&philo->sem_meal) < 0)
		return (SEM_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	result = cur_time - philo->last_meal_time;
	if (sem_post(&philo->sem_meal) < 0)
		return (SEM_ERROR);
	return (result);
}

int			set_meal(t_philo *philo)
{
	if (sem_wait(&philo->sem_meal) < 0)
		return (SEM_ERROR);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	if (sem_post(&philo->sem_meal) < 0)
		return (SEM_ERROR);
	return (0);
}

int			get_done(t_philo *philo)
{
	t_bool is_done;

	if (sem_wait(&philo->sem_done) < 0)
		return (SEM_ERROR);
	is_done = g_is_done;
	if (sem_post(&philo->sem_done) < 0)
		return (SEM_ERROR);
	return (is_done);
}

int			set_done(t_bool bool, sem_t *sem_done)
{
	if (sem_wait(sem_done) < 0)
		return (SEM_ERROR);
	g_is_done = bool;
	if (sem_post(sem_done) < 0)
		return (SEM_ERROR);
	return (0);
}
